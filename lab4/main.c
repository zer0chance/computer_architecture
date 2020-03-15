// #include <unistd.h> 
// #include <stdlib.h> 
// #include <fcntl.h> 
// #include <termios.h> 
// #include <stdio.h> 
// int main ( int argc, char **argv ) { 
//    struct termios savetty, tty; 
//    char ch; 
//    int x, y; 
//    printf( "Enter start position (x y): " ); 
//    scanf( "%d %d", &x, &y ); 
//    if( !isatty( 0 ) ) { 
//       fprintf( stderr, "stdin not terminal\n" ); 
//       exit( EXIT_FAILURE ); 
//    }; 
//    tcgetattr( 0, &tty );             // получили состояние терминала 
//    savetty = tty; 
//    tty.c_lflag &= ~( ICANON | ECHO | ISIG ); 
//    tty.c_cc[ VMIN ] = 1; 
//    tcsetattr( 0, TCSAFLUSH, &tty ); // изменили состояние терминала 
//    printf( "%c[2J", 27 );           // очистили экран 
//    fflush( stdout ); 
//    printf( "%c[%d;%dH", 27, y, x ); // установили курсор в позицию 
//    fflush( stdout ); 
//    for( ; ; ) { 
//       read( 0, &ch, 1 ); 
//       if( ch == 'q' ) break; 
//       switch( ch ) { 
//       case 'j': 
//          printf( "%c[1A", 27 ); 
//          break;
//       case 'k': 
//          printf( "%c[1B", 27 ); 
//          break;
//       case 'l': 
//          printf( "%c[1C", 27 ); 
//          break;
//       case 'h': 
//          printf( "%c[1D", 27 ); 
//          break;
//       }; 
//       fflush( stdout ); 
//    }; 
//    tcsetattr( 0, TCSAFLUSH, &savetty ); // восстановили состояние терминала 
//    printf( "\n" ); 
//    exit( EXIT_SUCCESS ); 
// } 








#include "myReadkey.h"


int main()
{
    rk_mytermsave();
    rk_mytermregime(OFF, 0, 1, OFF, OFF);

    printf( "\033[2J"); // cls
    fflush(stdout);

    int ch;

    for(;;)
    {
        rk_readkey(&ch);
        if (ch == 'q') break;

        switch (ch)
        {
        case 'k':
            printf("\033[1A");
            break;
        case 'j':
            printf("\033[1B");
            break;
        case 'l':
            printf("\033[1C");
            break;
        case 'h':
            printf("\033[1D");
            break;        
        }

        fflush(stdout);
    }

    if(rk_mytermrestore()) printf("Failed to restore terminal state!\n\n");
    

    return 0;
}