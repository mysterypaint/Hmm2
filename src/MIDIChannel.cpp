#include "MIDIChannel.hpp"

#include <iostream>
#include <3ds.h>

using namespace std;

MIDIChannel::MIDIChannel() {
}

MIDIChannel::~MIDIChannel() {
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
	return;
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