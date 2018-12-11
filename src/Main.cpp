#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <3ds.h>

#include "Game.hpp"

Game *game = nullptr;

#define FPS (60)
#define TICKS_PER_FRAME (268123480/FPS) // 268MHz / 60 frames per second  ; aka 268123480 ticks / 1 second

u64 last_time, now_time, frame_time;

int main(int argc, char **argv) {

	//last_time = svcGetSystemTick();
   
   
	game = new Game();
	while (game->GetMainLoop() == 1) {
		//now_time = svcGetSystemTick();
		//frame_time = now_time - last_time;

		game->Step();
		game->Draw();
		//last_time = now_time;
		//if (frame_time < TICKS_PER_FRAME)
		//	svcSleepThread(TICKS_PER_FRAME - frame_time);
	}
	game->Ending();
	return 0;
}