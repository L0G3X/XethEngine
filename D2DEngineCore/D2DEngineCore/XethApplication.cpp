#include "XethApplication.h"
#include <iostream>

using namespace Xeth;

#pragma region Static Variables
bool CApplication::isRunning = false;
std::unordered_map<WPARAM, bool> CApplication::keys;
CApplication* CApplication::instance = NULL;
Point CApplication::mousePos = Point(0.0f, 0.0f);
CApplication::FuncPTR CApplication::MouseLButtonDownEvent;
CApplication::FuncPTR CApplication::MouseLButtonUpEvent;
CApplication::FuncPTR CApplication::MouseRButtonDownEvent;
CApplication::FuncPTR CApplication::MouseRButtonUpEvent;
BOOL CApplication::MouseLDownSet;
BOOL CApplication::MouseRDownSet;
BOOL CApplication::MouseLUpSet;
BOOL CApplication::MouseRUpSet;
#pragma endregion

void CApplication::InitializeD2D(HWND hWnd, int width, int height, ID2D1Factory ** factory, ID2D1HwndRenderTarget ** rt) {
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory);
	if(FAILED(hr)) {
		throw new std::exception("Failed to create Factory");
	}
	else {
		(*factory)->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(width, height)), rt);
	}
}

void CApplication::InitializeWIC(IWICImagingFactory ** factory) {
	HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr)) {
		throw std::exception("Failed to Initialize WIC");
	}
	else {
		hr = CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(factory));
		if(FAILED(hr)) {
			throw std::exception("Failed to CoCreate Instance");
		}
	}
}

void CApplication::InitializeDWrite(LPCWSTR font, float size) {
	HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_write_factory), (IUnknown**)&m_write_factory);
	if(FAILED(hr)) {
		throw std::exception("Failed to create DWrite Factory");
	}
	else {
		SetFont(font, size);
	}
}

CApplication::CApplication(float width, float height, wchar_t title_text[]) : m_window_size(width, height) {
	lstrcpyW(title, title_text);
}

void CApplication::Run() {
	InitializeWindow();
	Loop();
	CleanAll();
}

void CApplication::InitializeWindow() {
	HWND hWnd;
	WNDCLASS wc = { 0 };
	HINSTANCE hInstance = GetModuleHandle(0);
	m_factory = NULL;
	m_render_target = NULL;
	instance = this;
	
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"Xeth";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClass(&wc);

	hWnd = CreateWindow(L"Xeth", title, WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX,
						CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(m_window_size.X), static_cast<int>(m_window_size.Y), NULL, NULL, hInstance, NULL);

	m_hwnd = &hWnd;

	try {
		InitializeD2D(hWnd, static_cast<int>(m_window_size.X), static_cast<int>(m_window_size.Y), &m_factory, &m_render_target);

		InitializeWIC(&m_image_factory);
		InitializeDWrite(L"D2Coding", 40.0f);
		m_render_target->BeginDraw();
		if(!Initialize()) {
			throw std::exception("Failed to initialize");
		}
		m_render_target->EndDraw();

		HRESULT hr = ShowWindow(hWnd, SW_SHOWDEFAULT) == 0 ? 0 : -1;
		if(FAILED(hr)) {
			throw std::exception("Failed to Show Window");
		}
	}
	catch(std::exception e) {
		CleanD2DElements();
		throw std::exception(e.what());
	}
	
	m_hinstance = &hInstance;
	instance = this;
	isRunning = true;
}

void CApplication::Loop() {
	MSG msg = { 0 };
	DWORD last = 0L;
	while(isRunning) {
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
// 			if (m_is_box2d_initialized)
// 			{
// 				m_world->Step(16.6f, 6, 2);
// 			}
			m_render_target->BeginDraw();
			auto current = timeGetTime();
			GetRenderTarget()->Clear();
			Display(current - last);

			last = current;
			m_render_target->EndDraw();
			Sleep(16.6);
		}
	}
	CleanAll();
}

void CApplication::CleanD2DElements() {
	SafeRelease(m_render_target);
	SafeRelease(m_factory);
}

void CApplication::CleanAll() {
	Clean();
	CleanD2DElements();
}

LRESULT CApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_CREATE:
			return 0;
		case WM_KEYDOWN:			
			keys[wParam] = true;
			return 0;
		case WM_KEYUP:
			keys[wParam] = false;
			return 0;
		case WM_DESTROY:
			isRunning = false;
			PostQuitMessage(0);
			return 0;
		case WM_MOUSEMOVE:
			mousePos = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_LBUTTONDOWN:
			if (MouseRDownSet != TRUE)
				SetMouseLButtonDownEvent();
			(instance->*MouseLButtonDownEvent)();
			return 0;
		case WM_LBUTTONUP:
			if (MouseLUpSet != TRUE)
				SetMouseLButtonUpEvent();
			(instance->*MouseLButtonUpEvent)();
			return 0;
		case WM_RBUTTONDOWN:
			if (MouseRDownSet != TRUE)
				SetMouseRButtonDownEvent();
			(instance->*MouseRButtonDownEvent)();
			return 0;
		case WM_RBUTTONUP:
			if (MouseRUpSet != TRUE)
				SetMouseRButtonUpEvent();
			(instance->*MouseRButtonUpEvent)();
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Xeth::CApplication::DrawSprite(Xeth::CSprite* spr) {
	spr->GetRotation() == 0.f ? DrawUnRotatedSprite(spr) : DrawRotatedSprite(spr);
}

void Xeth::CApplication::SetFont(LPCWSTR font, int size)
{
	HRESULT hr = m_write_factory->CreateTextFormat(font, 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"ko", &m_write_format);
	if (FAILED(hr)) {
		throw std::exception("Failed to Create Text Format");
	}
}

void Xeth::CApplication::DrawUnRotatedSprite(Xeth::CSprite * spr)
{
	GetRenderTarget()->DrawBitmap(spr->GetBitmap(), spr->GetRect());
}

 void Xeth::CApplication::DrawRotatedSprite(Xeth::CSprite* spr) {
	GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(spr->GetRotation(), D2D1::Point2F(spr->GetPosition().X, spr->GetPosition().Y)));
	DrawUnRotatedSprite(spr);
	GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, D2D1::Point2F(GetWinSize().X / 2, GetWinSize().Y / 2)));
}

void Xeth::CApplication::PrintText(wchar_t* msg, D2D1_RECT_F rect, ID2D1SolidColorBrush* brush) {
	GetRenderTarget()->DrawTextW(msg, static_cast<UINT32>(wcslen(msg)), GetTextFormat(), rect, brush);
}

// void Xeth::CApplication::InitializeBox2D(Point pos)
// {	
// 	m_gravity = new b2Vec2(0.f, -10.f);
// 	m_world = new b2World(*m_gravity);
// 
// 	b2BodyDef groundBodyDef;
// 	groundBodyDef.position.Set(pos.X, pos.Y);
// 
// 	m_groundBody = m_world->CreateBody(&groundBodyDef);
// 	m_is_box2d_initialized = true;
// }
