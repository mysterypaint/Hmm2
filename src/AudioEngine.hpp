#pragma once

#include <vector>
#include <MidiEvent.h>
#include "PHL.hpp"
#include "MIDIChannel.hpp"

class AudioEngine {
public:
	enum AudioState { INIT, LOADING, PLAYING, PAUSED, ENDING};

	AudioEngine();
	~AudioEngine();
	static void Startup();
	static void Ending();
	static void Step();

	static void LoadMIDI (const char* _f_in);

private:
	static AudioState audioState;
	MIDIChannel* midiChannels;
};