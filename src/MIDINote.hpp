#pragma once

class MIDINote {
public:
	MIDINote(int _note, float _len);
	~MIDINote();
	void SetNote(int _val);
	void SetNoteLifetime(float _val);
	int GetNote();
	float GetNoteLifetime();
private:
	float noteLifetime = 0; // Note length, but it subtracts and flags itself for deletion once it's <= 0
	int note = 0;
};