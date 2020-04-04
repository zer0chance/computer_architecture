#include "share.h"
#include "myReadkey.h"
#include "myBigChars.h"
#include "myTerm.h"
#include "mySimpleComputer.h"
 
struct itimerval nval, oval;

void print_selected()
{
    if(RAM[selected_pos] & TWO_POW_FIFTEEN)
        bc_printbigchar(bc_chars[MINUS], 17, 3, 0, 0);
    else
        bc_printbigchar(bc_chars[PLUS], 17, 3, 0, 0);

    int comand_num, operand;  
    sc_commandDecode(RAM[selected_pos], &comand_num, &operand);

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
    else
        printf("  ");    

    sc_regGet(CLOCK_IGNORE, &value);
    if (value)
        printf("C ");
    else
        printf("  ");     

    sc_regGet(MEM_OUT_OF_BOUND, &value);
    if (value)
        printf("M ");
    else
        printf("  ");     

    sc_regGet(ZERO_DIVISION, &value);
    if (value)
        printf("Z ");
    else
        printf("  ");     

    sc_regGet(OP_OVERFLOW, &value);
    if (value)
        printf("O ");
    else
        printf("  ");                     
}


void print_memory()
{
    bc_box(4, 2, 62, 12);
    mt_gotoXY(4, 29);
    printf("Memory");
    int comand_num, operand;
    for (int i = 0, k = 0, x = 5; i < 10; i++, x++)
    {
        mt_gotoXY(x, 3);
        for (int j = 0,  y = 3; j < 10; j++, k++)
        {
            if (mpos_x == x && mpos_y == y)
            {
                mt_setbgcolor(BG_BLUE);
                selected_pos = 10 * i + j;
            }

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
                printf("%X", operand);
            else 
                printf("0%X", operand);

            mt_setbgcolor(0);
            y += 6;   
            mt_gotoXY(x, y);       
        }
    }
}


void clear_input()
{   
    input_x = 27;
    mt_gotoXY(input_x++, input_y);
    for (int i = 0; i < 11; i++) 
    {   
        for (int j = 0; j < 20; j++) printf("  ");
        mt_gotoXY(input_x++, input_y);
    }    
    input_x = 27;
}


void print_term()
{
    print_memory();

    bc_box(4, 65, 20, 3);
    mt_gotoXY(4, 69);
    printf("accumulator");
    mt_gotoXY(5, 72);
    printf("      ");
    mt_gotoXY(5, 72);
    printf("%d", Accumulator);

    bc_box(7, 65, 20, 3);
    mt_gotoXY(7, 66);
    printf("instructionCounter");
    mt_gotoXY(8, 72);
    printf("      ");
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
    print_selected();

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

    if (input_x > 37) clear_input();
    mt_gotoXY(input_x, input_y);
}


void restore_term(void)
{
    if(rk_mytermrestore()) printf("Failed to restore terminal state!\n\n");
}


void signalhandler_reset (int signo)
{
    selected_pos = 0;
    mpos_x = 5;
    mpos_y = 3;
    IC = 0;
    sc_regInit();
    sc_memoryInit();
    signal(SIGUSR1, signalhandler_reset);
}

int CU();

void signalhandler_timer (int signo)
{
    signal(SIGALRM, SIG_IGN);
    CU();
    signal(SIGALRM, signalhandler_timer);
}


int CU()
{
    int memory_val = RAM[IC] ;
    int command_num, operand;    
    nval.it_interval.tv_sec = 1;
    nval.it_interval.tv_usec = 0;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;

    sc_commandDecode(memory_val, &command_num, &operand);
        
    switch(command_num)
    {
        case 10: // READ
            mt_gotoXY(input_x++, input_y);
            rk_mytermrestore();
            printf("New value of RAM[%d]: ", operand);
            fflush(stdout);

            scanf("%d", (RAM + operand));

            rk_mytermregime(OFF, 0, 1, OFF, OFF);
            break;

        case 11: // WRITE
            mt_gotoXY(input_x++, input_y);
            printf("Value of RAM[%d]: ", operand);
            fflush(stdout);

            printf("%d", *(RAM + operand));
            break;

        case 20: // LOAD
            Accumulator = *(RAM + operand);
            break;


        case 21: // STORE
            *(RAM + operand) = Accumulator;
            break;

        default:
            break;    
    }

    signal(SIGALRM, signalhandler_timer);
    setitimer(ITIMER_REAL, &nval, &oval);

    if (IC == 99) IC = 0;
    else IC++;
    return EXIT_SUCCESS;
}


