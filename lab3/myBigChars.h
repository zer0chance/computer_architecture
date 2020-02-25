#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

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

int bc_printA(char* str)
{
    printf("\033(0%s\033(B", str);
    return EXIT_SUCCESS;
}


int bc_box(int x1, int y1, int x2, int y2)
{
    struct winsize ws;
    if (!ioctl(1, TIOCGWINSZ, &ws))
    {
        if (x1 <= ws.ws_row && y2 <= ws.ws_col)
        { 
            printf("\033[%d;%dH\033(0", x1, y1);
            printf("┌");
            for (int i = 1; i < x2 - 1; i++)
                printf("─");
            printf("┐");
            for (int i = 1; i < y2 - 1; i++)
            {
                printf("\033[%d;%dH│", ++x1, y1);
                // for (int i = 1; i < x2 - 1; i ++)
                //     printf(" ");
                printf("\033[%d;%dH│", x1, y1 + x2 - 1);    
            }
            printf("\033[%d;%dH└", ++x1, y1);
            for (int i = 1; i < x2 - 1; i ++)
                printf("─");
            printf("┘\033(B");
            return EXIT_SUCCESS;
        }
        else
            return EXIT_FAILURE; 
    }
    else 
        return EXIT_FAILURE;
}


int bc_printbigchar(int bc[2], int x, int y, int fgcolor, int bgcolor)
{
    struct winsize ws;
    if (!ioctl(1, TIOCGWINSZ, &ws))
    {
        if (x <= ws.ws_row - 8 && y <= ws.ws_col - 8)
        { 
            if (fgcolor >= 30 && fgcolor < 38 && bgcolor >= 40 && bgcolor < 48)
            {
                printf("\033[%d;%dH\033(0\033[%d;%dm", x, y, fgcolor, bgcolor);
            }
            else 
                return EXIT_FAILURE;
            for (int k = 0; k < 2; k++)
            {
                int mask = 31;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if ((bc[k] >> mask) & 1) //TODO: need fix
                        {
                            printf("▒"); 
                        }
                        else 
                        {
                            printf(" "); 
                        } 
                        mask--; 
                    }
                    printf("\033[%d;%dH", ++x, y);
                }
            }    
            
            printf("\033(B\033[0m");
            return EXIT_SUCCESS;
        }
        else
            return EXIT_FAILURE; 
    }
    else 
        return EXIT_FAILURE;
}


int bc_setbigcharpos(int bc[2], int x, int y, int value) // x and y starts is in range 1..8
{
    if (x > 0 && x <= 8 && y > 0 && y <= 8)
    {
        int i, pos;
        if (x > 4)
        {
            i = 1;
            x -= 4;
        }
        else 
            i = 0;
        switch (value)
        {
            case 0:
                pos = (x - 1) * 8 + y; 
                bc[i] &= ~(1 << (32 - pos));    
                return EXIT_SUCCESS;
                break;
            case 1:
                pos = (x - 1) * 8 + y; 
                bc[i] |= (1 << (32 - pos)); // Because we count coordinates from left to right in number    
                return EXIT_SUCCESS;
            default:
                return EXIT_FAILURE;
                break;
        }
    }
    else 
        return EXIT_FAILURE;
}


int bc_getbigcharpos(int bc[2], int x, int y, int *value)
{
    if (x > 0 && x <= 8 && y > 0 && y <= 8)
    {
        int i, pos;
        if (x > 4)
        {
            i = 1;
            x -= 4;
        }
        else 
            i = 0;
        pos = (x - 1) * 8 + y;     
        *value = (bc[i] & (1 << (32 - pos))) >> (32 - pos);     
        return EXIT_SUCCESS; 
    }
    else 
        return EXIT_FAILURE;
}


int bc_bigcharwrite(int fd, int* big, int count)
{
    //TODO
}


int bc_bigcharread(int fd, int* big,int need_count, int* count)
{
    //TODO
}