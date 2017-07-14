#include <XethCore.h>
#include <XethApplication.h>
#include <XethSprite.h>
#include <XethMusic.h>
#include <XethPhysics.h>

#pragma once
#pragma comment(lib, "D2DEngineCore.lib")

class SampleApplication : Xeth::CApplication {
public:
	SampleApplication(float width, float height, wchar_t titie_text[]);
	~SampleApplication();

public:
	void Run();

// Override Functions
private:
	bool Initialize();
	void Display(DWORD delta_time);
	void Clean();

	void MouseLButtonDown();
	void MouseLButtonUp();

private:
	Xeth::CSprite* spr;
	Xeth::CSprite* bg;
	Xeth::CSprite* tex;

	Xeth::CMusic* mus;

	Xeth::CPhysics* physics;

	Xeth::CSprite** arr;
	int idx;
	bool have_to_draw;
	wchar_t* str;
	wchar_t* buffer;
	DWORD current;
	ID2D1SolidColorBrush* brush = 0;
	Xeth::Point winsize;

	DWORD last;

private:
	float x_speed;
	float y_speed;

	float rotation;

	bool music_button_click;
	bool isMusicRunning;
};