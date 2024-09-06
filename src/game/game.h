#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include"pch.h"

#include"console/console.h"
#include"core/texture/texture_2d.h"
#include"game_map/game_map.h"
#include"camera/camera.h"

/*
* Game Class
* - Simple Game class holding all of the functionalities of the game.
* - All other related game features are in the game folder
*/
class Game {
public:
	// Setup the game data
	Game(std::string map_folder);
	// Clear the game data
	~Game();
	// Run the game
	void Run();
	// Stop the game
	void Stop();
	// private game variables
private:
	// state describing if the game is running or not
	bool m_running;
	// Windows console on wich the image will be rendered to
	Console m_console;
	// Game viewport texture
	Texture2D m_viewport;
	// Camera class holding data about the position and angle of camera
	Camera m_camera;
	// Game map wich will be rendered trough camera onto the viewport
	GameMap m_map;
};
#endif GAME_CLASS_H
