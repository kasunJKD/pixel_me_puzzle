#pragma once

#include "defines.h"

// #############################################################################
//                           Assets Constants
// #############################################################################

// #############################################################################
//                           Assets Structs
// #############################################################################
enum SpriteID
{
  SPRITE_WHITE,
  SPRITE_DICE,
  SPRITE_CELESTE,

  SPRITE_COUNT
};

struct Sprite
{
  IVec2 atlasOffset;
  IVec2 spriteSize;
};

// #############################################################################
//                           Assets Functions
// #############################################################################
Sprite get_sprite(SpriteID spriteID)
{
  Sprite sprite = {};

  switch(spriteID)
  {
    case SPRITE_WHITE:
    {
      sprite.atlasOffset = {0, 0};
      sprite.spriteSize = {1, 1};

      break;

    }

    case SPRITE_CELESTE:
    {
      sprite.atlasOffset = {112, 0};
      sprite.spriteSize = {17, 20};
      break;
    }

    case SPRITE_DICE:
    {
      sprite.atlasOffset = {16, 0};
      sprite.spriteSize = {16, 16};

      break;
    }
  }

  return sprite;
}