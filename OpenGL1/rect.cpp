
#include "stdafx.h"

#include "rect.h"

rect::rect(std::size_t x, std::size_t y, std::size_t width, std::size_t height)
	: x(x), y(y), width(width), height(height) {

}

rect::rect(RECT r) 
	: x(r.left), y(r.top), width(r.right - r.left), height(r.bottom - r.top) {

}