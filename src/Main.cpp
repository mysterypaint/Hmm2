#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <3ds.h>

#include "Game.hpp"

Game *game = nullptr;

int main(int argc, char **argv) {
	game = new Game();
	while (game->GetMainLoop() == 1) {
		game->Step();
	}
	game->Ending();
	return 0;
}