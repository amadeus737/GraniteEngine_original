#pragma once

#include <GraniteEngine.h>
#include <random>
#include "Shape.h"
#include <stdio.h>

class Tetris : GraniteGame
{
public:
	// Constructor
	Tetris() 
		: 
		_map(), 
		_rng(std::random_device()()), 
		_currentShape(), 
		_shapePosition(), 
		_ghostShapePosition(), 
		_cellContents(_map.width()* _map.height(), Granite::Color::Black()), 
		_shouldDeleteRow(_map.height(), false) 
	{ }

	#pragma region FLOW CONTROL

	void Start()
	{
		// Setup 16x25 play area with cells that are 20 pixels wide and padded by 1 pixel. Don't apply a default border and
		// offset the map within the window by 70 pixels left and 50 pixels down
		_map.SetWidth(16);
		_map.SetHeight(25);
		_map.SetBorderProperties(0, 0);
		_map.SetCellProperties(20, 1);
		_map.SetOffset_x(70);
		_map.SetOffset_y(50);

		// Start the game by spawing a new shape
		SpawnNewShape();
	}

	void Update()
	{
		UpdateLogic();
		UpdateGraphics();
	}

	void ResetGame()
	{		
		// Reset count variables
		_rotationCount = 0.0f;
		_moveCount = 0.0f;
		_rowDeleteCount = 0.0f;
		_dropCount = 0.0f;

		// Clear all map tiles of any contents
		_cellContents.clear();

		// Re-initialize vetor arrays to default values
		_cellContents = std::vector<Granite::Color>(_map.width() * _map.height(), Granite::Color::Black());
		_shouldDeleteRow = std::vector<bool>(_map.height(), false);

		// Make sure gameOver boolean isn't set!
		_gameOver = false;
		_gameOverDrawCounts = 0;

		// Spawn a new shape
		SpawnNewShape();
	}
	
	#pragma endregion

	#pragma region LOGIC

	void SpawnNewShape()
	{
		// Pick a random shape and spawn it at the top of the screen
		_currentShape.Spawn_Random(_rng);
		//_currentShape.Spawn_I(0);
		_shapePosition = { 6, 1 };
		_ghostShapePosition = { 6, 1 };
		_dropWait = _defaultDropWait;
		_currentRotation = 0.0f;

		_updateGhost = true;
	}

	void UpdateLogic()
	{
		// If game is over, allow user to reset by pressing Return. Otherwise, skip updating game
		// logic when game is over
		if (_gameOver)
		{
			if (_window.keyboard.KeyIsPressed(VK_RETURN))
				ResetGame();
			else
				return;
		}

		// If enough frames have passed to allow determination of game pause update...
		if (_pauseCount >= _pauseWait)
		{
			// Pause is toggled with Escape key
			if (_window.keyboard.KeyIsPressed(VK_ESCAPE))
				_paused = !_paused;

			// Reset frame timer for pause update
			_pauseCount = 0.0f;
		}
		else
		{
			// Increment frame timer for pause update
			_pauseCount += _deltaTime;
		}

		// If paused, skip updating game logic
		if (_paused)
			return;

		// For the current shape, process rotation, lateral, and vertical movement. Also
		// check whether rows are filled and need to be deleted.
		ProcessRotation();
		ProcessLateralMovement();
		ProcessVerticalMovement();
		MarkRowsForDeletion();

		// Reset delta movement in case there are no keypresses next frame
		_delta = { 0, 0 };
	}

