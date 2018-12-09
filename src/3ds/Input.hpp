#pragma once

#include <3ds.h>

typedef struct {
	int pressed,
		held,
		released;
} Button;

class InputListener {
public:
	InputListener();
	~InputListener();
	Button btnUp, btnDown, btnLeft, btnRight;
	Button btnFaceUp, btnFaceDown, btnFaceLeft, btnFaceRight;

	Button btnL, btnR;
	Button btnStart, btnSelect;

	Button btnAccept;
	Button btnDecline;

	int axisX, axisY;

	void PHL_ScanInput();
	void updateKey(Button* btn, int state);
};