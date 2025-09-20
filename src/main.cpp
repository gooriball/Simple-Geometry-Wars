#include "Game.h"

int main(void)
{
	Game game{"config.json"};

	game.run();
	
	return (0);
}