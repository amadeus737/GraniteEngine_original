#pragma once

#include <GraniteEngine.h>
#include <random>
#include "Common/Map.h"
#include "Snake.h"
#include "Food.h"

class Snek : GraniteGame
{
public:
	Snek()
		:
		_map(),
		_rng(std::random_device()()),
		_snake({ 2,2 }),
		_food(_rng, _map, _snake)
	{ }

	void Update() override
	{
		UpdateLogic();
		UpdateGraphics();
	}

	void UpdateLogic()
	{
		if (!_gameStarted)
		{
			_gameStarted = _window.keyboard.KeyIsPressed(VK_RETURN);
		}

		if (_gameStarted && !_gameOver)
		{
			if (_window.keyboard.KeyIsPressed(VK_UP) || _window.keyboard.KeyIsPressed('W')) _delta = { 0, -1 };
			if (_window.keyboard.KeyIsPressed(VK_LEFT) || _window.keyboard.KeyIsPressed('A')) _delta = { -1, 0 };
			if (_window.keyboard.KeyIsPressed(VK_DOWN) || _window.keyboard.KeyIsPressed('S')) _delta = { 0, 1 };
			if (_window.keyboard.KeyIsPressed(VK_RIGHT) || _window.keyboard.KeyIsPressed('D')) _delta = { 1, 0 };

			++_moveCounter;

			if (_moveCounter >= _movePeriod)
			{
				_moveCounter = 0;
				const Coordinates next = _snake.GetNextHeadLocation(_delta);
				_gameOver = !_map.InMap(next) || _snake.InTileExcludeEnd(next);

				if (!_gameOver)
				{
					const bool eating = next == _food.coords();
					if (eating || _window.keyboard.KeyIsPressed(VK_CONTROL))
					{
						_snake.Grow();
					}

					_snake.Move(_delta);

					if (eating)
					{
						_food.Respawn(_rng, _map, _snake);
					}
				}
			}
		}
	}

	void UpdateGraphics()
	{
		if (!_gameOver)
		{
			if (_gameStarted)
			{
				_window.Clear(Granite::Color(0x33, 0x33, 0x33));
				_snake.Draw(_map);
				_food.Draw(_map);
				_map.DrawBorder();
			}
			else
			{
				_window.Clear(Granite::Color(0, 255, 0));
			}
		}
		else
		{
			_window.Clear(Granite::Color::Red());
		}
	}

private:
	Map _map;
	std::mt19937 _rng;
	Snake _snake;
	Food _food;
	Coordinates _delta = { 1,0 };
	static constexpr int _movePeriod = 50; // number of frames needed before move update
	int _moveCounter = 0;
	bool _gameStarted = false;
	bool _gameOver = false;
};