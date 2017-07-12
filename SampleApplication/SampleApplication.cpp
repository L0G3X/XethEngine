#include "SampleApplication.h"
#include <iostream>
#include <exception>

using namespace Xeth;

SampleApplication::SampleApplication(float width, float height, wchar_t title_text[]) : CApplication(width, height, title_text), winsize(width, height) {	}

SampleApplication::~SampleApplication() {	}

void SampleApplication::Run() {
	CApplication::Run();
}

bool SampleApplication::Initialize() {
	try {
		bg = new CSprite(L"bg.jpg", Point(winsize.X / 2, winsize.Y / 2));
		bg->SetImage(GetRenderTarget(), GetImageFactory());

		spr = new CSprite(L"c.png", Point(winsize.X / 2, winsize.Y / 2));
		spr->SetImage(GetRenderTarget(), GetImageFactory());
		spr->SetScale(0.7f, 0.7f);

		tex = new CSprite(L"texture.jpg", spr->GetPosition());
		tex->SetImage(GetRenderTarget(), GetImageFactory());

		str = L"HelloWorld\nBeep!";

		CApplication::GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

		mus = new CMusic(L"b.mp3");
		mus->SetVolume(100);
		//mus->Run(); 
		music_button_click = false;
		isMusicRunning = false;

		current = NULL;
		x_speed = y_speed = 8.f;

		rotation = 0.f;
		
		buffer = new wchar_t[256];

		arr = new Xeth::CSprite*[2];

		arr[0] = spr;
		arr[1] = tex;
		idx = 0;
		last = 0;
	}
	catch(std::exception e) {
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}

// Update
void SampleApplication::Display(DWORD delta_time) {
	//idx = (idx + 1) % 2;
	// Set state about Music Playing
	DrawSprite(bg);
	try {
		if(!isMusicRunning && music_button_click) {
			mus->Run();
			isMusicRunning = true;
			music_button_click = false;
		}
		else if(isMusicRunning && music_button_click) {
			mus->Pause();
			isMusicRunning = false; 
			music_button_click = false;
		}
	}
	catch(std::exception e) {
		std::cout << e.what() << std::endl;
	}

	if (GetKeyStatus(VK_ESCAPE)) {
		music_button_click = true;
	}

	if(rotation >= 360.f) {
	rotation = 0.f;
	}
	rotation += delta_time;

	wsprintf(buffer, L"%d", static_cast<UINT32>(rotation));
	SetFont(idx == 0 ? L"±Ã¼­" : L"D2Coding", 40.f);
	PrintText(buffer, D2D1::RectF(0, 0, CApplication::GetWinSize().X, CApplication::GetWinSize().Y), brush);

	arr[idx]->SetPosition(GetMousePosition());
	arr[idx]->SetRotation(rotation);
	DrawSprite(arr[idx]);
}

void SampleApplication::Clean() {
	SafeDelete(buffer);
	SafeDelete(str);
	SafeRelease(spr);
	SafeRelease(bg);
	SafeRelease(mus);
	SafeRelease(brush);
}

void SampleApplication::MouseLButtonDown() {
	idx = 1;
}
void SampleApplication::MouseLButtonUp() {
	idx = 0;
}