	void ProcessRotation()
	{
		// If up arrow or W is pressed, rotate shape 90 degrees
		if (_window.keyboard.KeyIsPressed(VK_UP) || _window.keyboard.KeyIsPressed('W'))
			_rotationIncrement = 90.0f;

		// If enough frames have passed to allow for rotation update...
		if (_rotationCount >= _rotationWait)
		{
			// Increase current rotation, ensuring not to exceed 270 degrees
			if (_currentRotation == 270 && _rotationIncrement == 90)
				_currentRotation = 0;
			else
				_currentRotation += _rotationIncrement;

			// Apply the rotation to the current shape
			_currentShape.Rotate(_currentRotation);

			// Rest the frame timer for the next rotation update
			_rotationCount = 0.0f;
		}
		else
		{
			// Increment rotation frame count
			_rotationCount += _deltaTime;
		}

		// Reset rotation increment in case up or W is not pressed next frame
		_rotationIncrement = 0;
	}

	void ProcessLateralMovement()
	{
		// Update delta movement based on left or right key input
		if (_window.keyboard.KeyIsPressed(VK_LEFT) || _window.keyboard.KeyIsPressed('A')) _delta = { -1, 0 };
		if (_window.keyboard.KeyIsPressed(VK_RIGHT) || _window.keyboard.KeyIsPressed('D')) _delta = { 1, 0 };

		// If enough frames have passed to allow for lateral movement update...
		if (_moveCount >= _moveWait)
		{
			// Get the minimum tile x position in the current shape, add the shapePosition and the delta offset that the shape intends to move
			// to...make sure it is >= 1 (i.e., to the right of the left play area border). 
			// 
			// Also make sure that the maximum tile x position in the current shape added to the shapePosition offset and the intended delta position
			// is less than mapwidth - 2 (i.e., is to the left of the right play area border). It's "-2" because the tiles along a row go from 0 to 
			// _map.width() - 1...but the tile at _map.width() - 1 is a border tile so we need _map.width() - 2!
			//
			// Finally, also make sure that the position we are moving to actually contains empty tiles...that's what the CheckDirection() function does.
			//
			// If all three of these conditions are satisfied, we can go ahead and apply the intended delta movement to the shape.
			if (_shapePosition.x + _currentShape.xmin() + _delta.x >= 1 && _shapePosition.x + _currentShape.xmax() + _delta.x <= _map.width() - 2 && CheckDirection(_delta))
			{
				// Add the lateral offset to both the shape and the ghosted shape that indicates where it will land
				_shapePosition.Add(_delta);
				_ghostShapePosition.Add(_delta);
			}

			// Reset frame counter for the next lateral movement update
			_moveCount = 0.0f;
		}
		else
		{
			// Increment lateral movement frame counter
			_moveCount += _deltaTime;
		}
	}

	void ProcessVerticalMovement()
	{
		int down = _map.height() - 2;
		for (int i = 0; i < 4; i++)
		{
			Coordinates c = _currentShape.GetCoordinate(i);
			c.x += _shapePosition.x;
			c.y += GetLowestEmptyY(c.x) - _currentShape.ymax();

			if (c.y < down) down = c.y;
		}

		_ghostShapePosition.y = down;

		// If the down arrow or S key is pressed, speed up the shape drop time by a factor of 10
		if (_window.keyboard.KeyIsPressed(VK_DOWN) || _window.keyboard.KeyIsPressed('S')) _dropWait = _defaultDropWait / 10;

		// If enough frames have passed to allow for vertical movement update...
		if (_dropCount >= _dropWait)
		{
			// If the vertical position of the shape hasn't yet hit the bottom border and the tiles below
			// the shape are empty...
			if (_shapePosition.y + _currentShape.ymax() + 1 <= _map.height() - 2 && CheckDirection({ 0,1 }))
			{
				// Drop the shape down by one tile
				_shapePosition.Add({ 0,1 });
				_updateGhost = true;
			}
			else
			{
				// If we have either reached the bottom of the play area or the tiles below the shape are 
				// not empty, we need to lock this shape to the play map area and spawn a new shape
				LockShape();

				if (!_gameOver)
					SpawnNewShape();

				_updateGhost = false;
			}

			// Reset frame counter for the next vertical movement update
			_dropCount = 0.0f;
		}
		else
		{
			// Increment vertical movement frame counter
			_dropCount += _deltaTime;
		}		
	}

