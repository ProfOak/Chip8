#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SET_BLACK() SDL_SetRenderDrawColor(renderer,   0,   0  , 0, 255);
#define SET_WHITE() SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

int  gfx_init(int width, int height);
void gfx_close(void);
void gfx_update(char gfx[64][32]);
void gfx_get_key(void);

void exit_with_error(char *);

extern bool GFX_IS_RUNNING;

#endif
