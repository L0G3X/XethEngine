#include "XethCore.h"

#pragma once

namespace Xeth {
	class CSprite {
	public:
		CSprite(wchar_t* file_name, Xeth::Point point, IWICImagingFactory* image_factory, ID2D1HwndRenderTarget* render_target, b2World* world);
		CSprite(wchar_t* file_name, Xeth::Point point, IWICImagingFactory* image_factory, ID2D1HwndRenderTarget* render_target);
		void Release();

		ID2D1Bitmap* GetBitmap();
		D2D1_RECT_F GetRect();
		void SetPosition(float _x, float _y);
		void SetPosition(Point _pos);
		Point GetPosition();
		void SetScale(float _x, float _y);
		void SetScale(Point _sca);
		Point GetScale();
		void SetRotation(float _rot);
		float GetRotation();
		wchar_t* GetFileName();

		b2Body* GetBody();

	public:
		bool IsSet;

	private:
		ID2D1Bitmap* m_bitmap;
		Xeth::Point m_pos;
		Xeth::Point m_first_pos;
		Xeth::Point m_scale;
		float m_rotation;
		wchar_t* m_file_name;

		b2Body* m_body;
	};
}

