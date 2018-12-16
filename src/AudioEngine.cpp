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

#include <stdio.h>
#include <stdlib.h>
const double PI = 3.14159265358979;

using namespace std;
using namespace smf;

int* gameTick;
int songStartTime = 0;
double songTimePos = 0;
double songDuration = 0;
float dT = 1/60.0f;
int songBPM = 0;
int tickRate = 192;
int noteCount = 0;
int trackCount = 0;
bool gamePaused = false;
bool loopBGM = true;
PHL_Sound* sound;
MIDIChannel* midiChannels;

MidiFile midifile;
int playedNotes = 0;
// Define the Hz frequency for every single possible MIDI note
float midiNotes[127];


int selectedSong = 0;

#define SAMPLERATE 48000
#define SAMPLESPERBUF (SAMPLERATE / 30)
#define BYTESPERSAMPLE 4

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

u32* audioBuffer = (u32*) linearAlloc(SAMPLESPERBUF * BYTESPERSAMPLE * 2);
bool fillBlock = false;
int filter = 0;
ndspWaveBuf waveBuf[2];
size_t stream_offset = 0;

// audioBuffer is stereo PCM16
void fill_buffer(void* audioBuffer, size_t offset, size_t size) {
	u32* dest = (u32*) audioBuffer;

	for (size_t streamPos = 0; streamPos < size; streamPos++) {
		// Prepare a waveform for the output stream
		s16 outStream = 0;

		for (int chn = 0; chn < trackCount; chn++) {
			if (chn != 11) {
				vector<MIDINote>* activeNotes = midiChannels[chn].GetActiveNotes();
				for (auto aNotes = activeNotes->begin(); aNotes != activeNotes->end(); ++aNotes) {		// For every single note that is currently playing,
					int _note = aNotes->GetNote();		// Grab the frequency of each note
					float _freq = midiNotes[_note + 12];
					// This is a simple sine wave, with a frequency of `frequency` Hz, and an amplitude 30% of maximum.
					outStream += 0.1 * 0x7FFF * sin(_freq * (2 * PI) * (offset + streamPos) / SAMPLERATE);
				}
			}
		}
		// Stereo samples are interleaved: left and right channels.
		dest[streamPos] = (outStream << 16) | (outStream & 0xffff);
	}

/*
	// Reverb algorithm
	int delayMilliseconds = 500; // half a second
	int delaySamples = 
	    (int)((float)delayMilliseconds * (SAMPLERATE * 0.001f));
	float decay = 0.5f;
	for (int i = 0; i < size - delaySamples; i++)
	{
	    // WARNING: overflow potential
	    dest[i + delaySamples] += (s16)((float)dest[i] * decay);
	}
*/

	DSP_FlushDataCache(audioBuffer, size);
}


