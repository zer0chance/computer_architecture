#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>


int mt_clrscr(void)
{
    printf("\033[H\033[2J");
    return EXIT_SUCCESS; 
}

int mt_getscreensize(int* rows, int* cols)
{
    struct winsize ws;

    if (!ioctl(1, TIOCGWINSZ, &ws))
    {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return EXIT_SUCCESS;
    }
    else 
        return EXIT_FAILURE;
}


int mt_gotoXY(int x, int y)
{
    int row, col;
    mt_getscreensize(&row, &col);
    if (x <= row && y <= col)
    { 
       printf("\033[%d;%dH", x, y);
       return EXIT_SUCCESS;
    }
    else
       return EXIT_FAILURE; 
} 


int mt_setfgcolor(int color)
{
    if (color >= 30 && color < 38)
    {
        printf("\033[%dm", color);
        return EXIT_SUCCESS;
    }
    else 
        return EXIT_FAILURE;
}


int mt_setbgcolor(int color)
{
    if (color >= 40 && color < 48)
    {
        printf("\033[%dm", color);
        return EXIT_SUCCESS;
    }
    else 
        return EXIT_FAILURE;
}
