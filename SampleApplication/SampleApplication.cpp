#include "SampleApplication.h"
#include <iostream>
#include <exception>

using namespace Xeth;

SampleApplication::SampleApplication(float width, float height, wchar_t title_text[]) : CApplication(width, height, title_text), winsize(width, height) {	}

SampleApplication::~SampleApplication() {	}

void SampleApplication::Run() {
	CApplication::Run();
}

void SampleApplication::SetMouseButtonEvent() {
	SetMouseLButtonDownEvent();
	SetMouseLButtonUpEvent();
	SetMouseRButtonDownEvent();
	SetMouseRButtonUpEvent();
}

bool SampleApplication::Initialize() {
	try {
		SetMouseButtonEvent();	
		bg = new CSprite(L"bg.jpg", Point(winsize.X / 2, winsize.Y / 2));
		bg->SetImage(GetRenderTarget(), GetImageFactory());

		spr = new CSprite(L"c.png", Point(winsize.X / 2, winsize.Y / 2));
		spr->SetImage(GetRenderTarget(), GetImageFactory());
		spr->SetScale(0.05f, 0.05f);

		tex = new CSprite(L"texture.jpg", spr->GetPosition());
		tex->SetImage(GetRenderTarget(), GetImageFactory());

		str = L"HelloWorld\nBeep!";

		CApplication::GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

		mus = new CMusic(L"b.mp3");
		mus->SetVolume(85);
		mus->Run(); 
		run_music = false;
		isMusicRunning = !run_music;

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

void SampleApplication::Display(DWORD delta_time) {
	//idx = (idx + 1) % 2;
	// Set state about Music Playing
	try {
		if(run_music && !isMusicRunning) {
			mus->Run();
			isMusicRunning = !isMusicRunning;
		}
		else if(!run_music && isMusicRunning) {
			mus->Pause();
			isMusicRunning = !isMusicRunning;
		}
	}
	catch(std::exception e) {
		std::cout << e.what() << std::endl;
	}

	/*
	// Process Key Input
	if(GetKeyStatus('W')) {
	spr->SetPosition(spr->GetPosition().X, spr->GetPosition().Y - y_speed);
	tex->SetPosition(spr->GetPosition().X, spr->GetPosition().Y);
	DrawSprite(arr[idx]);
	idx = (idx + 1) % 2;
	}
	if(GetKeyStatus('A')) {
	spr->SetPosition(spr->GetPosition().X - x_speed, spr->GetPosition().Y);
	tex->SetPosition(spr->GetPosition().X, spr->GetPosition().Y);
	DrawSprite(arr[idx]);
	idx = (idx + 1) % 2;
	}
	if(GetKeyStatus('S')) {
	spr->SetPosition(spr->GetPosition().X, spr->GetPosition().Y + y_speed);
	tex->SetPosition(spr->GetPosition().X, spr->GetPosition().Y);
	DrawSprite(arr[idx]);
	idx = (idx + 1) % 2;
	}
	if(GetKeyStatus('D')) {
	spr->SetPosition(spr->GetPosition().X + x_speed, spr->GetPosition().Y);
	tex->SetPosition(spr->GetPosition().X, spr->GetPosition().Y);
	DrawSprite(arr[idx]);
	idx = (idx + 1) % 2;
	}
	if(GetKeyStatus(VK_ESCAPE)){
	isMusicRunning = !isMusicRunning;
	}
	*/
	
	DrawSprite(bg);

	if(rotation >= 360.f) {
	rotation = 0.f;
	}
	rotation += delta_time;

	wsprintf(buffer, L"%d", static_cast<UINT32>(rotation));
	PrintText(buffer, D2D1::RectF(0, 0, CApplication::GetWinSize().X, CApplication::GetWinSize().Y), brush);

	arr[idx]->SetPosition(GetMousePosition().X, GetMousePosition().Y);
	DrawSprite(arr[idx], rotation);
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