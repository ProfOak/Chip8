#include "chip8.h"

int main(void) {

    char *filename = "PONG";
    c8_init(filename);

    for (int i = 0; i < 15; i++) {
        // emu one cycle at a time
        c8_emulate_cycle();
    }

    return 0;
}
