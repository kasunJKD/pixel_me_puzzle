#pragma once
#include "defines.h"
#include "assets.h"

// #############################################################################
//                           Render Interface Constants
// #############################################################################
constexpr int MAX_TRANSFORMS = 10000;

// #############################################################################
//                           Render Interface Structs
// #############################################################################
struct Transform 
{
  Vec2 pos;
  Vec2 size;
  IVec2 atlasOffset;
  IVec2 spriteSize;
};

struct RenderData
{
  int transformCount;
  Transform transforms[MAX_TRANSFORMS];
};

static RenderData renderData;


void draw_sprite(SpriteID spriteID, Vec2 pos, Vec2 size)
{
  Sprite sprite = get_sprite(spriteID);

  Transform transform = {};
  transform.pos = pos;
  transform.size = size;
  transform.atlasOffset = sprite.atlasOffset;
  transform.spriteSize = sprite.spriteSize; 

  renderData.transforms[renderData.transformCount++] = transform;
}

