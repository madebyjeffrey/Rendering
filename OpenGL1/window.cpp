
#include "stdafx.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include "rect.h"
#include "window.h"

static std::wstring generate_unique();
static LRESULT CALLBACK window_event_loop(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

window make_window(std::wstring const title, rect const location, create_handler create_handler)
{
	auto w = window(create_handler);

	w.set_title(title);	
	w.set_position(location);

	return w;
}

void window::set_title(std::wstring const title) {
	SetWindowText(handle, title.c_str());
}

std::wstring window::get_title() const {
	auto length = GetWindowTextLength(handle);

	std::wstring str;
	str.reserve(length);

	int result = GetWindowText(handle, str.data(), length);

	return str;
}

void window::set_position(rect const position) {
	SetWindowPos(handle, HWND_TOP, static_cast<int>(position.x), 
		static_cast<int>(position.y), 
		static_cast<int>(position.width), 
		static_cast<int>(position.height), 
		static_cast<UINT>(SWP_NOZORDER));
}

rect window::get_position() const {
	RECT r;

	GetWindowRect(handle, &r);

	return rect(r);
}

void window::show() {
	SetWindowPos(handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
}

void window::hide() {
	SetWindowPos(handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_HIDEWINDOW);
}

HWND const window::get_handle() const
{
	return handle;
}

void window::set_fallback_handler(event_handler_fn handler)
{
	_unhandled_events = handler;
}
event_handler_fn const &window::get_fallback_handler() const
{
	return _unhandled_events;
}

void window::clear_fallback_handler()
{
	_unhandled_events = event_handler_fn();
}

window::window(create_handler create_handler)
	: class_name(generate_unique()),
	window_instance_atom(GlobalAddAtom(_T("WindowInstance"))),
	_unhandled_events(nullptr),
	handle(nullptr),
	_create_handler(create_handler)
{
	
	register_class();
	bool value1 = static_cast<bool>(_unhandled_events);
	create_window();
	bool value2 = static_cast<bool>(_unhandled_events);
}

void window::register_class() 
{
	WNDCLASSEX WndClass;

	memset(&WndClass, 0, sizeof(WNDCLASSEX));						// Clear the class record
	WndClass.cbSize = sizeof(WNDCLASSEX);							// Size of this record
	WndClass.style = CS_OWNDC;										// Class styles
	WndClass.lpfnWndProc = window_event_loop;						// Handler for this class
	WndClass.cbClsExtra = 0;										// No extra class data
	WndClass.cbWndExtra = sizeof(LPVOID);										// No extra window data
	WndClass.hInstance = GetModuleHandle(NULL);						// This instance
	WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);					// Set icon
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);					// Set cursor
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// Set background brush
	WndClass.lpszMenuName = NULL;									// No menu yet
	WndClass.lpszClassName = class_name.c_str();							// Set class name
	RegisterClassEx(&WndClass);										// Register the class	
}

void window::create_window() {
	handle = CreateWindowEx(0, class_name.c_str(), _T("DefaultLabel"),
		WS_OVERLAPPEDWINDOW, 0, 0,
		10, 10, 0, 0, 0, reinterpret_cast<LPVOID>(this));											// Create a window	
}

// never returns
void window::run_events() {
	MSG Msg;

	while (GetMessage(&Msg, 0, 0, 0)) {								// Get messages
		TranslateMessage(&Msg);										// Translate each message
		DispatchMessage(&Msg);										// Dispatch each message
	};
}

LPARAM window::event_handler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) 
	{
	case WM_CREATE:
		if (_create_handler)
		{
			CREATESTRUCT * cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			return _create_handler(*this, cs);
		}
	default:
		if (_unhandled_events) {
			return _unhandled_events(*this, Msg, wParam, lParam);
		}
	}



	return TRUE;
}

static std::wstring generate_unique() {
	auto uuid = boost::uuids::random_generator()();

	return boost::lexical_cast<std::wstring>(uuid);
}

static LRESULT CALLBACK window_event_loop(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	
	if (Msg != WM_NCCREATE) {
		auto ptr = GetWindowLongPtr(Wnd, 0);
		auto wnd = reinterpret_cast<window*>(ptr);

		if (wnd != nullptr) {
			bool value = static_cast<bool>(wnd->get_fallback_handler());
			return wnd->event_handler(Wnd, Msg, wParam, lParam);
		}	

		return 0;
	}
	
	auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
	// it is WM_NCCREATE
	SetWindowLongPtr(Wnd, 0, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
	return TRUE;
}