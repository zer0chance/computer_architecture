#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

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

}


int rk_mytermrestore(void)
{

}


int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{

}