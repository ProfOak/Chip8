#include "graphics.h"

SDL_Window* window     = NULL;
SDL_Renderer* renderer = NULL;

int WIDTH  = 640;
int HEIGHT = 320;
int SCALE  = 10; // original res is 64x32

int gfx_init() {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        return exit_with_error("SDL could not initialize");
    }

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SET_WHITE();
    SDL_RenderClear(renderer);

    return 1;
}

void gfx_update(char * gfx) {

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 64; y++) {

            if (gfx[x*32 + y] == 1) {
                SET_BLACK();
            } else {
                SET_WHITE();
            }

            SDL_RenderDrawPoint(renderer, x, y);
            //fill_square(x*SCALE, y*SCALE);
        }
    }
}


void gfx_get_key() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch (event.type) {
            case SDL_QUIT:
                gfx_close();

        }
    }
}

void fill_square(int xfrom, int yfrom) {

    // based on SCALE amount
    for (int i = 0; i < SCALE; i++) {
        for (int j = 0; j < SCALE; j++) {
            SDL_RenderDrawPoint(renderer, i,j);
        }
    }
}

void gfx_close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int exit_with_error(char *str) {
    printf("[Error] %s: %s\n", str, SDL_GetError());
    SDL_Quit();
    return 0;
}
