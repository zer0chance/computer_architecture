#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RAM_MAX_SIZE 100
#define COMMAND 0x8000      // 1 0 0000000 0000000
#define COMMAND_NUM 0x3f80  //     1111111 0000000 
#define OPERAND 0x7f        //             1111111

uint8_t FLAGS;
int16_t RAM[RAM_MAX_SIZE];

enum
{
    WRONG_OPCODE = 0x1,	
    CLOCK_IGNORE = 0x2,	
    MEM_OUT_OF_BOUND = 0x4,
    ZERO_DIVISION = 0x8,
    OP_OVERFLOW = 0x10
};   


void sc_memoryInit()
{
    for (int i = 0; i < RAM_MAX_SIZE; i++) RAM[i] = 0;
}


int sc_memorySet(uint16_t adress, int value)
{
    if (adress >= RAM_MAX_SIZE) 
    {
	FLAGS |= MEM_OUT_OF_BOUND;
	return EXIT_FAILURE;
    } 
    else 
    {
	RAM[adress] = value;
	return EXIT_SUCCESS;
    }
}    


int sc_memoryGet(uint16_t adress, int* value)
{
    if (adress >= RAM_MAX_SIZE) 
    {
	FLAGS |= MEM_OUT_OF_BOUND;
	return EXIT_FAILURE;
    } 
    else 
    {
	*value = RAM[adress];
	return EXIT_SUCCESS;
    }
}    


int sc_memorySave(char* filename)
{
    FILE* file;
    file = fopen(filename, "wb");
    if (file == NULL)
    {
	printf("Can`t open file");
        return EXIT_FAILURE;
    }
    else 
    {    
        fwrite(RAM, sizeof(RAM), 1, file);
	return EXIT_SUCCESS;
    }
}


int sc_memoryLoad(char* filename)
{
    FILE* file;
    file = fopen(filename, "rb");
    if (file == NULL)
    {
	printf("Can`t open file");
        return EXIT_FAILURE;
    }
    else 
    {    
        fread(RAM, sizeof(RAM), 1, file);
	return EXIT_SUCCESS;
    }
}


void sc_regInit()
{
   FLAGS &= 0;
}


int sc_regSet(int reg, int value)
{
    switch (value) {
        case 1:
            switch(reg) {
                case WRONG_OPCODE:
                    FLAGS |= WRONG_OPCODE;
                    return EXIT_SUCCESS;
                case CLOCK_IGNORE:  
                    FLAGS |= CLOCK_IGNORE;
                    return EXIT_SUCCESS;
                case MEM_OUT_OF_BOUND:
                    FLAGS |= MEM_OUT_OF_BOUND;
                    return EXIT_SUCCESS;
                case ZERO_DIVISION:
                    FLAGS |= ZERO_DIVISION;
                    return EXIT_SUCCESS;
                case OP_OVERFLOW:   
                    FLAGS |= OP_OVERFLOW;
                    return EXIT_SUCCESS;
                default:
                    return EXIT_FAILURE;
            }        
        case 0:
            switch(reg) {
                case WRONG_OPCODE:
                    FLAGS &= ~1;
                    return EXIT_SUCCESS;
                case CLOCK_IGNORE:  
                    FLAGS &= ~(1 << 1);
                    return EXIT_SUCCESS;
                case MEM_OUT_OF_BOUND:
                    FLAGS &= ~(1 << 2);
                    return EXIT_SUCCESS;
                case ZERO_DIVISION:
                    FLAGS &= ~(1 << 3);
                    return EXIT_SUCCESS;
                case OP_OVERFLOW:   
                    FLAGS &= ~(1 << 4);
                    return EXIT_SUCCESS;
                default:
                    return EXIT_FAILURE;  
            }      
        default:
            return EXIT_FAILURE;                     
    }	    
}


int sc_regGet(int reg, int* value)
{
    switch(reg) {
	case WRONG_OPCODE:
	    *value = FLAGS & WRONG_OPCODE;
	    return EXIT_SUCCESS;
	case CLOCK_IGNORE:  
	    *value = (FLAGS & CLOCK_IGNORE) >> 1;
	    return EXIT_SUCCESS;
	case MEM_OUT_OF_BOUND:
	    *value = (FLAGS & MEM_OUT_OF_BOUND) >> 2;
	    return EXIT_SUCCESS;
	case ZERO_DIVISION:
	    *value = (FLAGS & ZERO_DIVISION) >> 3;
	    return EXIT_SUCCESS;
	case OP_OVERFLOW:   
	    *value = (FLAGS & OP_OVERFLOW) >> 4;
	    return EXIT_SUCCESS;
	default:
    	return EXIT_FAILURE;
    }
}


int sc_commandEncode(int command_num, int operand, int* value) // TODO: handle exeptions
{
    *value = (COMMAND | (command_num << 7)) | operand;
    return EXIT_SUCCESS;
}


int sc_commandDecode(int value, int* command_num, int* operand) // TODO: handle exeptions
{
    *command_num = (value & COMMAND_NUM) >> 7;
    *operand = value & OPERAND;
    return EXIT_SUCCESS;
}



	
