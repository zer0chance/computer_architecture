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
    //bc_printA("ccc");
    //bc_box(5, 20, 40, 20);

    int n1 = 4286677503;
    int n2 = 2172748287;
    int char1[2] = {n1, n2};
    bc_printbigchar(char1, 10, 20, 0, 0);
    int i = -2147483648;
    //printf("\n%d\n", (n1 >> 14) );
    return 0;
}