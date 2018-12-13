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

void MIDIChannel::SetNotePlaying(bool _val) {
	notePlaying = _val;
}
bool MIDIChannel::GetNotePlaying() {
	return notePlaying;
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
void MIDIChannel::SetPitch(int _val) {
	pitch = _val;
}
int MIDIChannel::GetPitch() {
	return pitch;
}