#include <GraniteEngine.h>
#include "KeyInputTest.h"

Granite::Game* Granite::LaunchGame()
{
	ASSIGN_WINDOW_PROPERTIES(L"Test Window", 800, 600, 300, 50);
	//SHOW_FPS(true);
	GRANITE_RUN_GAME(KeyInputTest);
}