	int GetLowestEmptyY(int x)
	{
		for (int y = _map.height() - 2; y >= 1; y--)
		{
			if (IsCellEmpty({ x, y }))
				return y;
		}

		return 1;
	}

	bool CheckDirection(const Coordinates& direction)
	{
		bool allowMove = true;
		for (int i = 0; i < 4; i++)
		{
			Coordinates nextCoords = ADD(_currentShape.GetCoordinate(i), direction);
			nextCoords = ADD(nextCoords, _shapePosition);
			allowMove = allowMove && IsCellEmpty(nextCoords);
		}

		return allowMove;
	}

	bool CheckGhostDirection(const Coordinates& direction)
	{
		bool allowMove = true;
		for (int i = 0; i < 4; i++)
		{
			Coordinates nextCoords;
			nextCoords.x = _shapePosition.x + direction.x;
			nextCoords.y = direction.y;

			nextCoords = ADD(nextCoords, _ghostShapePosition);
			allowMove = allowMove && IsCellEmpty(nextCoords);
		}

		return allowMove;
	}

	void LockShape()
	{
		for (int i = 0; i < 4; i++)
		{
			Coordinates lockCoordinate = ADD(_shapePosition, _currentShape.GetCoordinate(i));
			SetCellContents(lockCoordinate, _currentShape.Color());

			if (lockCoordinate.y == 1)
			{
				_gameOver = true;
			}
		}
	}

	void SetCellContents(const Coordinates& location, Granite::Color color)
	{
		_cellContents[location.y * _map.width() + location.x] = color;
	}

	Granite::Color GetCellContents(const Coordinates& location) const
	{
		int index = location.y * _map.width() + location.x;
		if (index < _cellContents.size())
			return _cellContents[index];
		else
			return Granite::Color::Black();
	}
	
	bool IsCellEmpty(const Coordinates& location)
	{
		return GetCellContents(location).IsEqual(Granite::Color::Black());
	}

	void MarkRowsForDeletion()
	{
		for (int j = _map.height() - 2; j > 0; j--)
		{
			_shouldDeleteRow[j] = false;

			bool deleteRow = true;
			for (int i = 1; i < _map.width() - 1; i++)
			{
				deleteRow = deleteRow && !IsCellEmpty({ i,j });
			}

			if (deleteRow)
			{
				for (int i = 1; i < _map.width() - 1; i++)
				{
					_cellContents[j * _map.width() + i] = _deletionRowColor;
					_shouldDeleteRow[j] = true;
				}
			}
		}

		if (_rowDeleteCount > _rowDeleteWait)
		{
			DeleteRows();
			_rowDeleteCount = 0.0f;
		}
		else
		{
			_rowDeleteCount += _deltaTime;
		}
	}

	void DeleteRows()
	{
		for (int j = _map.height() - 2; j > 0; j--)
		{
			if (_shouldDeleteRow[j])
			{
				_shouldDeleteRow[j] = false;				
				for (int jj = j; jj > 1; jj--)
				{
					for (int i = 1; i < _map.width() - 1; i++)
						_cellContents[(jj) * _map.width() + i] = _cellContents[(jj - 1)*_map.width() + i];
				}				
			}
		}
	}

	#pragma endregion

	#pragma region GRAPHICS

