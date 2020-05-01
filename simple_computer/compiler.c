#include <string.h>
#include "mySimpleComputer.h"

#ifdef DEBUG
#define DEBUG_ONLY(expr) expr
#else
#define DEBUG_ONLY(expr)
#endif

#define ERROR_MSG printf("\033[31;1merror: \033[0m");
#define SRC_MAX_LINES 30
#define SRC_LINE_MAX_LENGTH 50

struct __STACK__
{
    size_t size;      
};

struct __HEAP__
{
    size_t size;      
};

struct __SRC_LINE__
{
    char line[SRC_LINE_MAX_LENGTH];

    uint16_t line_number;           // Started with 1
    char* operand;
    char* args;

    __int8_t stack_begining_pos;
} src_lines[SRC_MAX_LINES];


int check_operand(const char* token, int line_num)
{
    if (!strcmp(token, "REM"))
        return EXIT_SUCCESS;
    if (!strcmp(token, "END"))
        return EXIT_SUCCESS;
    if (!strcmp(token, "LET"))
        return EXIT_SUCCESS;
    if (!strcmp(token, "IF"))
        return EXIT_SUCCESS;
    if (!strcmp(token, "INPUT"))
        return EXIT_SUCCESS;
    if (!strcmp(token, "OUTPUT"))
        return EXIT_SUCCESS;
    if (!strcmp(token, "GOTO"))
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
}


int parse_src(char* filename)
{
    FILE* fd = fopen(filename, "r");  
    if (!fd) 
    {
        ERROR_MSG
        printf("can't open file: %s \n", filename);
        return EXIT_FAILURE;
    }

    uint16_t error_flag = 0;
    uint16_t line_num = 0;

    while (fgets(src_lines[line_num].line, SRC_LINE_MAX_LENGTH, fd) != NULL)
    {
        DEBUG_ONLY(printf(" %s\n", src_lines[line_num].line);)
        char* line = src_lines[line_num].line;
        DEBUG_ONLY(printf(" %s\n", line);)

        char* number = strtok(line, " "); 

        src_lines[line_num].line_number = (int)strtol(number, NULL, 10); 
        if (src_lines[line_num].line_number < 1 || src_lines[line_num].line_number > 30)
        {
            ERROR_MSG
            printf("%s: line %d: misspeled line num %d\n", filename, line_num + 1, src_lines[line_num].line_number);
            error_flag = 1;
        }

        char* operand = strtok(NULL, " ");
        
        if (!check_operand(operand, line_num))
        {
            src_lines[line_num].operand = operand;
            src_lines[line_num].args = line + strlen(number) + strlen(operand) + 2;
        }
        else
        {
            ERROR_MSG
            printf("%s: line %d: unknown operand \033[1m%s\033[0m\n", filename, line_num + 1, operand);
            error_flag = 1;
        }

        line_num++;
    }


    int lines_amount = line_num - 1;
    for (int i = 0; i < lines_amount; i++)
    {
        if (src_lines[i].line_number != src_lines[i + 1].line_number - 1)
        {
            error_flag = 1;
            ERROR_MSG
            printf("%s: lines %d %d: line numbers misorder \033[1m%d %d\033[0m\n",                \
                    filename, i, i + 1, src_lines[i].line_number, src_lines[i + 1].line_number);    \
        }
    }
    

    if (error_flag)
        return -1;
    else    
        return lines_amount;
}



int main(int argc, char** argv)
{
    char* filename = argv[1]; 

    int src_len = parse_src(filename);
    
    if(src_len < 0)
    {
        printf("\nCompilation: \033[31;1mfailed\033[0m\nexit code: %d\n", src_len);
    }
    else
    {
        printf("\nCompilation: \033[32;1msuccess\033[0m\nexit code: 0\n");
        // char* file = strtok(filename, ".");
        // strcat(file, ".sa");
        // if(sc_memorySave(file))
        //     printf("\nfailed to create object file %s", file);
    }
    
        

#ifdef DEBUG
    for (int i = 0; i < 30; i++)
    {
        if(src_lines[i].operand != NULL)
            printf(" Line num: %d\n Operand: %s \n Args: %s\n\n",   \
                    src_lines[i].line_number,                       \
                    src_lines[i].operand,                           \
                    src_lines[i].args);                             \
    }    
#endif

    return 0;
}