int main()
{
    mt_clrscr();
    rk_mytermsave();
    rk_mytermregime(OFF, 0, 1, OFF, OFF);
    atexit(restore_term);

    //Presets
    sc_commandEncode(10, 10, RAM);
    sc_commandEncode(11, 10, (RAM + 1));
    sc_commandEncode(20, 10, (RAM + 2));
    sc_commandEncode(21, 11, (RAM + 3));
         
    signal(SIGALRM, signalhandler_timer);
    signal(SIGUSR1, signalhandler_reset);
    
    nval.it_interval.tv_sec = 1;
    nval.it_interval.tv_usec = 0;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &nval, &oval);

    int new_ic;
    char ch;
    for(;;)
    {
        sc_regSet(CLOCK_IGNORE, 0);
        
        print_term();
        fflush(stdout);
        read(0, &ch, 1);

        if(ch == 'q') break;

        if (ch == 27)
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();

            read(0, &ch, 1);
            read(0, &ch, 1);
            read(0, &ch, 1);
            __fpurge(stdin);    

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
            setitimer(ITIMER_REAL, &nval, &oval);
            signal(SIGALRM, signalhandler_timer);
        }

        if(ch == 'k')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();
            
            if (mpos_y < 57) mpos_y += 6;

            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);    
        }

        if(ch == 'g')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();
            
            if (mpos_y > 3) mpos_y -= 6;

            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);
            sc_regSet(CLOCK_IGNORE, 0);
        }

        if(ch == 'h')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();
            
            if (mpos_x < 14) mpos_x++;

            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);
            sc_regSet(CLOCK_IGNORE, 0);
        }

        if(ch == 'j')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();
            
            if (mpos_x > 5) mpos_x--;

            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);
            sc_regSet(CLOCK_IGNORE, 0);
        }

        if(ch == 's')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();  

            char filename[20];
            if(mt_gotoXY(input_x++, input_y)) printf("Failed to move to io!\n\n");
            if(rk_mytermrestore()) printf("Failed to restore terminal state!\n\n");
            printf("Save to file: ");
            fflush(stdout);

            scanf("%s", filename);
            sc_memorySave(filename);

            rk_mytermregime(OFF, 0, 1, OFF, OFF);
            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);       
        }


        if(ch == 'l')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();

            char filename[20];
            mt_gotoXY(input_x++, input_y);
            rk_mytermrestore();
            printf("Load file: ");
            fflush(stdout);

            scanf("%s", filename);
            sc_memoryLoad(filename);

            rk_mytermregime(OFF, 0, 1, OFF, OFF);
            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);
        }

        if(ch == 'c')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();

            mt_gotoXY(input_x++, input_y);
            rk_mytermrestore();
            printf("Change to: ");
            fflush(stdout);

            scanf("%d", &(RAM[selected_pos]));
            rk_mytermregime(OFF, 0, 1, OFF, OFF);
            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);
        }

        if(ch == 'i')
        {
            signal(SIGALRM, SIG_IGN);
            sc_regSet(CLOCK_IGNORE, 1);
            print_term();

            raise(SIGUSR1);

            signal(SIGALRM, signalhandler_timer);
            setitimer(ITIMER_REAL, &nval, &oval);
        }

        ch = 0;
        fflush(stdout);
    }

    mt_gotoXY(40, 1);     

    return 0;
}

