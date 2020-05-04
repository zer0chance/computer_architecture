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

struct __code_segment
{
    __uint8_t current_pos;
    size_t size;      
} code;

struct __heap
{

    size_t size;      
} heap;

struct __SRC_LINE__
{
    char line[SRC_LINE_MAX_LENGTH];

    uint16_t line_number;           // Started with 1
    char* operand;
    char* args;

    uint16_t code_begining_pos;
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


    int lines_amount = line_num;
    DEBUG_ONLY(printf("\n\nLINES: %d \n\n", lines_amount);)

    for (int i = 0; i < lines_amount - 1; i++)
    {
        if (src_lines[i].line_number != src_lines[i + 1].line_number - 1)
        {
            error_flag = 1;
            ERROR_MSG
            printf("%s: lines %d %d: line numbers misorder \033[1m%d %d\033[0m\n",                  \
                    filename, i, i + 1, src_lines[i].line_number, src_lines[i + 1].line_number);    \
        }
    }
    
    fclose(fd);

    if (error_flag)
        return -1;
    else    
        return lines_amount;
}


char* compile(char* result, int src_len)
{
    //char result[1000]; 
    // result = (char *) calloc(1000, sizeof(char));
    code.current_pos = 0;
    uint16_t line_num = 0;

    for (int i = 0; i < src_len; i++)
    {
        if (!strcmp(src_lines[i].operand, "REM"))
            continue;

        else if (!strcmp(src_lines[i].operand, "END"))
        {
            if (line_num < 10) {
                result[code.current_pos++] = '0';
                sprintf(&(result[code.current_pos++]), "%d", line_num++);
            } else {
                sprintf(&(result[code.current_pos]), "%d", line_num++);
                code.current_pos += 2;
            }

            sprintf(&(result[code.current_pos]), "%s", " HALT");
        }
            
        else if (!strcmp(src_lines[i].operand, "LET"))
        {
            
        }    

        else if (!strcmp(src_lines[i].operand, "IF"))
        {
            
        }    

        else if (!strcmp(src_lines[i].operand, "INPUT"))
        {
            
        }    

        else if (!strcmp(src_lines[i].operand, "OUTPUT"))
        {
            
        }    

        else if (!strcmp(src_lines[i].operand, "GOTO"))
        {
            
        }    
    }

    printf("\n Result: %s\n\n", result);    
    return EXIT_SUCCESS;
}


int main(int argc, char** argv)
{
    char* filename = argv[1]; 

    int src_len = parse_src(filename);
    
#ifdef DEBUG
    for (int i = 0; i < src_len; i++)
    {
        if(src_lines[i].operand != NULL)
            printf(" Line num: %d\n Operand: %s \n Args: %s\n\n",   \
                    src_lines[i].line_number,                       \
                    src_lines[i].operand,                           \
                    src_lines[i].args);                             \
    }    
#endif

    if(src_len < 0) {
        printf("\nCompilation: \033[31;1mfailed\033[0m\nexit code: %d\n", src_len);
        return 0;
    } else if (strcmp(src_lines[src_len - 1].operand, "END") != 0) {
        ERROR_MSG
        printf("%s: program must ends up with \"END\" operator\n", filename);
        printf("\nCompilation: \033[31;1mfailed\033[0m\nexit code: %d\n", -2);
        return 0;
    }
    else
    {
        char* code = (char *) calloc(1000, sizeof(char));
        
        if(!compile(code, src_len))
        {
            printf("\nCompilation: \033[32;1msuccess\033[0m\nexit code: 0\n");
            char* file = strtok(filename, ".");
            strcat(file, ".sa");
            
            FILE* res = fopen(file, "w");
            if (!res) 
            {
                ERROR_MSG
                printf("can't open file: %s \n", file);
                return EXIT_FAILURE;
            }

            if(fputs(code, res) == EOF)
                printf("\nfailed to write result to file %s", file);

            fclose(res);    
        }
        else 
        {
            printf("\nCompilation: \033[31;1mfailed\033[0m\nexit code: %d\n", -3);
            return 0;
        } 
        
        free(code);           
    }

    return 0;
}