#pragma once

#include "stdafx.h"

class rect {

public:
	std::size_t x, y, width, height;

	rect(std::size_t x, std::size_t y, std::size_t width, std::size_t height);
	rect(RECT r);

	RECT asRECT() {
		RECT r{
			static_cast<LONG>(x), 
			static_cast<LONG>(y), 
			static_cast<LONG>(x + width), 
			static_cast<LONG>(y + height)

		};

		return r;
	}

	rect inset(std::size_t top, std::size_t right, std::size_t bottom, std::size_t left) const {
		return rect(x + left, y + top, width - right - left, height - top - bottom);
	}
};