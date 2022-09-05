#pragma once

#include <GraniteEngine.h>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include "FrameTimer.h"

// An external function called CreateGame will generate the pointer to the application
extern Granite::Game* Granite::LaunchGame();

// Global variables
Granite::Game* _game; // Need game pointer globally available
Granite::WindowProperties _windowProperties;
Granite::Window _window;

// FPS and frame-timing options
int _framesBeforeFPSupdate = 200;
bool _showFPS = false;
float _FPS = 0.0f;
float _deltaTime = 0.0f;
float _time = 0.0f;

// Windows entry point...not actually used as entry point to project because the project is configured as
// a console project. Instead, the main() function below is the entry point, which calls this entry point.
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR cmdLine, int cmdShow)
{
	// Setup a frame timer
	Granite::FrameTimer frameTimer;
	int _fpsFrameCount = 0.0f;
	float _fpsTimeCount = 0.0f;

	// First, get a pointer to the game
	_game = Granite::LaunchGame();
	_window = Granite::Window(GetModuleHandle(NULL), _windowProperties.title(), _windowProperties.width(), _windowProperties.height(), _windowProperties.left(), _windowProperties.top(), GetCommandLineW());

	// Run the game Start() function
	_game->Start();

	while (_window.IsRunning())
	{
		_window.BeginFrame();
		_window.ProcessMessage();
		_deltaTime = frameTimer.Mark();
		_time += _deltaTime;

		if (_fpsFrameCount < _framesBeforeFPSupdate)
		{
			_fpsTimeCount += _deltaTime;
			_fpsFrameCount++;
		}
		else
		{
			_FPS = _fpsFrameCount / _fpsTimeCount;

			_fpsFrameCount = 0;
			_fpsTimeCount = 0;

			if (_showFPS)
			{
				system("CLS");
				printf("FPS = %f\n", _FPS);
			}
		}
		_game->Update();
		_window.RenderFrame();
		_window.EndFrame();
	}

	return 0;
}

// Console entry point
int main()
{
	return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}
