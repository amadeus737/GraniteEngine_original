#pragma once

#include "Engine/Window.h"
#include "Engine/Game.h"
#include "Engine/EntryPoint.h"
#include "Engine/Color.h"
#include "Engine/Keyboard.h"

#define GraniteGame public Granite::Game
#define ASSIGN_WINDOW_PROPERTIES _windowProperties.Assign
#define SHOW_FPS(x) _showFPS = x
#define GRANITE_RUN_GAME(x) return new x()