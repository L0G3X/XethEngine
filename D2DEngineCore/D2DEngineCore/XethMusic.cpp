#include "XethMusic.h"

using namespace Xeth;

CMusic::CMusic(wchar_t* file_path) {
	HRESULT hr;
	hr = CoInitialize(NULL);
	if(FAILED(hr)) {
		throw new std::exception("Failed to CoInitialize");
	}
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, reinterpret_cast<void**>(&m_graph));
	if(FAILED(hr)) {
		throw new std::exception("Failed to CoCreate Instance");
	}
	hr = m_graph->QueryInterface(IID_IMediaControl, reinterpret_cast<void**>(&m_control));
	if(FAILED(hr)) {
		throw new std::exception("Failed to create MediaControl");
	}
	hr = m_graph->QueryInterface(IID_IMediaEvent, reinterpret_cast<void**>(&m_event));
	if(FAILED(hr)) {
		throw new std::exception("Failed to create MediaEvent");
	}
	hr = m_graph->QueryInterface(IID_IMediaPosition, reinterpret_cast<void**>(&m_position));
	if(FAILED(hr)) {
		throw new std::exception("Failed to create MediaPosition");
	}
	hr = m_graph->QueryInterface(IID_IBasicAudio, reinterpret_cast<void**>(&m_volume));
	if(FAILED(hr)) {
		throw std::exception("Failed to create BasicAudio");
	}
	hr = m_graph->RenderFile(file_path, NULL);
	if(FAILED(hr)) {
		char buffer[128];
		FileNotExist(buffer, file_path);
		throw std::exception(buffer);
	}
}

void CMusic::Release() {
	SafeRelease(m_control);
	SafeRelease(m_event);
	SafeRelease(m_position);
	SafeRelease(m_volume);
	SafeRelease(m_graph);
}

void CMusic::Run() {
	HRESULT hr;
	hr = m_control->Run();
	if(FAILED(hr)) {
		throw std::exception("Failed to Run Music");
	}
}

void CMusic::Stop() {
	HRESULT hr;
	hr = m_control->Stop();
	if(FAILED(hr)) {
		throw std::exception("Failed to Stop Music");
	}
	hr = m_position->put_CurrentPosition(0);
	if(FAILED(hr)) {
		throw std::exception("Failed to Reset Music Position");
	}
}

void CMusic::Pause() {
	HRESULT hr;
	hr = m_control->Pause();
	if(FAILED(hr)) {
		throw std::exception("Failed to Pause Music");
	}
}

void CMusic::SetVolume(int volume) {
	this->volume = volume;
	HRESULT hr;
	hr = m_volume->put_Volume(static_cast<long>(100 * this->volume - 10000));
	if(FAILED(hr)) {
		throw std::exception("Failed to Set Volume");
	}
}

void CMusic::SetBalance(int balance) {
	this->balance = balance;
	HRESULT hr;
	hr = m_volume->put_Balance(static_cast<long>(100 * this->balance));
	if(FAILED(hr)) {
		throw std::exception("Failed to Set Balance");
	}
}
