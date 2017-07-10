#include <d2d1.h>
#include <D2D1Helper.h>
#include <D2Derr.h>
#include <DWrite.h>
#include <dshow.h>
#include <wincodec.h>
#include <exception>
#include <iostream>
#include <unordered_map>

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")
#pragma comment(lib, "Box2D.lib")

#pragma once

namespace Xeth {
	#define SafeRelease(x) if(x) { x->Release(); x = 0; }
	#define SafeDelete(x) if(x) { delete x; x = 0; }
	
	/* W2A(x, y) -> (x = des, y = src) */
	#define W2A(x, y) int len = static_cast<int>(wcslen(y) + 1); x = new char[len]; wcstombs(x, y, len);

	/* FileNotExist(x, y)-> (x = Exception Message, y = File Name) */
	#define FileNotExist(x, y) char* name; W2A(name, y); sprintf(x, "File \"%s\" is not existing", name); delete[] name;

	struct Point {
	public:
		float X;
		float Y;
		Point operator()(float _x, float _y) {
			return Point(_x, _y); 
		}
		Point(float _x, float _y) {
			this->X = _x;
			this->Y = _y;
		}
	};
}