	void UpdateGraphics()
	{
		if (_paused)
			return;

		if (_gameOver && _gameOverDrawCounts > 0)
		{
			return;
		}

		// Clear the screen each frame
		_window.Clear(Granite::Color::Granite());

		// Draw locked shapes
		DrawLockedShapes();

		// Draw the shape
		_currentShape.Draw(_map, _shapePosition, _gameOver ? Granite::Color::Red() : Granite::Color::Black());

		// Draw the shape ghost
		if (_updateGhost)
		{
			//_currentShape.DrawGhost(_map, 1, _ghostShapePosition);

			int xmin = (_shapePosition.x + _currentShape.xmin()) * _map.cellsize() + _map.offsetx();
			int xmax = (_shapePosition.x + _currentShape.xmax() + 1) * _map.cellsize() + _map.offsetx();
			int ymin = (_shapePosition.y + _currentShape.ymax() + 1) * _map.cellsize() + _map.offsety();
			int thickness = 1;

			int height = (_map.height() - 2 - (_shapePosition.y + _currentShape.ymax() + 1)) * _map.cellsize();
			Granite::Color color = _currentShape.Color();

			int dashLength = 3;
			int gapLength = 10;
			bool draw = true;
			int totalLength = 0;
			while (totalLength <= height)
			{
				if (draw)
				{
					_window.DrawRect(xmin, ymin + totalLength, thickness, dashLength, color);
					_window.DrawRect(xmax, ymin + totalLength, thickness, dashLength, color);
					totalLength += dashLength;
				}
				else
				{
					totalLength += gapLength;
				}
								
				draw = !draw;
			}
		}

		// Draw border
		DrawPlayAreaBorder();

		if (_rowDeleteCount <= _rowDeleteWait)
		{
			float lerp = 0;
			
			if (lerp <= 0.05f) 
				lerp = (_rowDeleteLerpSpeed * 255.0f / (_rowDeleteWait) * _rowDeleteCount);

			if (lerp >= 254.95f)
				lerp = 255 - (_rowDeleteLerpSpeed * 255.0f / (_rowDeleteWait) * _rowDeleteCount);

			_deletionRowColor = Granite::Color(255u, lerp, lerp);
		}

		if (_gameOver)
			_gameOverDrawCounts++;
	}

	void DrawPlayAreaBorder()
	{
		for (int i = 0; i < _map.width(); i++)
		{
			_map.DrawCell({ i, 0 }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
			_map.DrawCell({ i, _map.height() - 1 }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
		}

		for (int j = 0; j < _map.height(); j++)
		{
			_map.DrawCell({ 0, j }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
			_map.DrawCell({ _map.width() - 1, j }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
		}
	}

	void DrawLockedShapes()
	{
		for (int i = 0; i < _map.width(); i++)
		{
			for (int j = 0; j < _map.height(); j++)
			{
				if (IsCellEmpty({i,j}))
					continue;

				_map.DrawCell({ i,j }, _gameOver ? Granite::Color::Red() : _cellContents[j * _map.width() + i]);
			}
		}
	}

	#pragma endregion

private:
	bool _gameOver = false;
	bool _wantsToPause = false;
	bool _paused = false;
	bool _updateGhost = true;
	Map _map;
	std::mt19937 _rng;
	Shape _currentShape;	
	Coordinates _shapePosition;
	Coordinates _ghostShapePosition;
	std::vector<Granite::Color> _cellContents;
	std::vector<bool> _shouldDeleteRow;
	Coordinates _delta = { 0,0 };
	Granite::Color _deletionRowColor = Granite::Color::White();	
	int _currentRotation = 0;
	int _rotationIncrement = 0;
	int _gameOverDrawCounts = 0;
	float _rotationCount = 0.0f;
	float _moveCount = 0.0f;
	float _rowDeleteCount = 0.0f;
	float _colorChangeCount = 0.0f;
	float _dropCount = 0.0f;
	float _pauseCount = 0.0f;
	static constexpr float _rotationWait = 0.1f;        // 20
	static constexpr float _moveWait = 0.1f;	         // 20
	float _pauseWait = 0.1f;                            // 25
	static constexpr float _rowDeleteWait = 1.0f;       // 200
	static constexpr float _rowDeleteLerpSpeed = 40.0f;   // 5
	float _colorChangeTime = 0.1f;                       // 33
 	float _dropWait = 0.7f;	                             // 70
	float _defaultDropWait = 0.7f;                       // 70	
};
