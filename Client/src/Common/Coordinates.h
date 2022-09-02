#pragma once

class Coordinates
{
public:
	void Add(const Coordinates& c)
	{
		x += c.x;
		y += c.y;
	}

	bool operator==(const Coordinates& c) const
	{
		return x == c.x && y == c.y;
	}

	int x;
	int y;
};