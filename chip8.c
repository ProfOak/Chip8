#include "chip8.h"

int key_press;
int draw_flag, pixel;                      // drawing

unsigned short x, y, xline, yline, height; // drawing
unsigned short width = 8;                  // drawing

unsigned short opcode;
unsigned short I;
unsigned short pc;
unsigned short stack[16];
unsigned short sp;

unsigned char V[16];
unsigned char gfx[ 64*32 ];
unsigned char delay_timer;
unsigned char sound_timer;
unsigned char key[16];      // keypress
unsigned char memory[4096];

/*
 * fontset example: the number 0
 *      ->           ->
 * 0xF0    1111 0000    1111
 * 0X90    1001 0000    1  1
 * 0X90    1001 0000    1  1
 * 0X90    1001 0000    1  1
 * 0XF0    1111 0000    1111
 */
unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void c8_init( char * filename ) {
    int buffer_size;
    unsigned char * buffer;
    FILE * fp;

    // store contents of ROM into buffer
    fp = fopen( filename, "rb" );
    fseek( fp, 0, SEEK_END );

    buffer_size = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    buffer = ( unsigned char * ) malloc( sizeof( char ) * buffer_size );
    fread( buffer, 1, buffer_size, fp );
    fclose( fp );
    // end IO

    pc     = 0x200; // starts at 0x200
    opcode = 0;
    I      = 0;
    sp     = 0;

    // load fonts into memory
    for ( int i = 0; i < 80; i++ )
        memory[i] = chip8_fontset[i];

    // load rom into memory
    for ( int i = 0; i < buffer_size; i++ ) {
        memory[i + 512] = buffer[i];
    }

    // 0xCXNN uses a random number
    srand( time( NULL ) );
}

