#pragma once

#include <vector>
#include <MidiEvent.h>
#include "PHL.hpp"
#include "MIDIChannel.hpp"

class AudioEngine {
public:
	enum AudioState { INIT, LOADING, PLAYING, PAUSED, ENDING};

	AudioEngine(PHL_Sound* _sounds, int* _tick);
	~AudioEngine();
	static void Startup();
	static void Ending();
	static void Step();

	static void LoadMIDI (const char* _f_in);
	static void ProcessMIDIEvent(MidiEvent* _event, int _midiChannel);
private:
	static AudioState audioState;
	//vector<MIDIChannel> midiChannels;
};