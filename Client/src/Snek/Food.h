#pragma once

#include <GraniteEngine.h>
#include "../Common/Map.h"
#include "Snake.h"
#include <random>

class Food
{
public:
	Food(std::mt19937& rng, const Map& map, const Snake& snake)
	{
		Respawn(rng, map, snake);
	}

	void Respawn(std::mt19937& rng, const Map& map, const Snake& snake)
	{
		std::uniform_int_distribution<int> xdist(0, map.width() - 1);
		std::uniform_int_distribution<int> ydist(0, map.height() - 1);

		// Find a position not occupied by the snake
		Coordinates newCoords;
		do
		{
			newCoords.x = xdist(rng);
			newCoords.y = ydist(rng);
		} while (snake.InTile(newCoords));

		_coords = newCoords;
	}

	void Draw(Map& map) const
	{
		map.DrawCell(_coords, _color);
	}

	const Coordinates& coords() const { return _coords; }

private:
	static constexpr Granite::Color _color = Granite::Color(255, 0, 0);
	Coordinates _coords;
};