#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chip8.h"
#include "graphics.h"

#define file_exists(f)  (access(f, F_OK) != -1)
#define usage()         puts("Usage:"); \
                        puts("    chip8 ROM_NAME");

int main(int argc, char *argv[]) {

    unsigned char gfx[WIDTH][HEIGHT];
    unsigned int i = 0;

    if (argc != 2 || !file_exists(argv[1])) {
        printf("Invalid rom file\n");
        usage();
        return 1;
    }

    printf("Loading: %s\n", argv[1]);
    puts("C8 init");
    c8_init(argv[1], gfx);
    gfx_init(gfx, argv[1]);

    while (GFX_IS_RUNNING) {
        if (DEBUG) {
            printf("%d: ", i++);
        }

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
