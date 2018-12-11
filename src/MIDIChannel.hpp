#pragma once
#include <vector>
#include <MidiFile.h>
using namespace std;
using namespace smf;

class MIDIChannel {
public:
	MIDIChannel();
	~MIDIChannel();
	void SetEventPosition(int _val);
	void SetEventMax(int _val);
	void IncEventPosition();
	int GetEventPosition();
	int GetEventMax();
private:
	int eventPosition = 0;
	int eventMax = 999999;
	vector<MidiEvent> events;
};