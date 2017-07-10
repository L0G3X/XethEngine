#include "XethSprite.h"
#include <iostream>
#include <memory>

using namespace Xeth;

CSprite::CSprite(wchar_t* file_name, Point point) : m_file_name(file_name), m_pos(point), m_scale(1.0f, 1.0f), m_rotation(0.f) {	}

void CSprite::Release() {
	SafeRelease(m_bitmap);
}

void CSprite::SetImage(ID2D1RenderTarget * target, IWICImagingFactory * factory) {
	HRESULT hr;
	IWICBitmapDecoder *decoder = 0;
	hr = factory->CreateDecoderFromFilename(m_file_name, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if(FAILED(hr) || decoder == NULL) {
		char buffer[128];
		FileNotExist(buffer, m_file_name);
		throw std::exception(buffer);
	}

	IWICBitmapFrameDecode* frame_decode = 0;

	hr = decoder->GetFrame(0, &frame_decode);
	if(FAILED(hr) || frame_decode == NULL) {
		decoder->Release();
		throw std::exception("Failed to initialize IWICBitmapFrameDecoder");
	}

	IWICFormatConverter* converter = 0;
	hr = factory->CreateFormatConverter(&converter);
	if(FAILED(hr) || converter == NULL) {
		decoder->Release();
		throw std::exception("Failed to create IWICFormatConverter");
	}

	hr = converter->Initialize(frame_decode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0.0, WICBitmapPaletteTypeCustom);
	if(FAILED(hr)) {
		decoder->Release();
		frame_decode->Release();
		converter->Release();
		throw std::exception("Failed to initialize IWICFormatConverter");
	}

	hr = target->CreateBitmapFromWicBitmap(converter, 0, &m_bitmap);
	if(FAILED(hr)) {
		throw std::exception("Failed to create Bitmap");
	}

	decoder->Release();
	frame_decode->Release();
	converter->Release();
}

ID2D1Bitmap* CSprite::GetBitmap() {
	return m_bitmap;
}

D2D1_RECT_F CSprite::GetRect() {
	auto size = this->GetBitmap()->GetSize();
	return D2D1::RectF(m_pos.X - (size.width / 2 * m_scale.X), m_pos.Y - (size.height / 2 * m_scale.Y), m_pos.X + (size.width / 2 * m_scale.X), m_pos.Y + (size.height / 2 * m_scale.Y));
}

void CSprite::SetScale(float _x, float _y) {
	this->m_scale.X = _x;
	this->m_scale.Y = _y;
}

Point CSprite::GetScale() {
	return m_scale;
}

void CSprite::SetRotation(float setha) {
	m_rotation = setha;
}

void CSprite::SetPosition(float _x, float _y) {
	this->m_pos.X = _x;
	this->m_pos.Y = _y;
}

Point CSprite::GetPosition() {
	return m_pos;
}
