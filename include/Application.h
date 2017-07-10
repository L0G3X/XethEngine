#pragma once

#include <d2d1.h>
#include <D2D1Helper.h>
#include <D2Derr.h>

namespace Xeth {
	#define Release(x) if(x) { x->Release(); x = 0 }
	#define Delete(x) if(x) { delete x; x = 0 }

	struct Point {
	public:
		float X;
		float Y;
		Point operator()(float _x, float _y) {
			return Point(_x, _y);
		}
		Point(float _x, float _y) {
			this->X = _x;
			this->Y = float(_y);
		}
	};

	class CApplication {
	public:
		CApplication();
		~CApplication();

	private:
		HRESULT InitializeD2D(HWND hWnd, int width, int height, ID2D1Factory** factory, ID2D1HwndRenderTarget **rt);
		void InitializeWindow();
		void Loop();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		
	private:
		ID2D1Factory* m_factory = 0;
		ID2D1HwndRenderTarget* m_render_target = 0;
		HWND* m_hwnd;
		HINSTANCE* m_hinstance;

		bool isRunning;

	protected:
		virtual bool Initialize() = 0;
		virtual void Display(DWORD delta_time) = 0;
		virtual void Clean() = 0;

		inline ID2D1Factory* GetFactory() { return m_factory; }
		inline ID2D1HwndRenderTarget* GetRenderTarget() { return m_render_target; }
		
	};
}
