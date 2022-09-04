#pragma once

#include <GraniteEngine.h>
#include <random>
#include "../Common/Map.h"

#define ADD(a,b) { a.x + b.x, a.y + b.y }

enum class ShapeIdentifier
{
	NONE, I, T, O, S, Z, J, L
};

class Shape
{
public:

	int xmin() { return _xmin; }
	int xmax() { return _xmax; }
	int ymin() { return _ymin; }
	int ymax() { return _ymax; }

	Granite::Color& Color()
	{
		return _color;
	}

	Coordinates bottom_coord()
	{
		return _bottom_coord;
	}

	Coordinates& top_coord()
	{
		return _top_coord;
	}

	Coordinates& left_coord()
	{
		return _left_coord;
	}

	Coordinates& right_coord()
	{
		return _right_coord;
	}

	Coordinates& GetCoordinate(int i)
	{
		return _tileLocations[i];
	}

	ShapeIdentifier GetIdentifier()
	{
		return _shapeIdentifier;
	}
	
	void Rotate(int rotation)
	{
		switch (_shapeIdentifier)
		{
			case ShapeIdentifier::I:
				Spawn_I(rotation);
				break;

			case ShapeIdentifier::O:
				Spawn_O(rotation);
				break;

			case ShapeIdentifier::T:
				Spawn_T(rotation);
				break;

			case ShapeIdentifier::S:
				Spawn_S(rotation);
				break;

			case ShapeIdentifier::Z:
				Spawn_Z(rotation);
				break;

			case ShapeIdentifier::J:
				Spawn_J(rotation);
				break;

			case ShapeIdentifier::L:
				Spawn_L(rotation);
				break;
		}
	}

	void Spawn_Random(std::mt19937& rng)
	{
		// Seven unique tetriminoes (non-rotated)s
		std::uniform_int_distribution<int> shape_dist(0, 6);

		// Pick a shape randomly
		int random_shape_index = shape_dist(rng);

		switch (random_shape_index)
		{
			case 0:
				Spawn_I(0);
				_shapeIdentifier = ShapeIdentifier::I;
				break;

			case 1:
				Spawn_O(0);
				_shapeIdentifier = ShapeIdentifier::O;
				break;

			case 2:
				Spawn_T(0);
				_shapeIdentifier = ShapeIdentifier::T;
				break;

			case 3:
				Spawn_S(0);
				_shapeIdentifier = ShapeIdentifier::S;
				break;

			case 4:
				Spawn_Z(0);
				_shapeIdentifier = ShapeIdentifier::Z;
				break;

			case 5:
				Spawn_J(0);
				_shapeIdentifier = ShapeIdentifier::J;
				break;

			case 6:
				Spawn_L(0);
				_shapeIdentifier = ShapeIdentifier::L;
				break;
		}
	}

	void Spawn_I(int rotation)
	{
		_shapeIdentifier = ShapeIdentifier::I;
		_color = I_color;

		_xmin = 9999;
		_xmax = -9999;
		_ymin = 9999;
		_ymax = -9999;
		for (int i = 0; i < 4; i++)
		{
			switch (rotation)
			{
				case 0:
				case 180:
					_tileLocations[i] = _I_0[i];
					break;

				case 90:
				case 270:
					_tileLocations[i] = _I_90[i];				
					break;
			}

			if (_tileLocations[i].x < _xmin)
			{
				_xmin = _tileLocations[i].x;				
				_left_coord.x = _xmin;
				_left_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].x > _xmax)
			{
				_xmax = _tileLocations[i].x;
				_right_coord.x = _xmax;
				_right_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].y < _ymin)
			{
				_ymin = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymin;
			}

