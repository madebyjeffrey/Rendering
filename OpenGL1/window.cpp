
#include "stdafx.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include "rect.h"
#include "window.h"

static std::wstring generate_unique();
static LRESULT CALLBACK window_event_loop(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

window create_window(std::wstring const title, rect const location) 
{
	auto w = window();

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
	SetWindowPos(handle, HWND_TOP, position.x, position.y, position.width, position.height, static_cast<UINT>(SWP_NOZORDER));
}

rect window::get_position() const {
	RECT r;

	GetWindowRect(handle, &r);

	return rect(r);
}

void window::show() {
	SetWindowPos(handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

void window::hide() {
	SetWindowPos(handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_HIDEWINDOW);
}

window::window()
	: class_name(generate_unique()),
	window_instance_atom(GlobalAddAtom(_T("WindowInstance"))),
	_unhandled_events()
{
	register_class();
	create_window();
}

void window::register_class() 
{
	WNDCLASSEX WndClass;

	memset(&WndClass, 0, sizeof(WNDCLASSEX));						// Clear the class record
	WndClass.cbSize = sizeof(WNDCLASSEX);							// Size of this record
	WndClass.style = CS_OWNDC;										// Class styles
	WndClass.lpfnWndProc = window_event_loop;						// Handler for this class
	WndClass.cbClsExtra = 0;										// No extra class data
	WndClass.cbWndExtra = 0;										// No extra window data
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
void window::start_events() {
	MSG Msg;

	while (GetMessage(&Msg, 0, 0, 0)) {								// Get messages
		TranslateMessage(&Msg);										// Translate each message
		DispatchMessage(&Msg);										// Dispatch each message
	};
}

int window::event_handler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	if (_unhandled_events) {
		return _unhandled_events(*this, Msg, wParam, lParam);
	}

	return TRUE;
}

static std::wstring generate_unique() {
	auto uuid = boost::uuids::random_generator()();

	return boost::lexical_cast<std::wstring>(uuid);
}

static LRESULT CALLBACK window_event_loop(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (Msg != WM_NCCREATE) {
		auto ptr = GetWindowLongPtr(Wnd, GWLP_USERDATA);
		auto wnd = reinterpret_cast<window*>(ptr);
		return wnd->event_handler(Wnd, Msg, wParam, lParam);
	}
	
	// it is WM_NCCREATE
	SetWindowLongPtr(Wnd, GWLP_USERDATA, lParam);
	return TRUE;
}