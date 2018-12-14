#include "MIDIChannel.hpp"

#include <iostream>
#include <3ds.h>

using namespace std;

MIDIChannel::MIDIChannel() {
}

MIDIChannel::~MIDIChannel() {
}

void MIDIChannel::SetDeltaTimePointer(float* _addr) {
	dT = _addr;
}

void MIDIChannel::SetEventPosition(int _val) {
	eventPosition = _val;
}

void MIDIChannel::SetEventMax(int _val) {
	eventMax = _val;
}

void MIDIChannel::IncEventPosition() {
	eventPosition++;
}

int MIDIChannel::GetEventPosition() {
	return eventPosition;
}

int MIDIChannel::GetEventMax() {
	return eventMax;
}

void MIDIChannel::PlayNote(float _freq, float _len) {
	MIDINote newNote(_freq, _len);
	activeNotes.push_back(newNote);
}

void MIDIChannel::SetNoteVelocity(int _val) {
	noteVelocity = _val;
}
int MIDIChannel::GetNoteVelocity() {
	return noteVelocity;
}
void MIDIChannel::SetChnVolume(int _val) {
	channelVolume = _val;
}
int MIDIChannel::GetChnVolume() {
	return channelVolume;
}
void MIDIChannel::SetInstrument(int _val) {
	instrument = _val;
}
int MIDIChannel::GetInstrument() {
	return instrument;
}
void MIDIChannel::SetPitchBend(int _val) {
	pitchBend = _val;
}
int MIDIChannel::GetPitchBend() {
	return pitchBend;
}

void MIDIChannel::UpdateActiveNotes() {
	size_t size = activeNotes.size();
	for (size_t i = 0; i < size; i++) {		// For every single note that is currently playing,
		float _val = activeNotes[i].GetNoteLifetime();		// Grab the lifetime of each note
		activeNotes[i].SetNoteLifetime(_val - *dT);			// As the song is running, subtract from the note's lifetime until it's time to delete it
		if (activeNotes[i].GetNoteLifetime() <= 0.0f) {		// If the life of the note's playtime is <= 0...
			activeNotes.erase(activeNotes.begin()+i);		// Remove the note's reference from the vector of notes (delete the object, too)
		}
	}
}

vector<MIDINote>* MIDIChannel::GetActiveNotes() {
	return &activeNotes;
}

void MIDIChannel::ResetChannel() {
	// Delete any currently playing notes
	size_t size = activeNotes.size();
	for (size_t i = 0; i < size; i++) {
		activeNotes.erase(activeNotes.begin()+i);
	}
}

int MIDIChannel::GetNoteCount() {
	return activeNotes.size();
}