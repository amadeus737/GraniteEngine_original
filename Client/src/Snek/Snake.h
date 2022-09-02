#pragma once

#include <GraniteEngine.h>
#include "../Common/Map.h"
#include <assert.h>

class Snake
{
private:
	class Segment
	{
	public:
		const Coordinates& coords() const { return _coords; }

		void InitializeHead(const Coordinates& location)
		{
			_coords = location;
			_color = Snake::_headColor;
		}

		void InitializeBody(Granite::Color c)
		{
			_color = c;
		}

		void Follow(const Segment& next)
		{
			_coords = next._coords;
		}

		void Move(const Coordinates& delta)
		{
			assert(abs(delta.x) + abs(delta.y) == 1);
			_coords.Add(delta);
		}

		void Draw(Map& map) const
		{
			map.DrawCell(_coords, _color);
		}

	private:
		Coordinates _coords;
		Granite::Color _color;
	};

public:
	Snake(const Coordinates& startLocation)
	{
		InitializeSnake(startLocation);
	}

	void InitializeSnake(const Coordinates& startLocation)
	{
		constexpr int nBodyColors = 4;
		constexpr Granite::Color bodyColors[nBodyColors] =
		{
			{Granite::Color(10, 100, 32)},
			{Granite::Color(10, 130, 48)},
			{Granite::Color(18, 160, 48)},
			{Granite::Color(10, 130, 48)}
		};

		for (int i = 0; i < _maxSegments; i++)
			_segments[i].InitializeBody(bodyColors[i % nBodyColors]);

		_segments[0].InitializeHead(startLocation);
	}

	void Move(const Coordinates& delta)
	{
		for (int i = _nSegments - 1; i > 0; i--)
			_segments[i].Follow(_segments[i - 1]);

		_segments[0].Move(delta);
	}

	Coordinates GetNextHeadLocation(const Coordinates& delta) const
	{
		Coordinates next(_segments[0].coords());
		next.Add(delta);
		return next;
	}

	void Reset(const Coordinates& spawnLocation)
	{
		InitializeSnake(spawnLocation);
		_nSegments = 1;
	}

	void Grow()
	{
		if (_nSegments < _maxSegments)
		{
			++_nSegments;
		}
	}

	void Draw(Map& map) const
	{
		for (int i = 0; i < _nSegments; i++)
			_segments[i].Draw(map);
	}

	bool InTileExcludeEnd(const Coordinates& c) const
	{
		for (int i = 0; i < _nSegments - 1; i++)
		{
			if (_segments[i].coords() == c)
				return true;
		}

		return false;
	}

	bool InTile(const Coordinates& c) const
	{
		for (int i = 0; i < _nSegments; i++)
		{
			if (_segments[i].coords() == c)
				return true;
		}

		return false;
	}

private:
	static constexpr Granite::Color _headColor = Granite::Color(255, 255, 0);
	static constexpr Granite::Color _bodyColor = Granite::Color(0, 255, 0);
	static constexpr int _maxSegments = 100;
	int _nSegments = 1;
	Segment _segments[_maxSegments];
};