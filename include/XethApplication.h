#include "XethCore.h"
#include "XethSprite.h"

#pragma once

namespace Xeth {
	class CApplication {
	public:
		CApplication(float width, float heignt, wchar_t title_text[]);
		void Run();

	private:
		void InitializeWindow();
		void InitializeD2D(HWND hWnd, int width, int height, ID2D1Factory** factory, ID2D1HwndRenderTarget **rt);
		void InitializeWIC(IWICImagingFactory ** factory);
		void InitializeDWrite(LPCWSTR font, float size);

		void Loop();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		void CleanD2DElements();
		void CleanAll();

	private:
		void DrawRotatedSprite(CSprite* spr);
		void DrawUnRotatedSprite(CSprite* spr);

	private:
		static void SetMouseLButtonDownEvent();
		static void SetMouseRButtonDownEvent();
		static void SetMouseLButtonUpEvent();
		static void SetMouseRButtonUpEvent();

	protected:
		virtual bool Initialize();
		virtual void Display(DWORD delta_time);
		virtual void Clean();
		virtual void MouseLButtonDown();
		virtual void MouseRButtonDown();
		virtual void MouseLButtonUp();
		virtual void MouseRButtonUp();

	protected:
		void DrawSprite(CSprite* spr);
		void SetFont(LPCWSTR font, int size);
		void PrintText(wchar_t* msg, D2D1_RECT_F rect, ID2D1SolidColorBrush* brush);

	protected:
		ID2D1Factory*			GetFactory();
		ID2D1HwndRenderTarget*	GetRenderTarget();
		IWICImagingFactory*		GetImageFactory();
		IDWriteTextFormat*		GetTextFormat();
		HWND*					GetHWND();
		Point					GetWinSize();
		Point					GetMousePosition();
		bool					GetKeyStatus(WPARAM key);

		DWORD					GetTimePerFrame();

	private:
		typedef void(CApplication::*FuncPTR)(void);

	private:
		ID2D1Factory*			m_factory;
		ID2D1HwndRenderTarget*	m_render_target;
		HWND*					m_hwnd;
		HINSTANCE*				m_hinstance;
		IWICImagingFactory*		m_image_factory;
		IDWriteFactory*			m_write_factory;
		IDWriteTextFormat*		m_write_format;

		DWORD m_time_per_frame;
		DWORD delta_time;

		Point			m_window_size;
		static bool		isRunning;
		wchar_t			title[256];

		static CApplication* instance;

		static std::unordered_map<WPARAM, bool>	keys;

		static Point	mousePos;
		static BOOL		MouseLDownSet;
		static BOOL		MouseRDownSet;
		static BOOL		MouseLUpSet;
		static BOOL		MouseRUpSet;
		static FuncPTR	MouseLButtonDownEvent;
		static FuncPTR	MouseRButtonDownEvent;
		static FuncPTR	MouseLButtonUpEvent;
		static FuncPTR	MouseRButtonUpEvent;
	};
}