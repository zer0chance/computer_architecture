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