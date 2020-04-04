#include <stdio.h>


typedef struct CHS
{
    __uint8_t C : 2;
    __uint8_t H : 2;
    __uint8_t S : 2;
} tCHS;


typedef struct IDECHS
{
    __uint8_t C : 2;
    __uint8_t H : 2;
    __uint8_t S : 2;
} tIDECHS;


typedef struct LARGE
{
    __uint16_t C : 10;
    __uint8_t  H : 6;
    __uint8_t  S : 8;
    __uint64_t adress;
} tLARGE;


void foo()
{

}


int main()  
{
    register tCHS d;

    d.C = 4;
    d.H = 4;
    d.S = 4;

    printf("\n C: %d\n H: %d\n S: %d\n\n", d.C, d.H, d.S);

    return 0;
}