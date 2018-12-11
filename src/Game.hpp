#pragma once

#include "PHL.hpp"
#include "Resources.hpp"

class Game {
public:
	enum GameState { INIT, LOGO1, LOGO2, ATTRACT,
		TITLE, GAMEPLAY, ROOMSWITCHING, PAUSED, EXITING, GS_MAX};

	Game();
	~Game();
	static void Startup();
	static void Ending();
	static int GetMainLoop();
	static void Step(u64* frame_time);
	static void GameplayStep();
private:
	static bool IsExiting();
	static GameState gameState;
};