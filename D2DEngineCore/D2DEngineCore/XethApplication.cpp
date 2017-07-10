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
		hr = m_write_factory->CreateTextFormat(font, 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"ko", &m_write_format);
		if(FAILED(hr)) {
			throw std::exception("Failed to Create Text Format");
		}
	}
}

CApplication::CApplication(float width, float height, wchar_t title_text[]) : m_window_size(width, height), World(b2Vec2(0.0f,0.0f)) {
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
		InitializeDWrite(L"±Ã¼­", 40.0f);
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

	Gravity = b2Vec2(0.0f, -10.0f);
	World = b2World(Gravity);

	b2BodyDef GroundDef;
	GroundDef.position.Set(0.0f, -10.0f);
	GroundBody = World.CreateBody(&GroundDef);

	b2PolygonShape GroundBox;
	GroundBox.SetAsBox(50.f, 10.f);

	GroundBody->CreateFixture(&GroundBox, 0.0f);
	
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
			m_render_target->BeginDraw();
			DWORD current = timeGetTime();

			GetRenderTarget()->Clear();
			Display(current - last);

			last = current;
			m_render_target->EndDraw();
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
			(instance->*MouseLButtonDownEvent)();
			return 0;
		case WM_LBUTTONUP:
			(instance->*MouseLButtonUpEvent)();
			return 0;
		case WM_RBUTTONDOWN:
			(instance->*MouseRButtonDownEvent)();
			return 0;
		case WM_RBUTTONUP:
			(instance->*MouseRButtonUpEvent)();
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Xeth::CApplication::DrawSprite(Xeth::CSprite* spr) {
	GetRenderTarget()->DrawBitmap(spr->GetBitmap(), spr->GetRect());
}

void Xeth::CApplication::DrawSprite(Xeth::CSprite* spr, float rot) {
	GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(rot, D2D1::Point2F(spr->GetPosition().X, spr->GetPosition().Y)));
	DrawSprite(spr);
	GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, D2D1::Point2F(GetWinSize().X / 2, GetWinSize().Y / 2)));
}

void Xeth::CApplication::PrintText(wchar_t* msg, D2D1_RECT_F rect, ID2D1SolidColorBrush* brush) {
	GetRenderTarget()->DrawTextW(msg, static_cast<UINT32>(wcslen(msg)), GetTextFormat(), rect, brush);
}