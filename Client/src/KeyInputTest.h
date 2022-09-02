#pragma once

#include <GraniteEngine.h>
#include <stdio.h>
class KeyInputTest : GraniteGame
{
public:
	KeyInputTest() : _blockX(200), _blockY(150), _blockColor(Granite::Color::Blue()) { }
	~KeyInputTest() { }
	
	void Start() override { }

	void Update() override 
	{ 
		UpdateLogic();
		UpdateGraphics();
	}

	void UpdateLogic()
	{
		_blockColor = Granite::Color::Yellow();

		if (Granite::_keystates[VK_LEFT] || Granite::_keystates['A']) _blockX -= 1;
		else if (Granite::_keystates[VK_RIGHT] || Granite::_keystates['D']) _blockX += 1;
		else if (Granite::_keystates[VK_UP] || Granite::_keystates['W']) _blockY -= 1;
		else if (Granite::_keystates[VK_DOWN] || Granite::_keystates['S']) _blockY += 1;
		else _blockColor = Granite::Color::Blue();
	}

	void UpdateGraphics()
	{
		_window.Clear(Granite::Color::MakeRGB(33, 33, 33));
		_window.DrawRect(_blockX, _blockY, 100, 100, _blockColor);
	}

private:
	int _blockX;
	int _blockY;
	Granite::Color _blockColor;
};