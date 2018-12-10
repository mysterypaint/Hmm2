#pragma once
#include <vector>
#include <MidiFile.h>
using namespace std;
using namespace smf;

class MIDIChannel {
public:
	MIDIChannel();
	~MIDIChannel();
private:
	vector<MidiEvent> events;
};