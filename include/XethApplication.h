#include "XethCore.h"
#include "XethSprite.h"

#pragma once

namespace Xeth {
	class CApplication {
	public:
		CApplication(float width, float heignt, wchar_t title_text[]);
		void Run();
		typedef void(CApplication::*FuncPTR)(void);

	private:
		typedef void(*func)();
		void InitializeWindow();
		void Loop();
		void CleanD2DElements();
		void CleanAll();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		void InitializeD2D(HWND hWnd, int width, int height, ID2D1Factory** factory, ID2D1HwndRenderTarget **rt);
		void InitializeWIC(IWICImagingFactory ** factory);
		void InitializeDWrite(LPCWSTR font, float size);

		static CApplication* instance;

	private:
		ID2D1Factory* m_factory;
		ID2D1HwndRenderTarget* m_render_target;
		HWND* m_hwnd;
		HINSTANCE* m_hinstance;
		IWICImagingFactory* m_image_factory;
		IDWriteFactory* m_write_factory;
		IDWriteTextFormat* m_write_format;

		Point m_window_size;
		static bool isRunning;
		wchar_t title[256];
		static std::unordered_map<WPARAM, bool> keys;
		static Point mousePos;
		static FuncPTR MouseLButtonDownEvent;
		static FuncPTR MouseRButtonDownEvent;
		static FuncPTR MouseLButtonUpEvent;
		static FuncPTR MouseRButtonUpEvent;

		b2Vec2 Gravity;
		b2Body* GroundBody;

	protected:
		b2World World;

		virtual bool Initialize() = 0;
		virtual void Display(DWORD delta_time) = 0;
		virtual void Clean() = 0;
		virtual void MouseLButtonDown()					{	}
		virtual void MouseRButtonDown()					{	}
		virtual void MouseLButtonUp()					{	}
		virtual void MouseRButtonUp()					{	}

		inline ID2D1Factory* GetFactory()				{ return m_factory; }
		inline ID2D1HwndRenderTarget* GetRenderTarget() { return m_render_target; }
		inline IWICImagingFactory* GetImageFactory()	{ return m_image_factory; }
		inline Point GetWinSize()						{ return m_window_size; }
		inline IDWriteTextFormat* GetTextFormat()		{ return m_write_format; }
		inline HWND* GetHWND()							{ return m_hwnd; }
		inline bool GetKeyStatus(WPARAM key)			{ return keys[key]; }
		inline Point GetMousePosition()					{ return mousePos; }
		inline void SetMouseLButtonDownEvent()			{ MouseLButtonDownEvent = &CApplication::MouseLButtonDown; }
		inline void SetMouseLButtonUpEvent()			{ MouseLButtonUpEvent = &CApplication::MouseLButtonUp; }
		inline void SetMouseRButtonDownEvent()			{ MouseRButtonDownEvent = &CApplication::MouseRButtonDown; }
		inline void SetMouseRButtonUpEvent()			{ MouseRButtonUpEvent = &CApplication::MouseRButtonUp; }

		void DrawSprite(Xeth::CSprite* spr);
		void DrawSprite(Xeth::CSprite* spr, float rot);
		void PrintText(wchar_t* msg, D2D1_RECT_F rect,ID2D1SolidColorBrush* brush);
	};
}
