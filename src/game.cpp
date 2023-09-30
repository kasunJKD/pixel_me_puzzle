#include "game.h"
#include "assets.h"

EXPORT_FN void update_game(RenderData* renderDataIn, Input* inputIn)
{
    if(renderData != renderDataIn)
    {
        renderData = renderDataIn;
        input = inputIn;
    }
    draw_sprite(SPRITE_DICE, {100.0f, 100.0f}, {100.0f, 100.0f});
}