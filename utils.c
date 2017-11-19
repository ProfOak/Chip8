#include "utils.h"

void clear_screen(unsigned char gfx[WIDTH][HEIGHT]) {
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            gfx[y][x] = 0;
        }
    }
}
