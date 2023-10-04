#include "game.h"
#include "assets.h"

bool just_pressed(GameInputType type)
{
  KeyMapping mapping = gameState->keyMappings[type];
  for(int idx = 0; idx < mapping.keys.count; idx++)
  {
    if(input->keys[mapping.keys[idx]].justPressed)
    {
      return true;
    }
  }

  return false;
}

bool is_down(GameInputType type)
{
  KeyMapping mapping = gameState->keyMappings[type];
  for(int idx = 0; idx < mapping.keys.count; idx++)
  {
    if(input->keys[mapping.keys[idx]].isDown)
    {
      return true;
    }
  }

  return false;
}

IVec2 get_grid_pos(IVec2 worldPos)
{
  return {worldPos.x / TILESIZE, worldPos.y / TILESIZE};
}

Tile* get_tile(int x, int y)
{
  Tile* tile = nullptr;

  if(x >= 0  && x < WORLD_GRID.x && y >= 0 && y < WORLD_GRID.y)
  {
    tile = &gameState->worldGrid[x][y];
  }

  return tile;
}

Tile* get_tile(IVec2 worldPos)
{
  IVec2 gridPos = get_grid_pos(worldPos);
  return get_tile(gridPos.x, gridPos.y);
}

void simulate() {

  gameState->player.prevPos = gameState->player.pos;
  
  {
    if(is_down(MOVE_LEFT))
    {
        gameState->player.pos.x -= 1;
    }
    if(is_down(MOVE_RIGHT))
    {
       gameState->player.pos.x += 1; 
    }
    if(is_down(MOVE_UP))
    {
        gameState->player.pos.y -= 1;
    }
     if(is_down(MOVE_DOWN))
    {
        gameState->player.pos.y += 1;
    }
  }
  bool updateTiles = false;
  if(is_down(MOUSE_LEFT))
    {
      IVec2 mousePosWorld = input->mousePosWorld;
      Tile* tile = get_tile(mousePosWorld);
      if(tile)
      {
        tile->isVisible = true;
        updateTiles = true;
      }
    }

    if(is_down(MOUSE_RIGHT))
    {
      IVec2 mousePosWorld = input->mousePosWorld;
      Tile* tile = get_tile(mousePosWorld);
      if(tile)
      {
        tile->isVisible = false;
        updateTiles = true;
      }
    }

    if(updateTiles)
  {
    // Neighbouring Tiles        Top    Left      Right       Bottom  
    int neighbourOffsets[24] = { 0,-1,  -1, 0,     1, 0,       0, 1,   
    //                          Topleft Topright Bottomleft Bottomright
                                -1,-1,   1,-1,    -1, 1,       1, 1,
    //                           Top2   Left2     Right2      Bottom2
                                 0,-2,  -2, 0,     2, 0,       0, 2};

    // Topleft     = BIT(4) = 16
    // Toplright   = BIT(5) = 32
    // Bottomleft  = BIT(6) = 64
    // Bottomright = BIT(7) = 128

    for(int y = 0; y < WORLD_GRID.y; y++)
    {
      for(int x = 0; x < WORLD_GRID.x; x++)
      {
        Tile* tile = get_tile(x, y);

        if(!tile->isVisible)
        {
          continue;
        }

        tile->neighbourMask = 0;
        int neighbourCount = 0;
        int extendedNeighbourCount = 0;
        int emptyNeighbourSlot = 0;

        // Look at the sorrounding 12 Neighbours
        for(int n = 0; n < 12; n++)
        {
          Tile* neighbour = get_tile(x + neighbourOffsets[n * 2],
                                     y + neighbourOffsets[n * 2 + 1]);

          // No neighbour means the edge of the world
          if(!neighbour || neighbour->isVisible)
          {
            tile->neighbourMask |= BIT(n);
            if(n < 8) // Counting direct neighbours
            {
              neighbourCount++;
            }
            else // Counting neighbours 1 Tile away
            {
              extendedNeighbourCount++;
            }
          }
          else if(n < 8)
          {
            emptyNeighbourSlot = n;
          }
        }

        if(neighbourCount == 7 && emptyNeighbourSlot >= 4) // We have a corner
        {
          tile->neighbourMask = 16 + (emptyNeighbourSlot - 4);
        }
        else if(neighbourCount == 8 && extendedNeighbourCount == 4)
        {
          tile->neighbourMask = 20;
        }
        else
        {
          tile->neighbourMask = tile->neighbourMask & 0b1111;
        }
      }
    }
  }
}

EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float dt)
{
    if(renderData != renderDataIn)
    {
        gameState = gameStateIn;
        renderData = renderDataIn;
        input = inputIn;
    }

     if(!gameState->initialized)
    {
        renderData->gameCamera.dimensions = {WORLD_WIDTH, WORLD_HEIGHT};
        gameState->initialized = true;

         // Key Mappings
        {
            gameState->keyMappings[MOVE_UP].keys.add(KEY_W);
            gameState->keyMappings[MOVE_UP].keys.add(KEY_UP);
            gameState->keyMappings[MOVE_LEFT].keys.add(KEY_A);
            gameState->keyMappings[MOVE_LEFT].keys.add(KEY_LEFT);
            gameState->keyMappings[MOVE_DOWN].keys.add(KEY_S);
            gameState->keyMappings[MOVE_DOWN].keys.add(KEY_DOWN);
            gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_D);
            gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_RIGHT);
            gameState->keyMappings[MOUSE_LEFT].keys.add(KEY_MOUSE_LEFT);
            gameState->keyMappings[MOUSE_RIGHT].keys.add(KEY_MOUSE_RIGHT);
            gameState->keyMappings[JUMP].keys.add(KEY_SPACE);
        }

        renderData->gameCamera.position.x = 160;
        renderData->gameCamera.position.y = -90;

        {
          IVec2 tilePosition = {48, 0};

          for(int y = 0; y < 5; y++)
          {
            for(int x = 0; x < 4; x++)
            {
              gameState->tileCoords.add({tilePosition.x + x*8, tilePosition.y + y * 8});
            }
          }

          gameState->tileCoords.add({tilePosition.x, tilePosition.y + 5 * 8});
        }

        
    }

    // Fixed Update Loop
    {
      gameState->updateTimer += dt;
      while(gameState->updateTimer >= UPDATE_DELAY)
      {
        gameState->updateTimer -= UPDATE_DELAY;
        simulate();

        // Relative Mouse here, because more frames than simulations
        input->relMouse = input->mousePos - input->prevMousePos;
        input->prevMousePos = input->mousePos;

        // Clear the transitionCount for every key
        {
          for (int keyCode = 0; keyCode < KEY_COUNT; keyCode++)
          {
            input->keys[keyCode].justReleased = false;
            input->keys[keyCode].justPressed = false;
            input->keys[keyCode].halfTransitionCount = 0;
          }
        }
      }
    }

    float interpolatedDT = (float)(gameState->updateTimer / UPDATE_DELAY);

    {
      Player& player = gameState->player;
      IVec2 playerPos = lerp(player.prevPos, player.pos, interpolatedDT);
      draw_sprite(SPRITE_DICE, gameState->player.pos); 

    }


    {
      for(int y = 0; y < WORLD_GRID.y; y++)
      {
        for(int x = 0; x < WORLD_GRID.x; x++)
        {
          Tile* tile = get_tile(x, y);

          if(!tile->isVisible)
          {
            continue;
          }
    
        Transform transform = {};
        transform.pos = {x * (float)TILESIZE, y * (float)TILESIZE};
        transform.size = {8 ,8};
        transform.spriteSize = {8, 8};
        transform.atlasOffset = gameState->tileCoords[tile->neighbourMask];
        draw_quad(transform);
      }
      
      }
    }

    

    

    

}