#include "XethCore.h"

#pragma once

namespace Xeth {
	class CSprite {
	public:
		CSprite(wchar_t* file_name, Xeth::Point point);
		void Release();

		void SetImage(ID2D1RenderTarget * _target, IWICImagingFactory * _factory);

		ID2D1Bitmap* GetBitmap();
		D2D1_RECT_F GetRect();
		void SetPosition(float _x, float _y);
		void SetPosition(Point _pos);
		Point GetPosition();
		void SetScale(float _x, float _y);
		Point GetScale();
		void SetRotation(float _rot);
		inline float GetRotation() { return m_rotation; }

	private:
		ID2D1Bitmap* m_bitmap;
		ID2D1BitmapRenderTarget* m_bitmap_render_target;
		Xeth::Point m_pos;
		Xeth::Point m_scale;
		float m_rotation;
		wchar_t* m_file_name;
	};
}

