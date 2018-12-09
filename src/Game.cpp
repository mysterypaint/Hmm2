#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "Game.hpp"
#include "AudioEngine.hpp"

////// Global vars //////

AudioEngine *audioEngine = nullptr;

//PHL_Music currBGM[1];					// Keeps the current song stream in the memory
//PHL_Sound sounds[SE_MAX];			// Creates an array that stores every sound in the game

Game::Game(void) {
	Startup();
}

Game::~Game(void) {
}

void Game::Startup(void) {
	if (gameState != INIT)
		return;

	//Setup
	PHL_Init();
	audioEngine = new AudioEngine();
	//PHL_FreeMusic(currBGM[0]);
	//currBGM[0] = PHL_LoadMusic("m04", loopBGM);
	//currBGM[0] = PHL_LoadMusic(NULL, loopBGM);
	/*
		sounds[SE00] = PHL_LoadSound("se00");
		sounds[SE01] = PHL_LoadSound("se01");
		sounds[SE02] = PHL_LoadSound("se02");
		sounds[SE03] = PHL_LoadSound("se03");
		sounds[SE04] = PHL_LoadSound("se04");
	*/
	//PHL_PlayMusic(currBGM[0]);
}

void Game::Ending(void) {
	delete audioEngine;
	PHL_Deinit();
}

int Game::GetMainLoop() {
	return PHL_MainLoop();
}

bool Game::IsExiting() {
	if(gameState == Game::EXITING) 
		return true;
	else 
		return false;
}

void Game::Step() {

	hidScanInput();
	u32 kDown = hidKeysHeld();
		
	if (kDown & KEY_START)
		PHL_GameQuit();


	switch(gameState) {
		case Game::INIT:
			gameState = Game::GAMEPLAY;
			break;
		case Game::GAMEPLAY:
			GameplayStep();
			break;
		default:
			break;
	}

	audioEngine->Step();

	if (GetMainLoop() == 0) {
		// The game is ending; Free all game resources


		/*
		//Free sounds
		for (int i = 0; i < SE_MAX; i++) {
			PHL_FreeSound(sounds[i]);
		}
		
		//Free Music
		PHL_FreeMusic(currBGM[0]);

		*/
	}
}

void Game::GameplayStep() {
}

Game::GameState Game::gameState = INIT;