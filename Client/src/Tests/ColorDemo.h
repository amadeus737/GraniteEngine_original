#pragma once

#include <GraniteEngine.h>
#include <random>
#include "../Common/Map.h"

class ColorsTest : GraniteGame
{
public:
	ColorsTest()
		:
		_map(),
		_rng(std::random_device()())
	{ }

	void Update() override
	{
		std::uniform_int_distribution<int> colorDist(0, 255);
		for (int y = 0; y < _map.height(); y++)
		{
			for (int x = 0; x < _map.width(); x++)
			{
				Coordinates coords = { x, y };
				Granite::Color c(colorDist(_rng), colorDist(_rng), colorDist(_rng));
				_map.DrawCell(coords, c);
			}
		}
	}
private:
	Map _map;
	std::mt19937 _rng;
};