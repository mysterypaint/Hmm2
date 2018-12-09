#include "System.hpp"
#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>

int quitGame = 0;

int PHL_MainLoop() {
	if (quitGame == 1) {
		return 0;
	}
	
	return aptMainLoop();
}

void PHL_ConsoleInit() {
	consoleInit(GFX_TOP, NULL); // Initializes the console for printing debug stuff. Not to be confused with turning the screen on for displaying graphics.
}

void PHL_GameQuit() {
	quitGame = 1;
}

void PHL_ErrorScreen(char* message) {
	consoleInit(GFX_TOP, NULL);
	
	printf(message);
	printf("\n\nPress START to close");
	
	u32 kDown;
	while (PHL_MainLoop()) {
		//gfxFlushBuffers();
		//gfxSwapBuffers();
		hidScanInput();
		kDown = hidKeysHeld();
		
		if (kDown & KEY_START)
			break;
		
		gspWaitForVBlank();
	}
	
	exit(1);
}