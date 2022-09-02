#pragma once

#include <GraniteEngine.h>
#include <stdio.h>
class KeyInputTest : GraniteGame
{
public:
	KeyInputTest() : 
		_blockX(200), _blockY(150), _blockWidth(100), _blockHeight(100), _blockIsMoving(false), _blockColor(Granite::Color::Blue()) { }
	~KeyInputTest() { }
	
	void Start() override { }

	void Update() override 
	{ 
		UpdateLogic();
		UpdateGraphics();
	}

	void UpdateLogic()
	{		
		_blockIsMoving = false;

		if (Granite::_keystates[VK_LEFT] || Granite::_keystates['A'])
		{
			_blockX -= 1; 
			_blockIsMoving = true;
		}		
		
		if (Granite::_keystates[VK_RIGHT] || Granite::_keystates['D'])
		{
			_blockX += 1; 
			_blockIsMoving = true;
		}

		if (Granite::_keystates[VK_UP] || Granite::_keystates['W'])
		{
			_blockY -= 1; 
			_blockIsMoving = true;
		}

		if (Granite::_keystates[VK_DOWN] || Granite::_keystates['S'])
		{
			_blockY += 1; 
			_blockIsMoving = true;
		}
		
		if (_blockIsMoving) 
			_blockColor = Granite::Color::Yellow();
		else
			_blockColor = Granite::Color::Blue();

		if (_blockX + _blockWidth >= _window.ScreenWidth())
			_blockX = _window.ScreenWidth() - 1 - _blockWidth;

		if (_blockX < 0)
			_blockX = 0;

		if (_blockY < 0)
			_blockY = 0;

		if (_blockY + _blockHeight >= _window.ScreenHeight())
			_blockY = _window.ScreenHeight() - 1 - _blockHeight;
	}

	void UpdateGraphics()
	{
		_window.Clear(Granite::Color::MakeRGB(33, 33, 33));
		_window.DrawRect(_blockX, _blockY, _blockWidth, _blockHeight, _blockColor);
	}

private:
	int _blockX;
	int _blockY;
	int _blockWidth;
	int _blockHeight;
	bool _blockIsMoving;
	Granite::Color _blockColor;
};