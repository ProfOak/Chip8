#include "chip8.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int file_exists ( char *f ) {
    return access( f, F_OK ) != -1;
}

int main(int argc, char *argv[]) {

    if ( argc != 2 || !file_exists( argv[1] ) ) {
        printf( "Invalid file\n" );
        return 1;
    }

    char * filename = malloc( 20 );
    strncpy( filename,  argv[1], strlen( argv[1] ) );

    c8_init( filename );

    free( filename );

    for ( int i = 0; i < 15; i++ ) {
        // emu one cycle at a time
        c8_emulate_cycle();
    }

    return 0;
}

