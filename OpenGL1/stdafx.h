// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
//#define _WIN32_WINNT 0x0500
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <windows.h>		// Standard windows headers
#include <tchar.h>			// Unicode support	.. we will use TCHAR rather than char	
#include <commctrl.h>		// Common controls dialogs unit
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

// This is the lazy adding libraries via #pragma rather than in linker includes
// If you are not on visual studio you will need to add the librarys via your linker
#pragma comment(lib,"ComCtl32.lib")
#pragma comment(lib,"OpenGl32.lib")
#pragma comment(lib,"GLU32.lib")

#include <iterator>


// TODO: reference additional headers your program requires here
