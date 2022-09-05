#pragma once

#include <GraniteEngine.h>
#include <assert.h>
#include <vector>
#include "Coordinates.h"

class Map
{
public:
	int width() const { return _width; }
	int height() const { return _height; } 
	int cellsize() const { return _cellsize; }
	int offsetx() const { return _xoffset; }
	int offsety() const { return _yoffset; }

	void SetWidth(int w) { _width = w; }
	void SetHeight(int h) { _height = h; }
	void SetOffset_x(int x) { _xoffset = x; }
	void SetOffset_y(int y) { _yoffset = y; }
	void SetBorderProperties(int borderWidth, int borderPadding) { _borderWidth = borderWidth; _borderPadding = borderPadding; }
	void SetCellProperties(int cellWidth, int cellPadding) { _cellsize = cellWidth; _cellPadding = cellPadding; }

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

		const int offset_x = _xoffset + _borderWidth + _borderPadding;
		const int offset_y = _yoffset + _borderWidth + _borderPadding;

		_window.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
			coords.y * _cellsize + offset_y + _cellPadding,
			_cellsize - 2 * _cellPadding,
			_cellsize - 2 * _cellPadding,
			c);
	}

	void DrawCellBorder(const Coordinates& coords, int thickness, Granite::Color c) const
	{
		assert(coords.x >= 0);
		assert(coords.x < _width);
		assert(coords.y >= 0);
		assert(coords.y < _height);

		const int offset_x = _xoffset + _borderWidth + _borderPadding;
		const int offset_y = _yoffset + _borderWidth + _borderPadding;

		// Draw left edge of tile
		_window.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
			coords.y * _cellsize + offset_y + _cellPadding,
			thickness,
			_cellsize,
			c);

		// Draw top edge of tile
		_window.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
			coords.y * _cellsize + offset_y + _cellPadding,
			_cellsize,
			thickness,
			c);

		// Draw right edge of tile
		_window.DrawRect(coords.x * _cellsize + offset_x + _cellPadding + _cellsize,
			coords.y * _cellsize + offset_y + _cellPadding,
			thickness,
			_cellsize,
			c);

		// Draw top edge of tile
		_window.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
			coords.y * _cellsize + offset_y + _cellPadding + _cellsize,
			_cellsize + thickness,
			thickness,
			c);
	}

	void DrawBorder()
	{
		const int top = _yoffset;
		const int left = _xoffset;
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
	int _cellsize = 20;
	int _cellPadding = 1;
	int _width = 25;
	int _height = 25;
	int _borderWidth = 4;
	int _borderPadding = 2;
	int _xoffset = 70;
	int _yoffset = 50;
	static constexpr Granite::Color _borderColor = Granite::Color(0, 0, 255);
};