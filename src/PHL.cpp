#include "PHL.hpp"

void PHL_Init() {
	PHL_GraphicsInit();
	PHL_ConsoleInit();
	PHL_AudioInit();

	#ifdef _3DS
		sdmcInit();
		osSetSpeedupEnable(false);
		Result rc = romfsInit();
	#endif
}

void PHL_Deinit() {
	PHL_AudioClose();
	PHL_GraphicsExit();

	#ifdef _3DS
		romfsExit();
		sdmcExit();
	#endif
}