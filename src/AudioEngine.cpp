#include <MidiFile.h>
#include <set>
#include <utility>
#include <iostream>
#include <string.h>
#include "AudioEngine.hpp"

using namespace std;
using namespace smf;


/*

double ticksPerMinute = bpm * midifile.getTicksPerQuarterNote();
double ticksPerSecond = ticksPerMinute / 60;
double samplesPerTick = 44100 / ticksPerSecond;
*/
#define SAMPLERATE 48000
#define SAMPLESPERTICK (SAMPLERATE / 60)

int tick = 0;
float deltaTime = 1.0f;
bool gamePaused = false;
bool loopBGM = true;

AudioEngine::AudioEngine(void) {
	InitMIDIChannels(16);
	string f_in = "romfs:/music/m01.mid";
	LoadMIDI(f_in.c_str());
}

AudioEngine::~AudioEngine(void) {
}

void AudioEngine::Step() {
	printf("%d                        \033[0;0H", tick);
	tick++;
}

void AudioEngine::InitMIDIChannels(int _channelCount) {
}

void AudioEngine::LoadMIDI(const char* _f_in) {
	MidiFile midifile;
	midifile.read(_f_in);
	if (!midifile.status()) {
		printf("Could not read MIDI file");
		return;
	}


	pair<int, int> trackinst;
	set<pair<int, int>> iset;
	for (int i=0; i<midifile.getTrackCount(); i++) {
		for (int j=0; j<midifile[i].getEventCount(); j++) {
			if (midifile[i][j].isTimbre()) {
				trackinst.first = i;
				trackinst.second = midifile[i][j].getP1();
				iset.insert(trackinst);
			}
		}
	}
	for (auto it : iset)
		printf("Track: %d\tInstrument:%d\n", it.first, it.second);


   midifile.doTimeAnalysis();
   midifile.linkNotePairs();
   printf("%d", midifile.isAbsoluteTicks());
}





/*
   int tracks = midifile.getTrackCount();
   printf("TPQ: %d", midifile.getTicksPerQuarterNote());
   if (tracks > 1) printf("TRACKS: %d", tracks);
   for (int track=0; track<tracks; track++) {
      if (tracks > 1) printf("\nTrack %d", track);
      //printf("Tick\tSeconds\tDur\tMessage");
      for (int event=0; event<midifile[track].size(); event++) {

         printf("%d", midifile[track][event].tick);
         printf("\t%f", midifile[track][event].seconds);
         printf("\t");
         if (midifile[track][event].isNoteOn())
            printf("%f", midifile[track][event].getDurationInSeconds());
         printf("\t");
         for (int i=0; i<midifile[track][event].size(); i++)
            printf(" %x", (int)midifile[track][event][i]);
         printf("\n");
      }
   }
*/

AudioEngine::AudioState AudioEngine::audioState = INIT;