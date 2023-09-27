#include "defines.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include "platform.h"
#ifdef _WIN32
#include "win32_platform.cpp"
#endif

#include "gl_render.h"

int main()
{
    platform_create_window(1200, 720, "Puzzle deez nutz");

    while(running)
    {
        platform_update_window();
    }

    return 0;
} 