			if (_tileLocations[i].y > _ymax) 
			{
				_ymax = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymax;
			}
		}
	}

	void Spawn_O(int rotation)
	{
		_shapeIdentifier = ShapeIdentifier::O;
		_color = O_color;

		_xmin = 9999;
		_xmax = -9999;
		_ymin = 9999;
		_ymax = -9999;
		for (int i = 0; i < 4; i++)
		{
			_tileLocations[i] = _O_0[i];

			if (_tileLocations[i].x < _xmin)
			{
				_xmin = _tileLocations[i].x;
				_left_coord.x = _xmin;
				_left_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].x > _xmax)
			{
				_xmax = _tileLocations[i].x;
				_right_coord.x = _xmax;
				_right_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].y < _ymin)
			{
				_ymin = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymin;
			}

			if (_tileLocations[i].y > _ymax)
			{
				_ymax = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymax;
			}
		}
	}

	void Spawn_T(int rotation)
	{
		_shapeIdentifier = ShapeIdentifier::T;
		_color = T_color;

		_xmin = 9999;
		_xmax = -9999;
		_ymin = 9999;
		_ymax = -9999;
		for (int i = 0; i < 4; i++)
		{
			switch (rotation)
			{
				case 0:
					_tileLocations[i] = _T_0[i];
					break;
			
				case 90:
					_tileLocations[i] = _T_90[i];
					break;

				case 180:
					_tileLocations[i] = _T_180[i];
					break;

				case 270:
					_tileLocations[i] = _T_270[i];
					break;
			}

			if (_tileLocations[i].x < _xmin)
			{
				_xmin = _tileLocations[i].x;
				_left_coord.x = _xmin;
				_left_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].x > _xmax)
			{
				_xmax = _tileLocations[i].x;
				_right_coord.x = _xmax;
				_right_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].y < _ymin)
			{
				_ymin = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymin;
			}

			if (_tileLocations[i].y > _ymax)
			{
				_ymax = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymax;
			}
		}
	}

	void Spawn_S(int rotation)
	{
		_shapeIdentifier = ShapeIdentifier::S;
		_color = S_color;

		_xmin = 9999;
		_xmax = -9999;
		_ymin = 9999;
		_ymax = -9999;
		for (int i = 0; i < 4; i++)
		{
			switch (rotation)
			{
				case 0:
				case 180:
					_tileLocations[i] = _S_0[i];
					break;

				case 90:
				case 270:
					_tileLocations[i] = _S_90[i];
					break;
			}

			if (_tileLocations[i].x < _xmin)
			{
				_xmin = _tileLocations[i].x;
				_left_coord.x = _xmin;
				_left_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].x > _xmax)
			{
				_xmax = _tileLocations[i].x;
				_right_coord.x = _xmax;
				_right_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].y < _ymin)
			{
				_ymin = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymin;
			}

			if (_tileLocations[i].y > _ymax)
			{
				_ymax = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymax;
			}
		}
	}

	void Spawn_Z(int rotation)
	{
		_shapeIdentifier = ShapeIdentifier::Z;
		_color = Z_color;

		_xmin = 9999;
		_xmax = -9999;
		_ymin = 9999;
		_ymax = -9999;
		for (int i = 0; i < 4; i++)
		{
			switch (rotation)
			{
			case 0:
			case 180:
				_tileLocations[i] = _Z_0[i];
				break;

			case 90:
			case 270:
				_tileLocations[i] = _Z_90[i];
				break;
			}

			if (_tileLocations[i].x < _xmin)
			{
				_xmin = _tileLocations[i].x;
				_left_coord.x = _xmin;
				_left_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].x > _xmax)
			{
				_xmax = _tileLocations[i].x;
				_right_coord.x = _xmax;
				_right_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].y < _ymin)
			{
				_ymin = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymin;
			}

			if (_tileLocations[i].y > _ymax)
			{
				_ymax = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymax;
			}
		}
	}

	void Spawn_J(int rotation)
	{
		_shapeIdentifier = ShapeIdentifier::J;
		_color = J_color;

		_xmin = 9999;
		_xmax = -9999;
		_ymin = 9999;
		_ymax = -9999;
		for (int i = 0; i < 4; i++)
		{
			switch (rotation)
			{
			case 0:
				_tileLocations[i] = _J_0[i];
				break;

			case 90:
				_tileLocations[i] = _J_90[i];
				break;

			case 180:
				_tileLocations[i] = _J_180[i];
				break;

			case 270:
				_tileLocations[i] = _J_270[i];
				break;
			}

			if (_tileLocations[i].x < _xmin)
			{
				_xmin = _tileLocations[i].x;
				_left_coord.x = _xmin;
				_left_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].x > _xmax)
			{
				_xmax = _tileLocations[i].x;
				_right_coord.x = _xmax;
				_right_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].y < _ymin)
			{
				_ymin = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymin;
			}

			if (_tileLocations[i].y > _ymax)
			{
				_ymax = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymax;
			}
		}
	}

	void Spawn_L(int rotation)
	{
		_shapeIdentifier = ShapeIdentifier::L;
		_color = L_color;

		_xmin = 9999;
		_xmax = -9999;
		_ymin = 9999;
		_ymax = -9999;
		for (int i = 0; i < 4; i++)
		{
			switch (rotation)
			{
			case 0:
				_tileLocations[i] = _L_0[i];
				break;

			case 90:
				_tileLocations[i] = _L_90[i];
				break;

			case 180:
				_tileLocations[i] = _L_180[i];
				break;

			case 270:
				_tileLocations[i] = _L_270[i];
				break;
			}

			if (_tileLocations[i].x < _xmin)
			{
				_xmin = _tileLocations[i].x;
				_left_coord.x = _xmin;
				_left_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].x > _xmax)
			{
				_xmax = _tileLocations[i].x;
				_right_coord.x = _xmax;
				_right_coord.y = _tileLocations[i].y;
			}

			if (_tileLocations[i].y < _ymin)
			{
				_ymin = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymin;
			}

			if (_tileLocations[i].y > _ymax)
			{
				_ymax = _tileLocations[i].y;
				_top_coord.x = _tileLocations[i].x;
				_top_coord.y = _ymax;
			}
		}
	}

	void Draw(Map& map, const Coordinates& coords, Granite::Color colorOverride = Granite::Color::Black())
	{
		for (int i = 0; i < 4; i++)
		{
			map.DrawCell(ADD(coords, _tileLocations[i]), colorOverride.IsEqual(Granite::Color::Black()) ? _color : colorOverride);
		}
	}

	void DrawGhost(Map& map, int thickness, const Coordinates& coords)
	{
		for (int i = 0; i < 4; i++)
		{
			map.DrawCellBorder(ADD(coords, _tileLocations[i]), thickness, _color);
		}
	}

	static constexpr Granite::Color I_color = Granite::Color(50u, 255u, 255u);
	static constexpr Granite::Color O_color = Granite::Color(255u, 255u, 50u);
	static constexpr Granite::Color T_color = Granite::Color(255u, 50u, 255u);
	static constexpr Granite::Color S_color = Granite::Color(50u, 255u, 50u);
	static constexpr Granite::Color Z_color = Granite::Color(255u, 50u, 50u);
	static constexpr Granite::Color J_color = Granite::Color(50u, 100u, 255u);
	static constexpr Granite::Color L_color = Granite::Color(255u, 200u, 50u);

