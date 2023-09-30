#pragma once

#include "input.h"
#include "defines.h"
#include "render_interface.h"

constexpr int tset = 5;
constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;
constexpr int TILESIZE = 8;


enum GameInputType
{
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_UP,
  MOVE_DOWN,
  JUMP,

  MOUSE_LEFT,
  MOUSE_RIGHT,

  GAME_INPUT_COUNT
};

struct KeyMapping
{
  Array<KeyCodeID, 3> keys;
};

struct GameState
{
  bool initialized = false;
  IVec2 playerpos;

  KeyMapping keyMappings[GAME_INPUT_COUNT];
};

static GameState* gameState;

extern "C"
{
  EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderdata, Input* inputIn);
}