#include "stdafx.h"

#include <string>
#include <functional>

#include "rect.h"

#pragma once

#include "rect.h"

class window {

public:
	void set_title(std::wstring const title);
	std::wstring get_title() const;

	void set_position(rect const position);
	rect get_position() const;

	void show();
	void hide();

	

private:
	// the unique name of the class 
	std::wstring const class_name;	
	// the store location where we store our 'this'
	ATOM const window_instance_atom;
	HWND handle;

	std::function<int(window&, UINT, WPARAM, LPARAM)> _unhandled_events;


	window();
	
	void register_class();
	void create_window();
	void start_events();


	int event_handler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	friend window create_window(std::wstring const title, rect const location);
	friend LRESULT CALLBACK window_event_loop(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

};

window create_window(std::wstring const title, rect const location);
