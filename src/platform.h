#pragma  once

//globals
static bool running = true;

//------------platform functions
bool platform_create_window(int width, int height, char* title);
void platform_update_window();
void* platform_load_gl_function(char* glName);
void platform_swap_buffers();