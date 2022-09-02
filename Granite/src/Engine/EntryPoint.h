#pragma once

#ifdef GRANITE_PLATFORM_WINDOWS

#include <Windows.h>
#include <string>
#include <stdio.h>
#include "Window.h"
#include "Game.h"

// An external function called CreateGame will generate the pointer to the application
extern Granite::Game* Granite::LaunchGame();

// Global variables
Granite::Game* _game; // Need game pointer globally available
Granite::WindowProperties _windowProperties;
Granite::Window _window;
bool _showFPS = false;

// Windows entry point...not actually used as entry point to project because the project is configured as
// a console project. Instead, the main() function below is the entry point, which calls this entry point.
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR cmdLine, int cmdShow)
{
	// First, get a pointer to the game
	_game = Granite::LaunchGame();

	// Create the game window, copying over the render window settings
	_window = Granite::Window(instance, _windowProperties.title(), _windowProperties.width(), _windowProperties.height(), _windowProperties.left(), _windowProperties.top(), cmdLine);

	// Run the game Start() function
	_game->Start();

	while (_window.IsRunning())
	{
		_window.BeginFrame();
		_window.ProcessMessage();
		_game->Update();
		_window.RenderFrame();
		_window.EndFrame();

		if (_showFPS) printf("FPS = %f\n", _window.GetFPS());
	}

	return 0;
}

// Console entry point
int main()
{
	return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}

#endif