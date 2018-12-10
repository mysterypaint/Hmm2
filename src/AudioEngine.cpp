#include <MidiFile.h>
#include <set>
#include <utility>
#include <iostream>
#include <string.h>
#include "AudioEngine.hpp"
#include "AudioResources.hpp"
#include <vector>
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
	midiChannels = new MIDIChannel[16]; // Create an array of 16 MIDI Channels
	Startup();
	string f_in = "romfs:/music/m00.mid";
	LoadMIDI(f_in.c_str());
}

AudioEngine::~AudioEngine(void) {
	delete[] midiChannels;
}

void AudioEngine::Startup() {
}

void AudioEngine::Step() {
	printf("%d                        \033[0;0H", tick);
	tick++;
}

void AudioEngine::LoadMIDI(const char* _f_in) {
	MidiFile midifile;
	midifile.read(_f_in);
	if (!midifile.status()) {
		printf("Could not read MIDI file");
		return;
	}

	vector<MIDIEvent> events;
	midifile.sortTracks();
	events.clear();

	printf("\n%d\n", midifile.getTicksPerQuarterNote());
	int total = 0;

	pair<int, int> trackinst;
	set<pair<int, int>> iset;
	for (int chn = 0; chn < midifile.getTrackCount(); chn++) {
		for (int ev = 0; ev < midifile[chn].getEventCount(); ev++) {
			events.push_back(midifile.getEvent(chn, ev));
			if (midifile[chn][ev].isTimbre()) {

				
				trackinst.first = chn;
				trackinst.second = midifile[chn][ev].getP1();
				iset.insert(trackinst);
			}
			//printf("%d\n", midifile[chn][ev].tick);
		}
	}
	//printf("total: %d\n", total);
	/*
	for (auto it : iset)
		printf("\nTrack: %d\tInstrument:%d", it.first, it.second);
		*/


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