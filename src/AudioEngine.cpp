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
using namespace std;
using namespace smf;


#define SAMPLERATE 48000
#define SAMPLESPERTICK (SAMPLERATE / 60)

double bpm = 136;
double ticksPerMinute = 0;//bpm * midifile.getTicksPerQuarterNote();
double ticksPerSecond = ticksPerMinute / 60;
double samplesPerTick = 44100 / ticksPerSecond;

int* gameTick;
int songBPM = 0;
int tickRate = 192;
int songTick = 0;
int noteCount = 0;
float deltaTime = 1.0f;
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
	string f_in = "romfs:/music/m00.mid";
	LoadMIDI(f_in.c_str());
}

AudioEngine::~AudioEngine(void) {
	delete[] midiChannels;
}

void AudioEngine::Startup() {
}

void AudioEngine::Step() {
	printf("\nSong Position: %d               ", songTick);
	int chn = 3;

	int eventPosition = midiChannels[chn].GetEventPosition();
	int evMax = midiChannels[chn].GetEventMax();

	if (songTick == midifile[chn][eventPosition].tick) {
		PHL_PlaySound(sound[SE00], 1);
		playedNotes++;
		midiChannels[chn].IncEventPosition();
		eventPosition = midiChannels[chn].GetEventPosition();

		while(!midifile[chn][eventPosition].isNoteOn()) {
			midiChannels[chn].IncEventPosition();
			eventPosition = midiChannels[chn].GetEventPosition();
		}
		midiChannels[chn].IncEventPosition();
	}

	printf("\nNext event: %d / %d (Tick: %d)          \nPlayed Notes: %d / %d\nTicks per Q-Note: %d", eventPosition, evMax, midifile[chn][eventPosition].tick, playedNotes, noteCount, songBPM);



/*
	for (int chn = 0; chn < midifile.getTrackCount(); chn++) {
		if (midifile[chn][songTick].isNoteOn()) {
			PHL_PlaySound(sound[SE00], 1);
		}
		//printf("%d: %d\n",chn,midiChannels[chn].GetEventMax());
		
	}
*/

	if (*gameTick % (1) == 0) { // songBPM
		songTick+=1;
	}
}

void AudioEngine::LoadMIDI(const char* _f_in) {
	songTick = 0;
	midifile.read(_f_in);
	if (!midifile.status()) {
		printf("Could not read MIDI file");
		return;
	}
	midifile.sortTracks();

	songBPM = midifile.getTicksPerQuarterNote();
	ticksPerMinute = bpm * midifile.getTicksPerQuarterNote();
	ticksPerSecond = ticksPerMinute / 60;
	samplesPerTick = 44100 / ticksPerSecond;

	//printf("\n%d\n", midifile.getTicksPerQuarterNote());
	int total = 0;

	pair<int, int> trackinst;
	set<pair<int, int>> iset;
	for (int chn = 0; chn < midifile.getTrackCount(); chn++) {
		midiChannels[chn].SetEventPosition(0);
		midiChannels[chn].SetEventMax(midifile[chn].getEventCount());

		for (int ev = 0; ev < midifile[chn].getEventCount(); ev++) {
			if (midifile[chn][ev].isTimbre()) {

				
				trackinst.first = chn;
				trackinst.second = midifile[chn][ev].getP1();
				iset.insert(trackinst);
			}
			//printf("%d\n", midifile[chn][ev].tick);
		}
	}

	for (int ev = 0; ev < midifile[3].getEventCount(); ev++) {
		if (midifile[3][ev].isNoteOn()) {
			noteCount++;
			//printf("%d\n", midifile[3][ev].tick);
		}
	}

	//printf("total: %d\n", total);
	/*
	for (auto it : iset)
		printf("\nTrack: %d\tInstrument:%d", it.first, it.second);
		*/


   midifile.doTimeAnalysis();
   midifile.linkNotePairs();
   //printf("%d", midifile.isAbsoluteTicks());



	for (int chn = 0; chn < midifile.getTrackCount(); chn++) {
		//printf("%d: %d\n",chn,midiChannels[chn].GetEventMax());
	}
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