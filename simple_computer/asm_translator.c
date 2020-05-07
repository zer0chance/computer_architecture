#include <string.h>
#include "mySimpleComputer.h"

#ifdef DEBUG
#define DEBUG_ONLY(expr) expr
#else
#define DEBUG_ONLY(expr)
#endif

#define LINE_MAX_LENGTH 50
#define ERROR_MSG printf("\033[31;1merror: \033[0m");


int get_command_num(const char* command_name)
{
    // CU commands
    if(!strcmp(command_name, "READ"))
        return 10;   
    if(!strcmp(command_name, "WRITE"))
        return 11;
    if(!strcmp(command_name, "SET"))
        return 12;    
    if(!strcmp(command_name, "LOAD"))
        return 20;  
    if(!strcmp(command_name, "STORE"))
        return 21;      
    if(!strcmp(command_name, "JUMP"))
        return 40; 
    if(!strcmp(command_name, "JNEG"))
        return 41;     
    if(!strcmp(command_name, "JZ"))
        return 42; 
    if(!strcmp(command_name, "HALT"))
        return 43;     
    if(!strcmp(command_name, "JNP"))
        return 55;     
    if(!strcmp(command_name, "JC"))
        return 56; 
    if(!strcmp(command_name, "JNC"))
        return 57;
    if(!strcmp(command_name, "JP"))
        return 58;
    if(!strcmp(command_name, "JNP"))
        return 59;          
    if(!strcmp(command_name, "MOVA"))
        return 71;
    if(!strcmp(command_name, "MOVR"))
        return 71;
    if(!strcmp(command_name, "MOVCA"))
        return 73;               
    if(!strcmp(command_name, "MOVCR"))
        return 74; 

    // ALU commands
    if(!strcmp(command_name, "ADD"))
        return 30;
    if(!strcmp(command_name, "SUB"))
        return 31;
    if(!strcmp(command_name, "DIVIDE"))
        return 32;         
    if(!strcmp(command_name, "MUL"))
        return 33;
    if(!strcmp(command_name, "NOT"))
        return 51;   
    if(!strcmp(command_name, "AND"))
        return 52;    
    if(!strcmp(command_name, "OR"))
        return 53;    
    if(!strcmp(command_name, "XOR"))
        return 54;    
    if(!strcmp(command_name, "CHL"))
        return 60;
    if(!strcmp(command_name, "SHR"))
        return 61;      
    if(!strcmp(command_name, "RCL"))
        return 62;
    if(!strcmp(command_name, "RCR"))
        return 63;        
    if(!strcmp(command_name, "NEG"))
        return 64;    
    if(!strcmp(command_name, "ADDC"))
        return 65;    
    if(!strcmp(command_name, "SUBC"))
        return 66;    
    if(!strcmp(command_name, "LOGICLC"))
        return 67;    
    if(!strcmp(command_name, "LOGICRC"))
        return 68;    
    if(!strcmp(command_name, "RCCL"))
        return 69;     
    if(!strcmp(command_name, "RCCR"))
        return 70;    
    if(!strcmp(command_name, "ADDCС"))
        return 75;    
    if(!strcmp(command_name, "SUBCС"))
        return 76;    

    return 0;
}


int interpreter(char* filename)
{
    FILE* fd = fopen(filename, "r");  
    if (!fd) 
    {
        ERROR_MSG
        printf("can't open file: %s \n", filename);
        return EXIT_FAILURE;
    }

    uint16_t error_flag = 0;
    uint16_t current_line_number = 1;
    char line[LINE_MAX_LENGTH];
    
	while (fgets(line, LINE_MAX_LENGTH, fd) != NULL)
    {
        DEBUG_ONLY(printf("Line: %s\n", line);) 

        char* token = strtok(line, " ");

        int mem_location = (int)strtol(token, NULL, 10); 
        if (mem_location < 0 || mem_location > 99)
        {
            ERROR_MSG
            printf("%s: line %d: invalid memory location %d\n", filename, current_line_number, mem_location);
            error_flag = 1;
        }

        char* command_name = strtok(NULL, " ");

        token = strtok(NULL, " ;");
        int operand = (int)strtol(token, NULL, 10); 

        int command_num = get_command_num(command_name);
        if(command_num)
        {
            DEBUG_ONLY(printf(" RAM pos: %d\n Command: %s\n Operand: %d\n\n",   \
                        mem_location, command_name, operand);)
            if(!error_flag) sc_commandEncode(command_num, operand, (RAM + mem_location));
        }
        else
        {
            ERROR_MSG
            printf("%s: line %d: unknown command \033[1m%s\033[0m\n", filename, current_line_number, command_name);
            error_flag = 1;
        }
        
        current_line_number++;
    }

    fclose(fd);

    if (error_flag)
        return EXIT_FAILURE;
    else    
        return EXIT_SUCCESS;
}


int main(int argc, char** argv)
{
    char* filename = argv[1]; 
    
    sc_memoryInit();
    if(interpreter(filename))
    {
        printf("\nTranslation: \033[31;1mfailed\033[0m\nexit code: 1\n");
    }
    else
    {
        printf("\nTranslation: \033[32;1msuccess\033[0m\nexit code: 0\n");
        char* file = strtok(filename, ".");
        strcat(file, ".o");
        if(sc_memorySave(file))
            printf("\nfailed to create object file %s", file);
    }
    
    return 0;
}