private:
	static constexpr Coordinates _I_0[4]   = { {0,0}, {1,0}, {2,0}, {3,0} };
	static constexpr Coordinates _I_90[4]  = { {0,0}, {0,1}, {0,2}, {0,3} };
	static constexpr Coordinates _O_0[4]   = { {1,0}, {2,0}, {1,1}, {2,1} };
	static constexpr Coordinates _T_0[4]   = { {0,0}, {1,0}, {2,0}, {1,1} };
	static constexpr Coordinates _T_90[4]  = { {0,1}, {1,0}, {1,1}, {1,2} };
	static constexpr Coordinates _T_180[4] = { {1,0}, {0,1}, {1,1}, {2,1} };
	static constexpr Coordinates _T_270[4] = { {1,0}, {1,1}, {1,2}, {2,1} };
	static constexpr Coordinates _S_0[4]   = { {1,0}, {2,0}, {0,1}, {1,1} };
	static constexpr Coordinates _S_90[4]  = { {1,0}, {1,1}, {2,1}, {2,2} };
	static constexpr Coordinates _Z_0[4]   = { {0,0}, {1,0}, {1,1}, {2,1} };
	static constexpr Coordinates _Z_90[4]  = { {1,1}, {1,2}, {2,0}, {2,1} };
	static constexpr Coordinates _J_0[4]   = { {2,0}, {2,1}, {1,2}, {2,2} };
	static constexpr Coordinates _J_90[4]  = { {0,0}, {0,1}, {1,1}, {2,1} };
	static constexpr Coordinates _J_180[4] = { {1,0}, {2,0}, {1,1}, {1,2} };
	static constexpr Coordinates _J_270[4] = { {0,0}, {1,0}, {2,0}, {2,1} };
	static constexpr Coordinates _L_0[4]   = { {1,0}, {1,1}, {1,2}, {2,2} };
	static constexpr Coordinates _L_90[4]  = { {2,0}, {0,1}, {1,1}, {2,1} };
	static constexpr Coordinates _L_180[4] = { {1,0}, {2,0}, {2,1}, {2,2} };
	static constexpr Coordinates _L_270[4] = { {0,0}, {1,0}, {2,0}, {0,1} };
	Granite::Color _color;
	Coordinates _tileLocations[4];
	int _xmin; 
	int _xmax;
	int _ymin;
	int _ymax;
	Coordinates _bottom_coord;
	Coordinates _top_coord;
	Coordinates _left_coord;
	Coordinates _right_coord;
	ShapeIdentifier _shapeIdentifier = ShapeIdentifier::NONE;
};