void c8_emulate_cycle( void ) {

    int tmp;

    // get opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    pc += 2;

    printf( "CURRENT OPCODE: 0x%X    |    %d\n", opcode, pc );
    // decode opcode
    switch ( opcode & 0xF000 ) {

        switch ( opcode & 0x000F ) {
            case 0x0000: // 0x00E0
                // clear the screen
                for ( int i = 0; i < 2048; i++ )
                    gfx[i] = 0x0;
                draw_flag = 1;
            break;

            case 0x000E: // 0x00EE
                // returns from subroutine
                pc = stack[--sp];
            break;
        }
        case 0x1000: // 0x1NNN
            // Jumps to address NNN
            pc = NNN;
        break;

        case 0x2000: // 0x2NNN
            // call subroutine at address NNN
            stack[sp++] = pc - 2; // because I increased it already
            pc = NNN;
        break;

        case 0x3000: // 0x3XNN
            // Skips the next instruction if VX equals NN
            if ( opcode & 0x00FF )
                pc += V[X] + 2;
        break;

        case 0x4000: // 0x4XNN
            // Skips the next instruction if VX doesn't equal NN
            if ( V[X] != NN )
                pc += 2;
        break;

        case 0x5000: //0x5XY0
            // Skips the next instruction if VX equals VY
            if ( V[X] == V[Y] )
                pc += 2;
        break;

        case 0x6000: // 0x6XNN
            // Sets VX to NN
            V[X] = NN;
        break;

        case 0x7000: // 0x7XNN
            // Adds NN to VX
            V[X] += NN;
        break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000: // 0x8000
                    // Sets VX to the value of VY
                    V[X] = V[Y];
                break;

                case 0x0001: // 0x8XY1
                    // Sets VX to the value of VY
                    V[X] = V[X] | V[Y];
                break;

                case 0x002: // 0x8XY2
                    // Sets VX to VX and VY
                    V[X] = V[X] & V[Y];
                break;

                case 0x003: // 0x8XY3
                    // Sets VX to VX xor VY
                    V[X] = V[X] ^ V[Y];
                break;

                case 0x0004: // 0x8XY4
                    // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
                    V[X] += V[Y];
                break;

                case 0x0005: // 0x8XY5
                    // VY is subtracted from VX
                    // VF is set to 0 when there's a borrow, and 1 when there isn't
                    tmp  = V[X] < V[Y] ? 1 : 0;
                    V[X] = V[X] - V[Y];
                    V[0xF] = tmp;
                break;

                case 0x0006: // 0x8XY6
                    // Shifts VX right by one
                    // VF is set to the value of the least significant bit of VX before the shift
                    tmp = V[X] & 0x01;
                    V[X] >>= 1;
                    V[0xF] = tmp;

                break;

                case 0x0007: // 0x8XY7
                    // Sets VX to VY minus VX
                    // VF is set to 0 when there's a borrow, and 1 when there isn't
                    tmp    = V[X] < V[Y] ? 1 : 0;
                    V[X]   = V[Y] - V[X];
                    V[0xF] = tmp;
                break;

                case 0x000E: // 0x8XYE
                    // Shifts VX left by one
                    // VF is set to the value of the most significant bit of VX before the shift
                    tmp    = V[X] >> 7;
                    V[X]   = V[X] << 1;
                    V[0xF] = tmp;
                break;
            }
        break;

        case 0x9000: // 0x9XY0
            // Skips the next instruction if VX doesn't equal VY
            if ( V[X] != V[Y] )
                pc += 2;
        break;

        case 0xA000: // 0xANNN
            // set address to NNN
            I = NNN;
        break;

        case 0xB000: // 0xBNNN
            // Jumps to the address NNN plus V0
            pc = V[0] + NNN;
        break;

        case 0xC000: // 0xCXNN
            // Sets VX to the result of a bitwise and operation on a random number and NN
            V[X] = ( rand() * 0xFF ) & NN;
        break;

        case 0xD000: // 0xDXYN drawing
            /*
             * Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels
             * and a height of N pixels. Each row of 8 pixels is read as bit-coded
             * starting from memory location I; I value doesn’t change after the
             * execution of this instruction. As described above, VF is set to 1
             * if any screen pixels are flipped from set to unset when the sprite
             * is drawn, and to 0 if that doesn’t happen.
             */
            x = V[X];
            y = V[Y];
            height = V[N];

            for ( yline = 0; yline < height; yline++ ) {
                pixel = memory[I + yline];

                for ( xline = 0; xline < width; xline++ ) {
                    if ( (pixel & (0x80 >> xline)) != 0 ) {
                        if (gfx[(x + xline + ((y+yline) * 64))] == 1) {
                            V[0xF] = 1;
                        }
                        gfx[x+xline + ((y+yline) * 64)] ^= 1;
                    }
                }
            }

            draw_flag = 1;
        break;

        case 0xE000: // key presses
            switch (opcode & 0x00FF) {
                case 0x009E: // 0xFX9E
                    // Skips the next instruction if the key stored in VX is pressed
                    if ( key[V[X]] != 0 )
                        pc +=  2;
                break;

                case 0x00A1: // 0xFX0A
                    // Skips the next instruction if the key stored in VX isn't pressed
                    if ( key[V[X]] == 0 )
                        pc += 2;
                break;
            }

        case 0xF000:
            switch (opcode & 0x00FF) {

                case 0x0007a: // 0xFX07
                    //Sets VX to the value of the delay timer
                    V[X] = delay_timer;
                break;

                case 0x000A: // 0xFX0A
                    // A key press is awaited, and then stored in VX
                    for ( int i = 0; i < 16; i++ ) {
                        if ( key[i] != 0 ) {
                            V[X] = i;
                            key_press = 1;
                        }
                    }
                    if ( !key_press ) {
                        return;
                    }

                break;

                case 0x0015: // 0xFX15
                    //Sets the delay timer to VX
                    delay_timer = V[X];
                break;

                case 0x0018: // 0xFX18
                    // Sets the sound timer to VX
                    sound_timer = V[X];
                    pc += 2;
                break;

                case 0x001E: // 0xFX1E
                    // Adds VX to I
                    V[0xF] = ( 0xFF - V[X] ) > I ? 1 : 0;
                    I += V[X];
                break;

                case 0x0029: // 0xFX29
                    // Sets I to the location of the sprite for the character in VX
                    // Characters 0-F (in hexadecimal) are represented by a 4x5 font
                    I = V[X] * 0x5;
                break;

                case 0x0033: // 0xFX33
                    /* Stores the Binary-coded decimal representation of VX,
                     * with the most significant of three digits at the address
                     * in I, the middle digit at I plus 1, and the least significant
                     * digit at I plus 2. (In other words, take the decimal
                     * representation of VX, place the hundreds digit in memory at
                     * location in I, the tens digit at location I+1, and the
                     * ones digit at location I+2.)
                     *
                     * V[X] = unsigned char = (0 - 255)
                     *                I  I+1 I+2
                     * decimal digit [0] (0) {0}
                     */
                    memory[I]   =   V[X] / 100;
                    memory[I+1] = ( V[X] / 10 ) % 10;
                    memory[I+2] = ( V[X] % 100 ) % 10;
                break;

                case 0x0055: // 0xFX55
                    // Stores V0 to VX in memory starting at address I
                    for ( int i = 0; i < X; i++ )
                        memory[I + i] = V[i];
                break;

                case 0x0065: // 0xFX65
                    // Fills V0 to VX with values from memory starting at address I
                    for ( int i = 0; i < X; i++ )
                        V[i] = memory[I + i];
                break;
            }
        break;

        default:
            printf( "Unknown opcode: 0x%X\n", opcode );
    }


    // update timers
    if ( delay_timer > 0 )
        --delay_timer;
    if ( sound_timer > 0 ) {
        if ( sound_timer == 1 )
            printf( "beep?\n" );
        --sound_timer;
    }
}

