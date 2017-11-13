#include "graphics.h"

SDL_Window* window     = NULL;
SDL_Renderer* renderer = NULL;

bool GFX_IS_RUNNING = true;

// original res is 64x32
int WIDTH  = 64;
int HEIGHT = 32;

// add option to scale/increase size
int SCALE  = 10;

int gfx_init() {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        GFX_IS_RUNNING = false;
        return exit_with_error("SDL could not initialize");
    }

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SET_WHITE();
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return 1;
}

void gfx_update(char * gfx) {

    int screen_coord = 0;
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            screen_coord = x*HEIGHT + y;

            printf("%d", gfx[screen_coord]);
            if (gfx[screen_coord] == 1) {
                SET_BLACK();
            } else {
                SET_WHITE();
            }

            SDL_RenderDrawPoint(renderer, x, y);
        }
        puts("");
    }
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
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
    GFX_IS_RUNNING = false;
}

int exit_with_error(char *str) {
    printf("[Error] %s: %s\n", str, SDL_GetError());
    SDL_Quit();
    return 0;
}
