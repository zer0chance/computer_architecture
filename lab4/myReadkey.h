#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#define ON 1
#define OFF 0

struct termios term_state;


int rk_readkey(int* key)
{
    char ch[20];
    int count;

    if ((count = read(0, ch, 20)) > 0)
    {
        *key = ch[0];
        return EXIT_SUCCESS;
    }
    else 
        return EXIT_FAILURE;
}


int rk_mytermsave(void)
{
    if(!tcgetattr(0, &term_state))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}


int rk_mytermrestore(void)
{
    if(!tcsetattr(0, TCSAFLUSH, &term_state))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}


int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    struct termios ts;
    if(regime) 
        ts.c_lflag |= ICANON;
    else 
        ts.c_lflag &= ~ICANON;

    ts.c_cc[VTIME] = vtime;
    ts.c_cc[VMIN] = vmin;

    if(echo)
        ts.c_lflag |= ECHO;
    else
        ts.c_lflag &= ~ECHO;
    
    if(sigint)
        ts.c_lflag |= ISIG;
    else
        ts.c_lflag &= ~ISIG;

    if(tcsetattr(0, TCSAFLUSH, &ts))
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;            
}