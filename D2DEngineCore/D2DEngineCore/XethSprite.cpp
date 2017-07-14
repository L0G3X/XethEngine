#include "XethSprite.h"
#include <iostream>
#include <memory>

using namespace Xeth;

CSprite::CSprite(wchar_t* file_name, Point point, IWICImagingFactory* image_factory, ID2D1HwndRenderTarget* render_target, b2World* world) : m_file_name(file_name), m_pos(point), m_first_pos(point), m_scale(1.0f, 1.0f), m_rotation(0.f) {
	HRESULT hr;
	IWICBitmapDecoder *decoder = nullptr;
	hr = image_factory->CreateDecoderFromFilename(m_file_name, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr) || decoder == NULL) {
		char buffer[128];
		FileNotExist(buffer, m_file_name);
		throw std::exception(buffer);
	}

	IWICBitmapFrameDecode* frame_decode = nullptr;

	hr = decoder->GetFrame(0, &frame_decode);
	if (FAILED(hr) || frame_decode == NULL) {
		decoder->Release();
		throw std::exception("Failed to initialize IWICBitmapFrameDecoder");
	}

	IWICFormatConverter* converter = nullptr;
	hr = image_factory->CreateFormatConverter(&converter);
	if (FAILED(hr) || converter == NULL) {
		decoder->Release();
		throw std::exception("Failed to create IWICFormatConverter");
	}

	hr = converter->Initialize(frame_decode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0.0, WICBitmapPaletteTypeCustom);
	if (FAILED(hr)) {
		decoder->Release();
		frame_decode->Release();
		converter->Release();
		throw std::exception("Failed to initialize IWICFormatConverter");
	}

	hr = render_target->CreateBitmapFromWicBitmap(converter, 0, &m_bitmap);
	if (FAILED(hr)) {
		throw std::exception("Failed to create Bitmap");
	}

	converter->Release();
	frame_decode->Release();
	decoder->Release();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_pos.X, m_pos.Y);
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_bitmap->GetSize().width, m_bitmap->GetSize().height);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;
	m_body->CreateFixture(&fixtureDef);
}

Xeth::CSprite::CSprite(wchar_t * file_name, Xeth::Point point, IWICImagingFactory * image_factory, ID2D1HwndRenderTarget * render_target) : m_file_name(file_name), m_pos(point), m_first_pos(point) ,m_scale(1.0f, 1.0f), m_rotation(0.f)
{
	HRESULT hr;
	IWICBitmapDecoder *decoder = nullptr;
	hr = image_factory->CreateDecoderFromFilename(m_file_name, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr) || decoder == NULL) {
		char buffer[128];
		FileNotExist(buffer, m_file_name);
		throw std::exception(buffer);
	}

	IWICBitmapFrameDecode* frame_decode = nullptr;

	hr = decoder->GetFrame(0, &frame_decode);
	if (FAILED(hr) || frame_decode == NULL) {
		decoder->Release();
		throw std::exception("Failed to initialize IWICBitmapFrameDecoder");
	}

	IWICFormatConverter* converter = nullptr;
	hr = image_factory->CreateFormatConverter(&converter);
	if (FAILED(hr) || converter == NULL) {
		decoder->Release();
		throw std::exception("Failed to create IWICFormatConverter");
	}

	hr = converter->Initialize(frame_decode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0.0, WICBitmapPaletteTypeCustom);
	if (FAILED(hr)) {
		decoder->Release();
		frame_decode->Release();
		converter->Release();
		throw std::exception("Failed to initialize IWICFormatConverter");
	}

	hr = render_target->CreateBitmapFromWicBitmap(converter, 0, &m_bitmap);
	if (FAILED(hr)) {
		throw std::exception("Failed to create Bitmap");
	}

	converter->Release();
	frame_decode->Release();
	decoder->Release();

	m_body = nullptr;
}

void CSprite::Release() {
	SafeDelete(m_file_name);
	SafeRelease(m_bitmap);
}

ID2D1Bitmap* CSprite::GetBitmap() {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	return m_bitmap;
}

D2D1_RECT_F CSprite::GetRect() {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	auto size = m_bitmap->GetSize();
	if (m_body != nullptr)
		m_pos = Point{ m_body->GetPosition().x, m_body->GetPosition().y };
	return D2D1::RectF(m_pos.X - (size.width / 2 * m_scale.X), m_pos.Y - (size.height / 2 * m_scale.Y), m_pos.X + (size.width / 2 * m_scale.X), m_pos.Y + (size.height / 2 * m_scale.Y));
}

void CSprite::SetScale(float _x, float _y) {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	this->m_scale.X = _x;
	this->m_scale.Y = _y;
}

void Xeth::CSprite::SetScale(Point _sca)
{
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	this->m_scale = _sca;
}

Point CSprite::GetScale() {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	return m_scale;
}

void CSprite::SetRotation(float _rot) {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	m_rotation = _rot;
}

float Xeth::CSprite::GetRotation() { return m_rotation; }

wchar_t * Xeth::CSprite::GetFileName() { return m_file_name; }

b2Body * Xeth::CSprite::GetBody()
{
	return m_body;
}

void CSprite::SetPosition(float _x, float _y) {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	this->m_pos = Point{ _x, _y };
}

void Xeth::CSprite::SetPosition(Point _pos) {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	this->m_pos = _pos;
}

Point CSprite::GetPosition() {
	if (m_bitmap == nullptr)
		throw std::exception("Bitmap dose not set");
	return m_pos;
}
