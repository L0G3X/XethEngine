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
		static inline void SetMouseLButtonDownEvent() { MouseLButtonDownEvent = &CApplication::MouseLButtonDown; MouseLDownSet = TRUE; }
		static inline void SetMouseRButtonDownEvent() { MouseRButtonDownEvent = &CApplication::MouseRButtonDown; MouseRDownSet = TRUE; }
		static inline void SetMouseLButtonUpEvent() { MouseLButtonUpEvent = &CApplication::MouseLButtonUp; MouseLUpSet = TRUE; }
		static inline void SetMouseRButtonUpEvent() { MouseRButtonUpEvent = &CApplication::MouseRButtonUp; MouseRUpSet = TRUE; }

	protected:
		virtual bool Initialize()						{ return true; }
		virtual void Display(DWORD delta_time)			{	}
		virtual void Clean()							{	}
		virtual void MouseLButtonDown()					{	}
		virtual void MouseRButtonDown()					{	}
		virtual void MouseLButtonUp()					{	}
		virtual void MouseRButtonUp()					{	}

	protected:
		void DrawSprite(CSprite* spr);
		void SetFont(LPCWSTR font, int size);
		void PrintText(wchar_t* msg, D2D1_RECT_F rect, ID2D1SolidColorBrush* brush);

	protected:
		inline ID2D1Factory*			GetFactory()				{ return m_factory; }
		inline ID2D1HwndRenderTarget*	GetRenderTarget()			{ return m_render_target; }
		inline IWICImagingFactory*		GetImageFactory()			{ return m_image_factory; }
		inline IDWriteTextFormat*		GetTextFormat()				{ return m_write_format; }
		inline HWND*					GetHWND()					{ return m_hwnd; }
		inline Point					GetWinSize()				{ return m_window_size; }
		inline Point					GetMousePosition()			{ return mousePos; }
		inline bool						GetKeyStatus(WPARAM key)	{ return keys[key]; }

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

		Point		m_window_size;
		static bool isRunning;
		wchar_t		title[256];

		static CApplication* instance;

		static std::unordered_map<WPARAM, bool> keys;

		static Point	mousePos;
		static FuncPTR	MouseLButtonDownEvent;
		static FuncPTR	MouseRButtonDownEvent;
		static FuncPTR	MouseLButtonUpEvent;
		static FuncPTR	MouseRButtonUpEvent;
		static BOOL		MouseLDownSet;
		static BOOL		MouseRDownSet;
		static BOOL		MouseLUpSet;
		static BOOL		MouseRUpSet;
	};
}
