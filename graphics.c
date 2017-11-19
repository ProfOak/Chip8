#include "graphics.h"

SDL_Window * window     = NULL;
SDL_Renderer * renderer = NULL;
SDL_Rect rect;

const int scale = 10;
int square_x = 0;
int square_y = 0;
bool GFX_IS_RUNNING = true;

void gfx_init(unsigned char gfx[WIDTH][HEIGHT]) {

    clear_screen(gfx);

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        exit_with_error("SDL could not initialize");
    }

    window = SDL_CreateWindow("Chip8",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH*scale,
                              HEIGHT*scale,
                              SDL_WINDOW_OPENGL);
    if (!window) {
        exit_with_error("SDL_CreateWindow");
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        exit_with_error("SDL_CreateRenderer");
    }

    SET_WHITE();

    if (SDL_RenderClear(renderer)) {
        exit_with_error("SDL_RenderClear error");
    }
    SDL_RenderPresent(renderer);

}

void gfx_update(unsigned char gfx[WIDTH][HEIGHT]) {

    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {

            if (gfx[y][x] == 1) {
                SET_WHITE();
            } else {
                SET_BLACK();
            }

            rect = (SDL_Rect) {
                .x = y*scale,
                .y = x*scale,
                .h = scale,
                .w = scale,
            };
            SDL_RenderDrawRect(renderer, &rect);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}


void gfx_get_key(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch (event.type) {
            case SDL_QUIT:
                gfx_close();

        }
    }
}

void gfx_close(void) {
    GFX_IS_RUNNING = false;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void exit_with_error(char *str) {
    printf("[Error] %s: %s\n", str, SDL_GetError());
    gfx_close();
    exit(1);
}
