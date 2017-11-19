#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chip8.h"
#include "graphics.h"


int file_exists(char * f);
void usage(void);

int main(int argc, char *argv[]) {

    unsigned char gfx[WIDTH][HEIGHT];

    if (argc != 2 || !file_exists(argv[1])) {
        printf("Invalid rom file\n");
        usage();
        return 1;
    }

    printf("Loading: %s\n", argv[1]);
    puts("C8 init");
    c8_init(argv[1], gfx);
    gfx_init(gfx);

    int i = 0;
    while (GFX_IS_RUNNING && i < 250) {
        printf("%d: ", i++);
        if (GFX_DRAW_FLAG) {
            gfx_update(gfx);
        }

        c8_emulate_cycle(gfx);
        gfx_get_key();
    }
    puts("Exiting...");
    gfx_close();

    return 0;
}

int file_exists(char * f) {
    return access(f, F_OK) != -1;
}

void usage(void) {
    puts("Usage:");
    puts("    chip8 ROM_NAME");
}
