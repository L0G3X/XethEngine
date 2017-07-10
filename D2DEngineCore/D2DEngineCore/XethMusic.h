#pragma once
#include "XethCore.h"

namespace Xeth {
	class CMusic {
	public:
		CMusic(wchar_t* file_path);
		void Release();

	public:
		void Run();
		void Stop();
		void Pause();
		void SetVolume(float volume);
		void SetBalance(float balance);

	private:
		IBasicAudio* m_volume;
		IMediaPosition* m_position;
		IGraphBuilder* m_graph;
		IMediaControl* m_control;
		IMediaEvent* m_event;
	};
}