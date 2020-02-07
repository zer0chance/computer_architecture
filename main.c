#include "mySimpleComputer.h"

void print_memory()
{
	printf("\n\nRAM: ");
    for (int i = 0; i < RAM_MAX_SIZE; i++) printf("%d ", RAM[i]);   
}


void print_flags()
{
	printf("\n\nWRONG_OPCODE:     %d\n", FLAGS & 1);
	printf("CLOCK_IGNORE:     %d\n", (FLAGS >> 1) & 1);
	printf("MEM_OUT_OF_BOUND: %d\n", (FLAGS >> 2) & 1);
	printf("ZERO_DIVISION:    %d\n", (FLAGS >> 3) & 1);
	printf("OP_OVERFLOW:      %d", (FLAGS >> 4) & 1);
}


int main()
{
	// sc_memoryLoad("Memory.dat");
	// print_memory();
	sc_memoryInit();
	sc_regInit();
	if (sc_memorySet(10, 9)) printf("\nmemory out of bound\n");
	if (sc_memorySet(99, 34)) printf("\nmemory out of bound\n");
	if (sc_memorySet(333, 1)) printf("\nmemory out of bound\n");

	if (sc_regSet(ZERO_DIVISION, 1)) printf("\nflag error\n");
	if (sc_regSet(CLOCK_IGNORE, 1)) printf("\nflag error\n");
	if (sc_regSet(0x9, 1)) printf("\nflag error\n");
	if (sc_regSet(ZERO_DIVISION, 0)) printf("\nflag error\n");

	int flag;
	if (sc_regGet(CLOCK_IGNORE, &flag)) printf("\nflag error\n");
	else printf("\nValue of flag CLOCK_IGNORE is: %d", flag);
	if (sc_regGet(0x3, &flag)) printf("\nflag error\n");
	else printf("\nValue of flag ------ is: %d", flag);
	if (sc_regGet(WRONG_OPCODE, &flag)) printf("\nflag error\n");
	else printf("\nValue of flag WRONG_OPCODE is: %d", flag);


	int value;
	if (sc_memoryGet(100, &value)) printf("\nmemory out of bound\n");
	if (sc_memoryGet(10, &value)) printf("\nmemory out of bound\n");


	if (sc_memorySave("Memory.dat")) printf("\ncan`t save memory\n");

	print_memory();
	print_flags();

	printf("\n\nValue of &10: %d\n\n", value);

    return EXIT_SUCCESS;       
}	
