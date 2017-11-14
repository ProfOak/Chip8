#include "graphics.h"

SDL_Window* window     = NULL;
SDL_Renderer* renderer = NULL;

bool GFX_IS_RUNNING = true;

int gfx_init(int width, int height) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        exit_with_error("SDL could not initialize");
    }

    printf("%d, %d\n", width, height);
    window = SDL_CreateWindow("Chip8",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height,
                              SDL_WINDOW_OPENGL);
    if (!window) {
        exit_with_error("SDL_CreateWindow");
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        exit_with_error("SDL_CreateRenderer");
    }

    if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)) {
        exit_with_error("SDL_SetRenderDrawColor");
    }

    if (SDL_RenderClear(renderer)) {
        exit_with_error("SDL_RenderClear error");
    }
    SDL_RenderPresent(renderer);

    return 0;
}

void gfx_update(char gfx[64][32]) {
    int width  = 64;
    int height = 32;


    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            if (gfx[y][x] == 1) {
                printf(" ");
                /*printf("%d", gfx[y][x]);*/
                if(SDL_SetRenderDrawColor(renderer,   0,   0  , 0, 255)) {
                    exit_with_error("SDL_SetRenderDrawColor");
                }
            } else {
                printf("%d", gfx[y][x]);
                if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)) {
                    exit_with_error("SDL_SetRenderDrawColor");
                }
            }

            if (SDL_RenderDrawPoint(renderer, y, x)) {
                exit_with_error("SDL_RenderDrawPoint");
            }
        }
        puts("");

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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    GFX_IS_RUNNING = false;
}

void exit_with_error(char *str) {
    printf("[Error] %s: %s\n", str, SDL_GetError());
    gfx_close();
    exit(1);
}
