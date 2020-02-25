#include "myBigChars.h"

// 11111111
// 10000001
// 10000001
// 11111111
// 10000001
// 10000001
// 10000001
// 11111111

int main()
{
    bc_printA("\n\nabc\n\n");
    bc_box(4, 8, 8, 4);
    bc_box(2, 4, 60, 30);
    

    int n1 = 4286677503;
    int n2 = 2172748287;
    int char1[2] = {n1, n2};
    bc_printbigchar(char1, 10, 10, 36, 43);

    // int char2[2] = {0, 0};
    bc_setbigcharpos(char1, 1, 1, 0);
    bc_setbigcharpos(char1, 1, 8, 0);
    bc_setbigcharpos(char1, 8, 8, 0);
    bc_setbigcharpos(char1, 8, 1, 0);
    bc_setbigcharpos(char1, 3, 4, 1);
    bc_setbigcharpos(char1, 6, 4, 1);
    bc_printbigchar(char1, 20, 10, 36, 43);

    //printf("\n%d\n", (n1 >> 14) );
    printf("\033[50;1H");
    int value;
    bc_getbigcharpos(char1, 1, 2, &value);
    printf("\n (1; 2) = %d \n\n", value);

    return 0;
}