#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>


enum
{
    FG_BLACK   = 30,
    FG_RED     = 31, 
    FG_GREEN   = 32,
    FG_YELLOW  = 33, 
    FG_BLUE    = 34, 
    FG_MAGENTA = 35, 
    FG_CYAN    = 36, 
    FG_WHITE   = 37
};


enum
{
    BG_BLACK   = 40,
    BG_RED     = 41, 
    BG_GREEN   = 42,
    BG_YELLOW  = 43, 
    BG_BLUE    = 44, 
    BG_MAGENTA = 45, 
    BG_CYAN    = 46, 
    BG_WHITE   = 47
};


int mt_clrscr(void)
{
    printf("\033[H\033[2J");
    return EXIT_SUCCESS; 
}


int mt_gotoXY(int x, int y)
{
    printf("\033[%d;%dH", x, y);
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