#include <GraniteEngine.h>
#include "Tests/MovementDemo.h"
#include "Tests/ColorDemo.h"
#include "Snek/Snek.h"
#include "Tetris/Tetris.h"
#include "3D_Renderer/RotatingWireframeCube.h"

Granite::Game* Granite::LaunchGame()
{
	ASSIGN_WINDOW_PROPERTIES(L"Test Window", 800, 600, 100, 100);
	SHOW_FPS(true);	GRANITE_RUN_GAME(RotatingWireframeCube);
}