AudioEngine::AudioEngine(PHL_Sound* _sounds, int* _tick) {
	gameTick = _tick;
	sound = _sounds;
	midiChannels = new MIDIChannel[17]; // Create an array of 16 MIDI Channels
	for (int i = 0; i < 16; i++)
		midiChannels[i].SetDeltaTimePointer(&dT);

	// Define every MIDI pitch, in terms of Hz
	int a = 440; // a is 440 hz...
	for (int x = 0; x < 127; ++x)
	{
	   midiNotes[x] = (a / 32.0f) * (pow(2, ((x - 9) / 12.0f)));
	}


	//Streaming Audio
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
	ndspChnSetRate(0, SAMPLERATE);
	ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);
	// Output at 100% on the first pair of left and right channels.
	float mix[12];
	memset(mix, 0, sizeof(mix));
	mix[0] = 1.0;
	mix[1] = 1.0;
	ndspChnSetMix(0, mix);

	memset(waveBuf,0,sizeof(waveBuf));
	waveBuf[0].data_vaddr = &audioBuffer[0];
	waveBuf[0].nsamples = SAMPLESPERBUF;
	waveBuf[1].data_vaddr = &audioBuffer[SAMPLESPERBUF];
	waveBuf[1].nsamples = SAMPLESPERBUF;

	fill_buffer(audioBuffer,stream_offset, SAMPLESPERBUF * 2);

	stream_offset += SAMPLESPERBUF;

	ndspChnWaveBufAdd(0, &waveBuf[0]);
	ndspChnWaveBufAdd(0, &waveBuf[1]);



	//midiChannels(std::vector<MIDIChannel>(16));
	Startup();


	printf("Left/Right: Move 1\nUp/Down: Move 10\nA: Select song\n\n%d: %s\033[0;0H", selectedSong, songNamesEN[jukeboxOrder[selectedSong]]);

	while(true) {
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_UP) {
			consoleClear();
			selectedSong -= 10;
		} else if (kDown & KEY_DOWN) {
			consoleClear();
			selectedSong += 10;
		} else if (kDown & KEY_LEFT) {
			consoleClear();
			selectedSong -= 1;
		} else if (kDown & KEY_RIGHT) {
			consoleClear();
			selectedSong += 1;
		}
		if (selectedSong < 0)
			selectedSong = ARRAY_SIZE(songNamesEN) - 1;
		else if (selectedSong > ARRAY_SIZE(songNamesEN) - 1)
			selectedSong = 0;

		if (kDown & KEY_A)
			break;

		printf("Left/Right: Move 1\nUp/Down: Move 10\nA: Select song\n\n%d: %s\033[0;0H", selectedSong, songNamesEN[jukeboxOrder[selectedSong]]);
	}
	consoleClear();

	string num = "";
	if (jukeboxOrder[selectedSong] < 10)
		num += '0';

  	char buffer [50];
	sprintf(buffer, "%d", jukeboxOrder[selectedSong]);
	//num += '3';
	num += buffer;

	string f_in = "romfs:/music/m" + num + ".mid";
	printf("Playing: %s\n\nFile: %s", songNamesEN[jukeboxOrder[selectedSong]], f_in.c_str());
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

	//printf("\nSong Position: %f               ", songTimePos);

	for (int chn = 0; chn < trackCount; chn++) {

		int eventPosition = midiChannels[chn].GetEventPosition();
		int evMax = midiChannels[chn].GetEventMax();
		if (kDown & KEY_B) {
			int evMax2;
			for (int z = 0; z < trackCount; z++) {
				evMax2 = midiChannels[z].GetEventMax();
				midiChannels[z].SetEventPosition(evMax2 - 90);
			}
			evMax = midiChannels[chn].GetEventMax();
			eventPosition = midiChannels[chn].GetEventPosition();
			songTimePos = midifile[chn][eventPosition % evMax].seconds;
		}


		midiChannels[chn].UpdateActiveNotes();










		if (songTimePos >= midifile[chn][eventPosition % evMax].seconds) {
			while(eventPosition < evMax) {
				if (songTimePos < midifile[chn][eventPosition % evMax].seconds)
					break;
				ProcessMIDIEvent(&midifile[chn][eventPosition % evMax], chn);
				midiChannels[chn].IncEventPosition();
				eventPosition = midiChannels[chn].GetEventPosition();
			}
		}

		//if (chn == 2)
			//printf("\nNext event: %d / %d (Tick: %f)   \nPlayed Notes: %d / %d\nTicks per Q-Note: %d", eventPosition, evMax, midifile[chn][eventPosition % evMax].seconds, playedNotes, noteCount, songBPM);


		// Now, play the actual audio based on the current MIDIChannel parameters

	}

	if (!songReset)
		songTimePos += dT;

	if (songTimePos >= songDuration || (kDown & KEY_X)) { // (eventPosition >= evMax - 1)
		for (int chn = 0; chn < trackCount; chn++) {
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

	

	// Streaming time
	if (waveBuf[fillBlock].status == NDSP_WBUF_DONE) {
		fill_buffer(waveBuf[fillBlock].data_pcm16, stream_offset, waveBuf[fillBlock].nsamples);
		ndspChnWaveBufAdd(0, &waveBuf[fillBlock]);
		stream_offset += waveBuf[fillBlock].nsamples;
		fillBlock = !fillBlock;
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
	trackCount = midifile.getTrackCount();

	for (int chn = 0; chn < midifile.getTrackCount(); chn++) {
		midiChannels[chn].SetEventPosition(0);
		midiChannels[chn].SetEventMax(midifile[chn].getEventCount());
	}
	midifile.doTimeAnalysis();
	midifile.linkNotePairs();
}


void AudioEngine::ProcessMIDIEvent(MidiEvent* _event, int _midiChannel) {
	if (_event->isNoteOn()) {
		int key = _event->getKeyNumber();
		float noteLen = _event->getDurationInSeconds();
		if (_midiChannel != 10) {
			midiChannels[_midiChannel].PlayNote(key, noteLen);
		}
		if (_midiChannel == 2) {
			//printf("Ch%d: %s (%fHz)\nPolyphony: %d", _midiChannel, NoteName(key), midiNotes[key], midiChannels[_midiChannel].GetNoteCount());
			/*
			PHL_StopSound(sound[SE00], 1);
			PHL_PlaySound(sound[SE00], 1);
			*/

			playedNotes++;
		}
	} else if (_event->isTimbre()) {
		int _inst = _event->getP1();
		midiChannels[_midiChannel].SetInstrument(_inst);
	}
}

const char* AudioEngine::NoteName(int _inNote) {
	string s = "   ";
	
	int relativeNote = _inNote % 12;
	int octave = _inNote / 12;

	s[1] = '-';
	switch(relativeNote) {
		default:
		case 1:
			s[1] = '#';
		case 0:
			s[0] = 'C';
			break;
		case 3:
			s[1] = '#';
		case 2:
			s[0] = 'D';
			break;
		case 4:
			s[0] = 'E';
			break;
		case 6:
			s[1] = '#';
		case 5:
			s[0] = 'F';
			break;
		case 8:
			s[1] = '#';
		case 7:
			s[0] = 'G';
			break;
		case 10:
			s[1] = '#';
		case 9:
			s[0] = 'A';
			break;
		case 11:
			s[0] = 'B';
			break;
	}
	
	s[2] = '0' + octave;
	return s.c_str();
}

AudioEngine::AudioState AudioEngine::audioState = INIT;