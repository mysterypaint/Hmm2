#pragma once
#include <vector>
#include <MidiFile.h>
#include "MIDINote.hpp"
using namespace std;
using namespace smf;

class MIDIChannel {
public:
	int eventPosition = 0;
	int eventMax = 0;
	MIDIChannel();
	~MIDIChannel();
	void SetDeltaTimePointer(float* _addr);

	void SetEventPosition(int _val);
	void SetEventMax(int _val);
	void IncEventPosition();
	int GetEventPosition();
	int GetEventMax();

	void PlayNote(float _freq, float _len);
	void SetNoteVelocity(int _val);
	int GetNoteVelocity();
	void SetChnVolume(int _val);
	int GetChnVolume();
	void SetInstrument(int _val);
	int GetInstrument();
	void SetPitchBend(int _val);
	int GetPitchBend();

	void UpdateActiveNotes();
	vector<MIDINote>* GetActiveNotes();
	void ResetChannel();
	int GetNoteCount();
private:
	float* dT;
	int noteVelocity = 0;
	int channelVolume = 0;
	int instrument = 0;
	int pitchBend = 0;
	vector<MIDINote> activeNotes;
};