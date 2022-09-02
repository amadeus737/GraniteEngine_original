#include <GraniteEngine.h>
#include "Tests/MovementDemo.h"
#include "Tests/ColorDemo.h"
#include "Snek/Snek.h"

Granite::Game* Granite::LaunchGame()
{
	ASSIGN_WINDOW_PROPERTIES(L"Test Window", 800, 600, 300, 50);
	//SHOW_FPS(true);
	GRANITE_RUN_GAME(Snek);
}
