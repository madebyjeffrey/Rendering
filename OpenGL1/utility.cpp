#include "stdafx.h"
#include "utility.h"
#include "rect.h"

namespace desktop {
	rect get_size() {
		RECT R;

		GetClientRect(GetDesktopWindow(), &R);

		return rect(R);
	}
}