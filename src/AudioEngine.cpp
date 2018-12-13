#include <MidiFile.h>
#include <set>
#include <utility>
#include <iostream>
#include <string.h>
#include "Resources.hpp"
#include "AudioEngine.hpp"
#include "AudioResources.hpp"
#include "MIDIChannel.hpp"
#include <vector>
#include <math.h>
using namespace std;
using namespace smf;

int* gameTick;
int songStartTime = 0;
double songTimePos = 0;
double songDuration = 0;
double dT = 1/60.0f;
int songBPM = 0;
int tickRate = 192;
int noteCount = 0;
bool gamePaused = false;
bool loopBGM = true;
PHL_Sound* sound;
MIDIChannel* midiChannels;
MidiFile midifile;
int playedNotes = 0;

AudioEngine::AudioEngine(PHL_Sound* _sounds, int* _tick) {
	gameTick = _tick;
	sound = _sounds;
	midiChannels = new MIDIChannel[17]; // Create an array of 16 MIDI Channels
	//midiChannels(std::vector<MIDIChannel>(16));
	Startup();
	string f_in = "romfs:/music/m01.mid";
	LoadMIDI(f_in.c_str());
}

AudioEngine::~AudioEngine(void) {
	delete[] midiChannels;
}

void AudioEngine::Startup() {
}

void AudioEngine::Step() {
	bool songReset = false;
	hidScanInput();
	u32 kDown = hidKeysHeld();
	if (kDown & KEY_START)
		PHL_GameQuit();

	printf("\nSong Position: %f               ", songTimePos);

	for (int chn = 0; chn < midifile.getTrackCount(); chn++) {

		int eventPosition = midiChannels[chn].GetEventPosition();
		int evMax = midiChannels[chn].GetEventMax();
		if (kDown & KEY_B) {
			int evMax2;
			for (int z = 1; z < midifile.getTrackCount(); z++) {
				evMax2 = midiChannels[z].GetEventMax();
				midiChannels[z].SetEventPosition(evMax2 - 90);
			}
			evMax = midiChannels[chn].GetEventMax();
			eventPosition = midiChannels[chn].GetEventPosition();
			songTimePos = midifile[chn][eventPosition % evMax].seconds;
		}

		if (songTimePos >= midifile[chn][eventPosition % evMax].seconds) {
			while(eventPosition < evMax) {
				if (songTimePos < midifile[chn][eventPosition % evMax].seconds)
					break;
				ProcessMIDIEvent(&midifile[chn][eventPosition % evMax], chn);
				midiChannels[chn].IncEventPosition();
				eventPosition = midiChannels[chn].GetEventPosition();
			}
		}

		if (chn == 3)
			printf("\nNext event: %d / %d (Tick: %f)   \nPlayed Notes: %d / %d\nTicks per Q-Note: %d", eventPosition, evMax, midifile[chn][eventPosition % evMax].seconds, playedNotes, noteCount, songBPM);


		// Now, play the actual audio based on the current MIDIChannel parameters
		
	}

	if (!songReset)
		songTimePos += dT;

	if (songTimePos >= songDuration || (kDown & KEY_X)) { // (eventPosition >= evMax - 1)
		for (int chn = 0; chn < midifile.getTrackCount(); chn++) {
			midiChannels[chn].SetEventPosition(0);
			int eventPosition = midiChannels[chn].GetEventPosition();
			int evMax = midiChannels[chn].GetEventMax();
			while(eventPosition < evMax) {
				if (midifile[chn][eventPosition % evMax].isNoteOn())
					break;
				midiChannels[chn].IncEventPosition();
				eventPosition = midiChannels[chn].GetEventPosition();
			}
		}
		songTimePos = 0;
		playedNotes = 0;
		songReset = true;
	}
}

void AudioEngine::LoadMIDI(const char* _f_in) {
	songStartTime = *gameTick;
	midifile.read(_f_in);
	if (!midifile.status()) {
		printf("Could not read MIDI file");
		return;
	}
	midifile.sortTracks();
	songDuration = midifile.getFileDurationInSeconds();
	for (int chn = 0; chn < midifile.getTrackCount(); chn++) {
		midiChannels[chn].SetEventPosition(0);
		midiChannels[chn].SetEventMax(midifile[chn].getEventCount());
	}
	midifile.doTimeAnalysis();
}


void AudioEngine::ProcessMIDIEvent(MidiEvent* _event, int _midiChannel) {
	if (_event->isNoteOn()) {
		midiChannels[_midiChannel].SetNotePlaying(true);
		if (_midiChannel == 3)
			playedNotes++;
	} else if (_event->isNoteOff()) {
		midiChannels[_midiChannel].SetNotePlaying(false);
	} else if (_event->isTimbre()) {
		int _inst = _event->getP1();
		midiChannels[_midiChannel].SetInstrument(_inst);
	}
}

AudioEngine::AudioState AudioEngine::audioState = INIT;