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