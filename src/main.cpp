#include "defines.h"
#include "input.h"
#include "game.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include "platform.h"
#ifdef _WIN32
#include "win32_platform.cpp"
#endif

#include "gl_render.cpp"

// #############################################################################
//                           Game DLL Stuff
// #############################################################################
// This is the function pointer to update_game in game.cpp
typedef decltype(update_game) update_game_type;
static update_game_type* update_game_ptr;

// #############################################################################
//                           Cross Platform functions
// #############################################################################
void reload_game_dll(Allocator* transientStorage);

int main()
{
    Allocator transtientStorage = make_bump_allocator(MB(50));
    Allocator persistentStorage = make_bump_allocator(MB(50));

    input = (Input*)bump_alloc(&persistentStorage, sizeof(Input));
    if(!input)
    {
        SM_ERROR("Failed to allocate Input");
        return -1;
    }

    renderData = (RenderData*)bump_alloc(&persistentStorage, sizeof(RenderData));
    if(!renderData)
    {
        SM_ERROR("Failed to allocate RenderData");
        return -1;
    }

    platform_create_window(1200, 720, "Puzzle deez nutz");
    input->screenSizeX = 1200;
    input->screenSizeY = 720;

    gl_init(&transtientStorage);

    while(running)
    {
      reload_game_dll(&transtientStorage);

      platform_update_window();
      update_game(renderData, input);
      gl_render();
      platform_swap_buffers();

      transtientStorage.used = 0;
    }

    return 0;
} 

void update_game(RenderData* renderdata, Input* inputIn)
{
  update_game_ptr(renderdata, inputIn);
}

void reload_game_dll(Allocator* transientStorage)
{
  static void* gameDLL;
  static long long lastEditTimestampGameDLL;

  long long currentTimestampGameDLL = get_timestamp("game.dll");
  if(currentTimestampGameDLL > lastEditTimestampGameDLL)
  {
    if(gameDLL)
    {
      bool freeResult = platform_free_dynamic_library(gameDLL);
      SM_ASSERT(freeResult, "Failed to free game.dll");
      gameDLL = nullptr;
      SM_TRACE("Freed game.dll");
    }

    while(!copy_file("game.dll", "game_load.dll", transientStorage))
    {
      Sleep(10);
    }
    SM_TRACE("Copied game.dll into game_load.dll");

    gameDLL = platform_load_dynamic_library("game_load.dll");
    SM_ASSERT(gameDLL, "Failed to load game.dll");

    update_game_ptr = (update_game_type*)platform_load_dynamic_function(gameDLL, "update_game");
    SM_ASSERT(update_game_ptr, "Failed to load update_game function");
    lastEditTimestampGameDLL = currentTimestampGameDLL;
  }
}
