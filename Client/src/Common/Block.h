#pragma once

#include <GraniteEngine.h>

class Block
{
public:
	Block()
		:
		_x(100),
		_y(200),
		_vx(0),
		_vy(0),
		_width(50),
		_height(50),
		_color(Granite::Color::White()),
		_hitScreenLR(false),
		_hitScreenUD(false)
	{ }

	void SetSpeed(int x_amount, int y_amount)
	{
		// First, increment the velocity
		_vx = x_amount;
		_vy = y_amount;

		// Next, make sure that velocities do not exceed maximum allowable limits
		if (_vx > _maxVelocity) _vx = _maxVelocity;
		if (_vy > _maxVelocity) _vy = _maxVelocity;
		if (_vx < -_maxVelocity) _vx = -_maxVelocity;
		if (_vy < -_maxVelocity) _vy = -_maxVelocity;

		// Use the new velocity to increment the position
		_x += _vx;
		_y += _vy;

		// Make sure that we aren't exceeding the bounds of the screen
		_hitScreenLR = false;
		_hitScreenUD = false;
		if (_x + _width >= _window.ScreenWidth())
		{
			_vx = 0;
			_x = _window.ScreenWidth() - 1 - _width;
			_hitScreenLR = true;
		}

		if (_x < 0)
		{
			_vx = 0;
			_x = 0;
			_hitScreenLR = true;
		}

		if (_y < 0)
		{
			_vy = 0;
			_y = 0;
			_hitScreenUD = true;
		}

		if (_y + _height >= _window.ScreenHeight())
		{
			_vy = 0;
			_y = _window.ScreenHeight() - 1 - _height;
			_hitScreenUD = true;
		}
	}

	int x() { return _x; }
	int y() { return _y; }
	int width() { return _width; }
	int height() { return _height; }
	Granite::Color color() { return _color; }
	bool HitScreenLR() { return _hitScreenLR; }
	bool HitScreenUD() { return _hitScreenUD; }

private:
	int _x;
	int _y;
	int _vx;
	int _vy;
	int _width;
	int _height;
	Granite::Color _color;
	bool _hitScreenLR;
	bool _hitScreenUD;
	static constexpr int _minWidth = 10;
	static constexpr int _maxWidth = 200;
	static constexpr int _minHeight = 10;
	static constexpr int _maxHeight = 200;
	static constexpr int _maxVelocity = 10;
};