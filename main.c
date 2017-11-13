#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chip8.h"
#include "graphics.h"


int file_exists(char *f) {
    return access(f, F_OK) != -1;
}

int main(int argc, char *argv[]) {

    if (argc != 2 || !file_exists(argv[1])) {
        printf("Invalid rom file\n");
        return 1;
    }

    char * filename = malloc(20);
    strncpy(filename, argv[1], strlen(argv[1]));
    printf("Loading: %s\n", filename);

    c8_init(filename);
    free(filename);

    puts("C8 init");

    gfx_init();
    // testing...
    // while (GFX_IS_RUNNING) {
    for (int i = 0; i < 1000; i++) {
        if (GFX_DRAW_FLAG) {
            gfx_update(gfx);
        }
        c8_emulate_cycle();
        //gfx_get_key();
    }
    gfx_close();

    return 0;
}

