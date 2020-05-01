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

typedef struct __SRC_LINE__
{
    char src[SRC_LINE_MAX_LENGTH];

    __int8_t stack_begining_pos;
} src_line;


int compile(char* filename)
{
    FILE* fd = fopen(filename, "r");  
    if (!fd) 
    {
        ERROR_MSG
        printf("can't open file: %s \n", filename);
        return EXIT_FAILURE;
    }

    uint16_t line_num = 0;
    char src[SRC_MAX_LINES][SRC_LINE_MAX_LENGTH];

    while (fgets(src[line_num], SRC_LINE_MAX_LENGTH, fd) != NULL)
    {
        DEBUG_ONLY(printf(" %s\n", src[line_num]);)

        line_num++;
    }

    return EXIT_SUCCESS;
}


int main(int argc, char** argv)
{
    char* filename = argv[1]; 

    compile(filename);
    
    return 0;
}