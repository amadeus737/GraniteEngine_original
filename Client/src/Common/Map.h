#pragma once

#include <GraniteEngine.h>
#include <assert.h>
#include "Coordinates.h"

class Map
{
public:
	int width() const { return _width; }
	int height() const { return _height; }

	bool InMap(const Coordinates& c) const
	{
		return c.x >= 0 && c.x < _width&& c.y >= 0 && c.y < _height;
	}

	void DrawCell(const Coordinates& coords, Granite::Color c) const
	{
		assert(coords.x >= 0);
		assert(coords.x < _width);
		assert(coords.y >= 0);
		assert(coords.y < _height);

		const int offset_x = _xborder + _borderWidth + _borderPadding;
		const int offset_y = _yborder + _borderWidth + _borderPadding;
		_window.DrawRect(coords.x * _cellsize + offset_x + _cellPadding, coords.y * _cellsize + offset_y + _cellPadding, _cellsize - 2 * _cellPadding, _cellsize - 2 * _cellPadding, c);
	}

	void DrawBorder()
	{
		const int top = _yborder;
		const int left = _xborder;
		const int bottom = top + (_borderWidth + _borderPadding) * 2 + _height * _cellsize;
		const int right = left + (_borderWidth + _borderPadding) * 2 + _width * _cellsize;

		// top
		_window.DrawRect(left, top, right - left, _borderWidth, _borderColor);
		// left
		_window.DrawRect(left, top + _borderWidth, _borderWidth, bottom - top - 2 * _borderWidth, _borderColor);
		// right
		_window.DrawRect(right - _borderWidth, top + _borderWidth, _borderWidth, bottom - top - 2 * _borderWidth, _borderColor);
		// bottom
		_window.DrawRect(left, bottom - _borderWidth, right - left, _borderWidth, _borderColor);
	}

private:
	static constexpr int _cellsize = 20;
	static constexpr int _cellPadding = 1;
	static constexpr int _width = 25;
	static constexpr int _height = 25;
	static constexpr int _borderWidth = 4;
	static constexpr int _borderPadding = 2;
	static constexpr int _xborder = 70;
	static constexpr int _yborder = 50;
	static constexpr Granite::Color _borderColor = Granite::Color(0, 0, 255);
};