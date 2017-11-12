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

    // testing...
    for (int i = 0; i < 200; i++) {
        c8_emulate_cycle();
    }

    /*
     * TODO: reinstate when opcodes function properly
     gfx_init();
     while (true) {
         gfx_update(gfx);
         c8_emulate_cycle();
         gfx_get_key();
     gfx_close();
    */

    return 0;
}

