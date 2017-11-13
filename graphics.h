#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define SET_BLACK() SDL_SetRenderDrawColor(renderer,   0,   0  , 0, 255);
#define SET_WHITE() SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

int  gfx_init();
void gfx_close();
void gfx_update();
void gfx_get_key();

void fill_square();
int exit_with_error(char *);

extern bool GFX_IS_RUNNING;

#endif
