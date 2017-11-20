#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "utils.h"


#define SET_BLACK() if(SDL_SetRenderDrawColor(renderer,   0,   0  , 0, 255)) \
    exit_with_error("SetRenderDrawColor")
#define SET_WHITE() if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)) \
    exit_with_error("SetRenderDrawColor")

void gfx_init(unsigned char gfx[WIDTH][HEIGHT], char * filename);
void gfx_update(unsigned char gfx[WIDTH][HEIGHT]);
void gfx_close(void);
void gfx_get_key(void);

void exit_with_error(char *);

extern bool GFX_IS_RUNNING;

#endif
