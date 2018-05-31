#include "stdafx.h"

#include <string>
#include <functional>

#include "rect.h"

#pragma once

#include "rect.h"

class window;

using event_handler_fn = std::function<LRESULT(window&, UINT, WPARAM, LPARAM)>;
using create_handler = std::function<LRESULT(window&, CREATESTRUCT*)>;

class window {

public:
	void set_title(std::wstring const title);
	std::wstring get_title() const;

	void set_position(rect const position);
	rect get_position() const;

	void show();
	void hide();

	HWND const get_handle() const;

	void set_fallback_handler(event_handler_fn handler);
	event_handler_fn const &get_fallback_handler() const;
	void clear_fallback_handler();

	void run_events();

private:
	// the unique name of the class 
	std::wstring const class_name;	
	// the store location where we store our 'this'
	ATOM const window_instance_atom;
	HWND handle;

	event_handler_fn _unhandled_events;

	create_handler _create_handler;

	window(create_handler create_handler);
	
	void register_class();
	void create_window();
	


	LPARAM event_handler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	friend window make_window(std::wstring const title, rect const location, create_handler create_handler);
	friend LRESULT CALLBACK window_event_loop(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

};

window make_window(std::wstring const title, rect const location, create_handler create_handler);
