#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// get byte(s) from opcodes
#define X   ((opcode & 0x0F00) >> 8)
#define Y   ((opcode & 0x00F0) >> 4)
#define N   (opcode & 0x000F)
#define NN  (opcode & 0x00FF)
#define NNN (opcode & 0x0FFF)

void c8_init(char * filename);
void c8_emulate_cycle(void);

#endif
