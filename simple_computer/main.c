#include "share.h"
#include "myReadkey.h"
#include "myBigChars.h"
#include "myTerm.h"
#include "mySimpleComputer.h"


void print_ic_content()
{
    int x = 3;
    bc_printbigchar(bc_chars[10], 17, x, 0, 0);
    bc_printbigchar(bc_chars[RAM[IC] / 1000], 17, 12, 0, 0);
    bc_printbigchar(bc_chars[(RAM[IC] % 1000) / 100], 17, 21, 0, 0);
    bc_printbigchar(bc_chars[(RAM[IC] % 100) / 10], 17, 30, 0, 0);
    bc_printbigchar(bc_chars[RAM[IC] % 10], 17, 39, 0, 0);
}


void print_flags()
{
    mt_gotoXY(14, 71);
    int value;
    sc_regGet(WRONG_OPCODE, &value);
    if (value)
        printf("W ");

    sc_regGet(CLOCK_IGNORE, &value);
    if (value)
        printf("C ");

    sc_regGet(MEM_OUT_OF_BOUND, &value);
    if (value)
        printf("M ");

    sc_regGet(ZERO_DIVISION, &value);
    if (value)
        printf("Z ");

    sc_regGet(OP_OVERFLOW, &value);
    if (value)
        printf("O ");                
}


void print_term()
{
    bc_box(4, 2, 62, 12);
    mt_gotoXY(4, 29);
    printf("Memory");
    for (int i = 0, k = 0, x = 5, y = 3; i < 9; i++)
    {
        mt_gotoXY(x++, y);
        for (int j = 0; j < 10; j++)
        {
            printf("+%-4d ", RAM[k++]);
        }
    }

    bc_box(4, 65, 20, 3);
    mt_gotoXY(4, 69);
    printf("accumulator");

    bc_box(7, 65, 20, 3);
    mt_gotoXY(7, 66);
    printf("instructionCounter");
    mt_gotoXY(8, 72);
    printf("+%-4d", IC);

    bc_box(10, 65, 20, 3);
    mt_gotoXY(10, 70);
    printf("Operation");

    bc_box(13, 65, 20, 3);
    mt_gotoXY(13, 72);
    printf("Flags");
    print_flags(); 

    bc_box(16, 2, 47, 10);
    print_ic_content();

    bc_box(16, 50, 35, 10);
    mt_gotoXY(16, 52);
    printf("Keys:");

    mt_gotoXY(17, 51);
    printf("l  - load");
    mt_gotoXY(18, 51);
    printf("s  - save");
    mt_gotoXY(19, 51);
    printf("r  - run");
    mt_gotoXY(20, 51);
    printf("t  - step");
    mt_gotoXY(21, 51);
    printf("i  - reset");
    mt_gotoXY(22, 51);
    printf("F5 - accumulator");
    mt_gotoXY(23, 51);
    printf("F6 - inctructionCounter");
}


int main()
{
    sc_memorySet(IC, 3245);
    IC = 44;
    sc_memorySet(IC, 23);

    sc_regSet(WRONG_OPCODE, 1);
    sc_regSet(CLOCK_IGNORE, 1);
    sc_regSet(OP_OVERFLOW, 1);

    print_term();

    mt_gotoXY(40, 1);       

    return 0;
}