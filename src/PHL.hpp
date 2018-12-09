// PHL (Portable Homebrew Library), by the Anon who ported Hydra Castle Labyrinth to 3DS
// I re-structured it a bit and changed it from C to C++, but for the most part it's the same code.
// This implementation of PHL includes the entire /3ds/ folder... I don't have plans to back-port to Wii and PSP, but I might do Switch...
// - mysterypaint
#pragma once

typedef struct {
	int x, y, w, h;
} PHL_Rect;

#ifdef _3DS
	#include "3ds/System.hpp"
	#include "3ds/Graphics.hpp"
	#include "3ds/Input.hpp"
	#include "3ds/Audio.hpp"
#endif

/*
#ifdef __SWITCH__
	#include "switch/System.hpp"
	#include "switch/Graphics.hpp"
	#include "switch/Input.hpp"
	#include "switch/Audio.hpp"
#endif

#ifdef _WIIU
	#include "wiiu/System.hpp"
	#include "wiiu/Graphics.hpp"
	#include "wiiu/Input.hpp"
	#include "wiiu/Audio.hpp"
#endif

#ifdef _WII
	#include "wii/System.hpp"
	#include "wii/Graphics.hpp"
	#include "wii/Input.hpp"
	#include "wii/Audio.hpp"
#endif

#ifdef _PSP
	#include "psp/System.hpp"
	#include "psp/Graphics.hpp"
	#include "psp/Input.hpp"
	#include "psp/Audio.hpp"
#endif
*/

void PHL_Init();
void PHL_Deinit();