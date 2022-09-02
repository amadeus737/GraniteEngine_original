#pragma once

#include "Common/Block.h"

class MovementDemo : GraniteGame
{
public:
	MovementDemo() : _block(), _block_vx(0), _block_vy(0), _block_dvx(0), _block_dvy(0) { }

	void Update() override
	{
		UpdateLogic();
		UpdateGraphics();
	}

	void UpdateLogic()
	{
		bool receivedInput = false;

		if (_window.keyboard.KeyIsPressed('W') || _window.keyboard.KeyIsPressed(VK_UP))
		{
			_block_dvy--;
			receivedInput = true;
		}
		if (_window.keyboard.KeyIsPressed('A') || _window.keyboard.KeyIsPressed(VK_LEFT))
		{
			_block_dvx--;
			receivedInput = true;
		}
		if (_window.keyboard.KeyIsPressed('S') || _window.keyboard.KeyIsPressed(VK_DOWN))
		{
			_block_dvy++;
			receivedInput = true;
		}
		if (_window.keyboard.KeyIsPressed('D') || _window.keyboard.KeyIsPressed(VK_RIGHT))
		{
			_block_dvx++;
			receivedInput = true;
		}

		if (!receivedInput)
		{
			if (_block_dvx > 0) _block_dvx--;
			if (_block_dvx < 0) _block_dvx++;

			if (_block_dvy > 0) _block_dvy--;
			if (_block_dvy < 0) _block_dvy++;
		}

		if (_block.HitScreenLR())
		{
			_block_dvx = -_block_dvx;
		}

		if (_block.HitScreenUD())
		{
			_block_dvy = -_block_dvy;
		}

		int final_vx = _block_vx + _block_dvx;
		int final_vy = _block_vy + _block_dvy;

		_block.SetSpeed(final_vx, final_vy);
	}

	void UpdateGraphics()
	{
		_window.Clear(Granite::Color::MakeRGB(0x33, 0x33, 0x33));
		_window.DrawRect(_block.x(), _block.y(), _block.width(), _block.height(), _block.color());
	}

private:
	Block _block;
	int _block_vx;
	int _block_vy;
	int _block_dvx;
	int _block_dvy;
};