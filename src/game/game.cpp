#include"pch.h"
#include"game.h"

#include"raycast_system/raycast_system.h"

Game::Game(std::string map_folder) : m_running(true), m_viewport(64*2, 48), m_map(std::string(map_folder + "/map.png").c_str(), std::string(map_folder + "/texture_atlas.png").c_str(), std::string(map_folder + "/map_data.txt").c_str(), m_camera) {
	// Console setup
	m_console.SetTitle("Console Raycaster");
	m_console.SetFont(L"Consolas", 16);
	m_console.SetDisplayMode(CONSOLE_FULLSCREEN_MODE);
	// Clear console
	m_console.Clear();

	printf("*****************\n");
	printf("Console Raycaster\n");
	printf("*****************\n");
	printf("Loaded map: %s\n", map_folder.c_str());
	printf("Use 'WSAD' to move\n");
	printf("'Right-click' to rotate the camera\n");
	printf("press 'enter' to play!\n");

	getchar();
}

Game::~Game() {
	getchar();
}

void Game::Run() {
	// Main game loop
	while (m_running) {
		// Get user input
		m_camera.GetInput();
		// Render to viewport texture
		Raycaster::RenderFloor(m_viewport);
		Raycaster::Render(m_viewport, m_map, m_camera.GetPosition(), m_camera.GetAngle());
		Raycaster::RenderMipMap(m_viewport, m_map, m_camera.GetPosition());
		// Draw viewport texture to the console
		m_console.DrawTexture(m_viewport, "@&#HOio*/+-,. ");
		// Clear the viewport
		m_viewport.SetAll(0);
	}
}

void Game::Stop() {
	m_running = false;
}