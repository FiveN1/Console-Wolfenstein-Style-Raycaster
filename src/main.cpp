#include"pch.h"
#include"game/game.h"

int main() {
	// Load any map you like! 
	// For more info about how to make your own map check README.txt in the map folder
	Game game("assets/map");
	game.Run();
	return 0;
}