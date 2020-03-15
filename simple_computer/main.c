#include "share.h"
#include "myReadkey.h"
#include "myBigChars.h"
#include "myTerm.h"
#include "mySimpleComputer.h"
#include <stdio_ext.h>

int input_x = 27;
int input_y = 3;
 
void print_ic_content()
{
    if(RAM[IC] & TWO_POW_FIFTEEN)
        bc_printbigchar(bc_chars[MINUS], 17, 3, 0, 0);
    else
        bc_printbigchar(bc_chars[PLUS], 17, 3, 0, 0);

    int comand_num, operand;  
    sc_commandDecode(RAM[IC], &comand_num, &operand);

    bc_printbigchar(bc_chars[(comand_num >> 4) & 7], 17, 12, 0, 0);
    bc_printbigchar(bc_chars[comand_num & 15], 17, 21, 0, 0);
    bc_printbigchar(bc_chars[(operand >> 4) & 7], 17, 30, 0, 0);
    bc_printbigchar(bc_chars[operand & 15], 17, 39, 0, 0);
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


void print_memory()
{
    bc_box(4, 2, 62, 12);
    mt_gotoXY(4, 29);
    printf("Memory");
    int comand_num, operand;
    for (int i = 0, k = 0, x = 5, y = 3; i < 9; i++)
    {
        mt_gotoXY(x++, y);
        for (int j = 0; j < 10; j++, k++)
        {  
            if(RAM[k] & 16384)
                printf("-");
            else
                printf("+");
                
            sc_commandDecode(RAM[k], &comand_num, &operand);

            if(comand_num > 15)
                printf("%X", comand_num);
            else 
                printf("0%X", comand_num); 

            if(operand > 15)
                printf("%X ", operand);
            else 
                printf("0%X ", operand);       
        }
    }
}


void print_term()
{
    //mt_clrscr();
    print_memory();

    bc_box(4, 65, 20, 3);
    mt_gotoXY(4, 69);
    printf("accumulator");
    mt_gotoXY(5, 72);
    printf("%d", Accumulator);

    bc_box(7, 65, 20, 3);
    mt_gotoXY(7, 66);
    printf("instructionCounter");
    mt_gotoXY(8, 72);
    printf("%d", IC);

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

    mt_gotoXY(26, 3); 
    printf("Input/Output:");
    mt_gotoXY(input_x, input_y);
}


int main()
{
    rk_mytermsave();
    rk_mytermregime(OFF, 0, 1, OFF, OFF);

    sc_memorySet(IC, 34181);
    sc_regSet(CLOCK_IGNORE, 1);

    int new_ic;
    char ch, garbage;
    for(;;)
    {
        print_term();
        fflush(stdout);
        read(0, &ch, 1);


        if (ch == 27)
        {
            read(0, &ch, 1);
            read(0, &ch, 1);
            read(0, &ch, 1);
            //read(0, &garbage, 10); // to read one extra byte of fn button
            __fpurge(stdin);
            // if(mt_gotoXY(input_x, input_y)) printf("Failed to move to io!\n\n");
            // if(rk_mytermrestore()) printf("Failed to restore terminal state!\n\n");
            switch (ch)
            {
            case 53:    // F5
                mt_gotoXY(input_x++, input_y);
                rk_mytermrestore();
                printf("Accumulator value: ");
                fflush(stdout);
                scanf("%d", &Accumulator);
                break;
            case 55:    // F6
                mt_gotoXY(input_x++, input_y);
                rk_mytermrestore();
                printf("IC value: ");
                fflush(stdout);
                scanf("%d", &new_ic);

                if((new_ic >= 0) && (new_ic < 100))
                    IC = new_ic;
                else
                {
                    printf("  Invalid value! %d", ch);
                    input_x++;
                }
                    
                fflush(stdout);
                break;
            default:
                printf(" Invalid button fn %c!", ch);    
            }
            rk_mytermregime(OFF, 0, 1, OFF, OFF);
        }

        if(ch == 's')
        {
            char filename[20];
            if(mt_gotoXY(input_x++, input_y)) printf("Failed to move to io!\n\n");
            if(rk_mytermrestore()) printf("Failed to restore terminal state!\n\n");
            printf("Filename: ");
            fflush(stdout);

            read(0, filename, 20);
            printf("\nget: %s\n\n", filename);
            rk_mytermregime(OFF, 0, 1, OFF, OFF);
        }
        
        if(ch == 'q') break;

        fflush(stdout);
    }


    if(rk_mytermrestore()) printf("Failed to restore terminal state!\n\n");

    mt_gotoXY(40, 1);     

    return 0;
}

