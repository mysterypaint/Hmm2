#pragma once

#include "PHL.hpp"

class AudioEngine {
public:
	enum AudioState { INIT, LOADING, PLAYING, PAUSED, ENDING};

	AudioEngine();
	~AudioEngine();
	static void Startup();
	static void Ending();
	static void Step();

	static void LoadMIDI (const char* _f_in);
	static void InitMIDIChannels(int _channelCount);

private:
	static AudioState audioState;
};