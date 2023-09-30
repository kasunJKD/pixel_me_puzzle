#pragma once

#include "input.h"
#include "defines.h"
#include "render_interface.h"

constexpr int tset = 5;

struct GameState
{
  IVec2 playerpos;
};

extern "C"
{
  EXPORT_FN void update_game(RenderData* renderdata, Input* inputIn);
}