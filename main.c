#include "chip8.h"
#include "graphics.h"
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

    gfx_init();
    while ( 1 ) {
        gfx_update(gfx);

        gfx_get_key();
        c8_emulate_cycle();
    }

    gfx_close();

    return 0;
}

