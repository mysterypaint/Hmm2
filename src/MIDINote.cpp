#include "MIDINote.hpp"
MIDINote::MIDINote(int _note, float _len) {
	note = _note;
	noteLifetime = _len; // Note length, but it subtracts and flags itself for deletion once it's <= 0
}

MIDINote::~MIDINote() {

}


void MIDINote::SetNote(int _val) {
	note = _val;
}

void MIDINote::SetNoteLifetime(float _val) {
	noteLifetime = _val;
}

int MIDINote::GetNote() {
	return note;
}

float MIDINote::GetNoteLifetime() {
	return noteLifetime;
}