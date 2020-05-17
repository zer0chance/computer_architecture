#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef DEBUG
#define DEBUG_ONLY(expr) expr
#else
#define DEBUG_ONLY(expr)
#endif

#define ERROR_MSG printf("\033[31;1merror: \033[0m");
#define SRC_MAX_LINES 30
#define SRC_LINE_MAX_LENGTH 50
#define HEAP_MEMORY_OFFSET 70
#define FRAME_BUFFER_OFFSET 90


#define SET_LINE_NUMBER /*src_lines[i].code_begining_pos = line_num;*/  \
    if (line_num < 10) {                                            \
        result[code.current_pos++] = '0';                           \
        sprintf(&(result[code.current_pos++]), "%d", line_num);     \
    } else {                                                        \
        sprintf(&(result[code.current_pos]), "%d", line_num);       \
        code.current_pos += 2;                                      \
    }                                                               


#define POP_AND_CALCULATE switch (op[op_pos])           \
    {                                                   \
        case '+':                                       \
            num[num_pos - 1] += num[num_pos];           \
            num_pos--;                                  \
            op_pos--;                                   \
            break;                                      \
        case '-':                                       \
            num[num_pos - 1] -= num[num_pos];           \
            num_pos--;                                  \
            op_pos--;                                   \
            break;                                      \
        case '/':                                       \
            num[num_pos - 1] /= num[num_pos];           \
            num_pos--;                                  \
            op_pos--;                                   \
            break;                                      \
        case '*':                                       \
            num[num_pos - 1] *= num[num_pos];           \
            num_pos--;                                  \
            op_pos--;                                   \
            break;                                      \
        default:                                        \
            ERROR_MSG                                   \
            exit(0);                                    \
    }                                                   


#define CHECK_VARIABLE(var) if (strlen(var) > 2)                                                \
    {                                                                                           \
        ERROR_MSG                                                                               \
        printf("%s: line %d: variable name can contain only one character: \033[1m%s\033[0m",   \
                filename, src_lines[i].line_number, var);                                       \
        return EXIT_FAILURE;                                                                    \
    }                                                                                           \
    if (islower(var[0]))                                                                        \
    {                                                                                           \
        ERROR_MSG                                                                               \
        printf("%s: line %d: only uppercase variable names allowed: \033[1m%s\033[0m",          \
                filename, src_lines[i].line_number, var);                                       \
        return EXIT_FAILURE;                                                                    \
    }

struct __code_segment
{
    uint8_t current_pos;

    char variables[20];
    uint8_t current_var_index; 

    char frames[10]; 
    char current_frame;   
} code;


struct __src_lines
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

        char* number = strtok(line, " "); 

        src_lines[line_num].line_number = (int)strtol(number, NULL, 10); 
        if (src_lines[line_num].line_number < 1 || src_lines[line_num].line_number > 30)
        {
            ERROR_MSG
            printf("%s: line %d: misspeled line num: %d\n", filename, line_num + 1, src_lines[line_num].line_number);
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
            printf("%s: line %d: unknown operand: \033[1m%s\033[0m\n", filename, line_num + 1, operand);
            error_flag = 1;
        }

        line_num++;
    }


    int lines_amount = line_num;
    DEBUG_ONLY(printf("\nLINES: %d \n\n", lines_amount);)

    for (int i = 0; i < lines_amount - 1; i++)
    {
        if (src_lines[i].line_number != src_lines[i + 1].line_number - 1)
        {
            error_flag = 1;
            ERROR_MSG
            printf("%s: lines %d %d: line numbers misorder: \033[1m%d %d\033[0m\n",              \
                    filename, i, i + 1, src_lines[i].line_number, src_lines[i + 1].line_number);     
        }
    }
    
    fclose(fd);

    if (error_flag)
        return -1;
    else    
        return lines_amount;
}


int index_of_var(char ch)
{
    for (int i = 0; i < code.current_var_index; i++)
    {
        if (code.variables[i] == ch)
            return i;
    }

    return -1;
}


int isoperation(char op)
{
    switch (op)
    {
        case '+':
            return 1;
        case '-':
            return 1;
        case '/':
            return 1;
        case '*':
            return 1;  
        case '(':
            return 1;
        case ')':
            return 1;                  
        default:
            return 0;
    }
}


int priority(char op) // Returns 1 if operand need to be placed on stack
{
    switch (op)
    {
        case '+':
            return 1;
        case '-':
            return 1;
        case '/':
            return 2;
        case '*':
            return 2;   
        case '(':
            return 0;             
        default:
            return 0;
    }    
}


int handle_expression(char* exp)
{
    char op[20];
    int num[20];

    int op_pos  = -1;
    int num_pos = -1;
    int isNegative = 0;

    if (*exp == '-')
    {
        exp++;
        isNegative = 1;
    }
    
    while(*exp != '\0')
    {
        if (*exp == ' ' || *exp == '\n') {
            exp++;
        } else if(isoperation(*exp)) {
            if (*exp == ')') {
                while (op[op_pos] != '(')
                {
                    POP_AND_CALCULATE
                }
                op[op_pos--] = ' ';    // Delete opening bracket '('
            } else if (op_pos == -1) {
                op[++op_pos] = *exp;
            } else if (*exp == '(') {
                op[++op_pos] = *exp;
            } else if (priority(*exp) > priority(op[op_pos])) {
                op[++op_pos] = *exp;        
            } else {
                POP_AND_CALCULATE
                continue;
            }

            exp++;
        } else if(isdigit(*exp)) {
            num[++num_pos] = strtol(exp, &exp, 10);      
        } else {
            ERROR_MSG
            printf("unknown symbol: \033[1m%c\033[0m", *exp);
            exit(0);
        }
    }

    while (op_pos >= 0)
    {
        POP_AND_CALCULATE
    }

    int result = num[0];

    if(isNegative) 
        return result *= (-1);
    else
        return result;
}


int contain_variable(const char* expr)                  //  0 - not contain
{                                                       //  1 - contain
    for (int i = 0; i < strlen(expr); i++)              // -1 - contain lowercase letter
    { 
        if (expr[i] >= 'A' && expr[i] <= 'Z') {
            return 1;
        }   
        else if (expr[i] >= 'a' && expr[i] <= 'z') {
            return -1;
        }
    }

    return 0;
}


int polish_revert_notation(char* expr, char* result)
{
    char* stack = (char *) calloc(20, sizeof(char));
    int res_pos = 0;
    int stack_pos = 0;

    while (*expr != '\0')
    {
        if (*expr == ' '  || *expr == '\n') {
            expr++;
        } else if (isalpha(*expr)) {
            if (islower(*expr))
            {
                ERROR_MSG                                                                               
                printf("unknown symbol: \033[1m%c\033[0m", *expr);                                    
                return EXIT_FAILURE;  
            } else if (isalpha(*(expr + 1))) {
                ERROR_MSG                                                                               
                printf("unknown symbol: \033[1m%c\033[0m", *expr);                                       
                return EXIT_FAILURE; 
            }
            else
            {
                result[res_pos++] = *expr;
                expr++;
            }
        } else if (isdigit(*expr)) {
            result[res_pos++] = *expr;
            expr++;
        } else {   
            switch (*expr)
            {  
            case '+':
                if (stack_pos == 0) 
                {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else if (stack[stack_pos - 1] == '(') {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else {
                    result[res_pos++] = stack[stack_pos - 1];
                    stack_pos--;
                }
                break;
            case '-':
                if (stack_pos == 0) 
                {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else if (stack[stack_pos - 1] == '(') {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else {
                    result[res_pos++] = stack[stack_pos - 1];
                    stack_pos--;
                }
                break;
            case '/':
                if (stack_pos == 0)
                {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else if (stack[stack_pos - 1] != '*') {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else {
                    result[res_pos++] = stack[stack_pos - 1];
                    stack_pos--;
                }
                break;
            case '*':
                if (stack_pos == 0)
                {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else if (stack[stack_pos - 1] != '/')
                {
                    stack[stack_pos++] = *expr;
                    expr++;
                } else {
                    result[res_pos++] = stack[stack_pos - 1];
                    stack_pos--;
                }
                break;  
            case '(':
                stack[stack_pos++] = *expr;
                expr++;
                break;
            case ')':
                stack_pos--;
                while (stack[stack_pos] != '(')
                {
                    result[res_pos++] = stack[stack_pos];
                    stack_pos--;
                }
                
                expr++;
                break;                  
            default:
                ERROR_MSG
                printf("unknown symbol: \033[1m%c\033[0m", *expr);
                return EXIT_FAILURE; 
            }
        }
    }

    for(int i = stack_pos - 1; i > -1; i--)
    {
        result[res_pos++] = stack[i];
    }
    
    result[res_pos] = '\0';
    printf("Polish notation: %s\n", result);
    return EXIT_SUCCESS;
}


void update_result(char* res, int pos)
{
    DEBUG_ONLY(printf("\nWas: %s", res);)
    char* tmp = (char *) calloc(strlen(res), sizeof(char));

    int i = 0;
    int j = 0;
    while (res[i] != '\0')
    {   
        if (i == pos - 2)
        {
            tmp[j] = code.current_frame - 1;
            i += 3;
            j++;
        }
        else 
        {
            tmp[j] = res[i];
            i++;
            j++;
        }
    }
    tmp[j] = '\0';

    strcpy(res, tmp);

    free(tmp);
    DEBUG_ONLY(printf("\nBecome: %s\n\n", res);)
}


int compile(char* result, int src_len, char* filename)
{
    code.current_pos = 0;
    code.current_var_index = 0;
    code.current_frame = 'a';

    uint16_t line_num = 0;

    for (int i = 0; i < src_len; i++)
    {
        if (!strcmp(src_lines[i].operand, "REM"))
            continue;

        else if (!strcmp(src_lines[i].operand, "END"))
        {
            SET_LINE_NUMBER

            sprintf(&(result[code.current_pos]), "%s", " HALT 0");
            src_lines[i].code_begining_pos = line_num;
            line_num++;
            code.current_pos += 7;
        }
             

        else if (!strcmp(src_lines[i].operand, "INPUT"))
        {
            CHECK_VARIABLE(src_lines[i].args)

            SET_LINE_NUMBER

            int index = index_of_var(src_lines[i].args[0]);

            if (index == -1)   // Not initialized yet
            {
                index = code.current_var_index;
                code.variables[code.current_var_index++] = src_lines[i].args[0];
            }

            sprintf(&(result[code.current_pos]), "%s%d", " READ ", index + HEAP_MEMORY_OFFSET);
            src_lines[i].code_begining_pos = line_num;
            line_num++;
            code.current_pos += 8;
        }    


        else if (!strcmp(src_lines[i].operand, "OUTPUT"))
        {
            CHECK_VARIABLE(src_lines[i].args)

            SET_LINE_NUMBER

            int index = index_of_var(src_lines[i].args[0]);

            if (index == -1)   // Not initialized yet
            {
                ERROR_MSG
                printf("%s: line %d: uninitialized variable: \033[1m%c\033[0m",            \
                        filename, src_lines[i].line_number, src_lines[i].args[0]);         

                return EXIT_FAILURE;
            }

            sprintf(&(result[code.current_pos]), "%s%d", " WRITE ", index + HEAP_MEMORY_OFFSET);
            src_lines[i].code_begining_pos = line_num;
            line_num++;
            code.current_pos += 9;
        }   
        

        else if (!strcmp(src_lines[i].operand, "GOTO"))
        {
            for (int i = 0; i < strlen(src_lines[i].args); i++)
            {
                if (src_lines[i].args[i] < '0' && src_lines[i].args[i] > '9')
                {
                    ERROR_MSG
                    printf("%s: line %d: invalid line number to GOTO : \033[1m%s\033[0m",    \
                            filename, src_lines[i].line_number, src_lines[i].args);          

                    return EXIT_FAILURE;  
                }
            }
            int jump_adress = (int)strtol(src_lines[i].args, NULL, 10); 
            if (jump_adress < 1 || jump_adress > SRC_MAX_LINES)                        
            {                                                              
                ERROR_MSG
                printf("%s: line %d: invalid memory location: %d",                           \
                        filename, src_lines[i].line_number, jump_adress);                    

                return EXIT_FAILURE;        
            }

            SET_LINE_NUMBER

            sprintf(&(result[code.current_pos]), "%s%d", " JUMP ", src_lines[jump_adress - 1].code_begining_pos);
            src_lines[i].code_begining_pos = line_num;
            line_num++;
            code.current_pos += 8;
        }  
        

        else if (!strcmp(src_lines[i].operand, "LET"))
        { 
            char* args = (char *) calloc(strlen(src_lines[i].args), sizeof(char));
            strcpy(args, src_lines[i].args);

            char* variable_name = strtok(args, " ");

            CHECK_VARIABLE(variable_name)

            SET_LINE_NUMBER
            src_lines[i].code_begining_pos = line_num;
            line_num++;
            int index = index_of_var(variable_name[0]);

            if (index == -1)   // Not initialized yet
            {
                index = code.current_var_index;
                code.variables[code.current_var_index++] = variable_name[0];
            }

            char* eq = strtok(NULL, " ");
            if (*eq != '=')
            {
                ERROR_MSG
                printf("%s: line %d: expected '=': \033[1m%s\033[0m", filename, src_lines[i].line_number, eq);                                    \

                return EXIT_FAILURE;
            }

            char* expr = args + 4;

            int chek = contain_variable(expr);
            if(chek == 0)
            {
                int res = handle_expression(expr);
                sprintf(&(result[code.current_pos]), "%s%d", " SET ", res);
                code.current_pos += 5;
                while (res)
                {
                    res /= 10;
                    code.current_pos++;
                } 
                result[code.current_pos++] = '\n';

                SET_LINE_NUMBER

                sprintf(&(result[code.current_pos]), "%s%d", " STORE ", index + HEAP_MEMORY_OFFSET);
                code.current_pos += 9;
                line_num++;
            } 
            else if (chek == 1)
            {
                char* expr_result = (char *) calloc(20, sizeof(char));
                
                if(!polish_revert_notation(expr, expr_result))
                {
                    int pos = 0;
                    while (expr_result[pos] != '\0')
                    {
                        if (!isoperation(expr_result[pos]))
                        {
                            pos++;
                            continue;
                        }
                        
                        if (pos < 2)
                        {
                            ERROR_MSG
                            printf("%s: line %d: wrong expression: \033[1m%s\033[0m", \
                            filename, src_lines[i].line_number, expr);                                    

                            return EXIT_FAILURE;
                        }

                        char val1 = expr_result[pos - 1];
                        char val2 = expr_result[pos - 2];

                        if (isalpha(val1) && isalpha(val2))
                        {
                            int index1 = index_of_var(val1);
                            if (index1 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%c\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }
                            int index2 = index_of_var(val2);
                            if (index2 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%c\033[0m",  \
                                        filename, src_lines[i].line_number, val2);                       

                                return EXIT_FAILURE;
                            }

                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index2 + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            switch (expr_result[pos])
                            {
                            case '+':
                                sprintf(&(result[code.current_pos]), "%s%d", " ADD ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '-':
                                sprintf(&(result[code.current_pos]), "%s%d", " SUB ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '*':
                                sprintf(&(result[code.current_pos]), "%s%d", " MUL ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '/':
                                sprintf(&(result[code.current_pos]), "%s%d", " DIVIDE ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 10;
                                break;

                            default:
                                ERROR_MSG
                                printf("Oh shit man"); // ???????
                                exit(0);
                            }
                            line_num++;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            int new_index = code.current_var_index;
                            code.variables[code.current_var_index++] = code.current_frame++;

                            sprintf(&(result[code.current_pos]), "%s%d", " STORE ", new_index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 9;
                            line_num++;

                            update_result(expr_result, pos);
                            pos = 0;
                        } 
                        else if (isdigit(val1) && isalpha(val2)) 
                        {
                            int index1 = index_of_var(val2);
                            if (index1 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%c\033[0m",  \
                                        filename, src_lines[i].line_number, val2);                       

                                return EXIT_FAILURE;
                            }
                            
                            sprintf(&(result[code.current_pos]), "%s%d", " SET ", val1 - 48);
                            code.current_pos += 6;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            int temp_index = code.current_var_index;
                            code.variables[code.current_var_index++] = code.current_frame++;

                            sprintf(&(result[code.current_pos]), "%s%d", " STORE ", temp_index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 9;
                            line_num++;

                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index1 + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                            line_num++;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER
                            
                            switch (expr_result[pos])
                            {
                            case '*':
                                sprintf(&(result[code.current_pos]), "%s%d", " MUL ", temp_index + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '/':
                                sprintf(&(result[code.current_pos]), "%s%d", " DIVIDE ", temp_index + HEAP_MEMORY_OFFSET);
                                code.current_pos += 10;
                                break;
                            
                            case '+':
                                sprintf(&(result[code.current_pos]), "%s%d", " ADD ", temp_index + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '-':
                                sprintf(&(result[code.current_pos]), "%s%d", " SUB ", temp_index + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            

                            default:
                                ERROR_MSG
                                printf("Oh shit man"); // ???????
                                exit(0);
                            }
                            line_num++;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            int new_index = code.current_var_index;
                            code.variables[code.current_var_index++] = code.current_frame++;

                            sprintf(&(result[code.current_pos]), "%s%d", " STORE ", new_index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 9;
                            line_num++;

                            update_result(expr_result, pos);
                            pos = 0;
                        } 
                        else if (isalpha(val1) && isdigit(val2)) 
                        {
                            int index1 = index_of_var(val1);
                            if (index1 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%c\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }

                            sprintf(&(result[code.current_pos]), "%s%d", " SET ", val2 - 48);
                            code.current_pos += 6;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            switch (expr_result[pos])
                            {
                            case '+':
                                sprintf(&(result[code.current_pos]), "%s%d", " ADD ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '-':
                                sprintf(&(result[code.current_pos]), "%s%d", " SUB ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '*':
                                sprintf(&(result[code.current_pos]), "%s%d", " MUL ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 7;
                                break;

                            case '/':
                                sprintf(&(result[code.current_pos]), "%s%d", " DIVIDE ", index1 + HEAP_MEMORY_OFFSET);
                                code.current_pos += 10;
                                break;

                            default:
                                ERROR_MSG
                                printf("Oh shit man"); // ???????
                                exit(0);
                            }
                            line_num++;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            int new_index = code.current_var_index;
                            code.variables[code.current_var_index++] = code.current_frame++;

                            sprintf(&(result[code.current_pos]), "%s%d", " STORE ", new_index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 9;
                            line_num++;

                            update_result(expr_result, pos);
                            pos = 0;
                        }    
                        else if (isdigit(val1) && isdigit(val2)) 
                        {
                            int dig1 = val2 - 48;
                            int dig2 = val1 - 48;
                            int res;

                            switch (expr_result[pos])
                            {
                            case '+':
                                res = dig1 + dig2;
                                break;

                            case '-':
                                res = dig1 - dig2;
                                break;

                            case '*':
                                res = dig1 * dig2;
                                break;

                            case '/':
                                res = dig1 / dig2;
                                break;

                            default:
                                ERROR_MSG
                                printf("Oh shit man"); // ???????
                                exit(0);
                            }

                            sprintf(&(result[code.current_pos]), "%s%d", " SET ", res);
                            code.current_pos += 6;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            int new_index = code.current_var_index;
                            code.variables[code.current_var_index++] = code.current_frame++;

                            sprintf(&(result[code.current_pos]), "%s%d", " STORE ", new_index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 9;
                            line_num++;

                            update_result(expr_result, pos);
                            pos = 0;
                        }    
                    }

                    if (strlen(expr_result) > 2)
                    {
                        ERROR_MSG
                        printf("%s: line %d: wrong expression: \033[1m%s\033[0m", \
                        filename, src_lines[i].line_number, expr);                                    

                        return EXIT_FAILURE;
                    }
                    else
                    {
                        result[code.current_pos++] = '\n';

                        SET_LINE_NUMBER

                        sprintf(&(result[code.current_pos]), "%s%d", " STORE ", index + HEAP_MEMORY_OFFSET);
                        code.current_pos += 9;
                        line_num++;
                    }
                }    
                else return EXIT_FAILURE;    
            }
            else if (chek == -1)
            {
                ERROR_MSG
                printf("%s: line %d: only uppercase variable names allowed: \033[1m%s\033[0m",        \
                        filename, src_lines[i].line_number, src_lines[i].args);                       

                return EXIT_FAILURE;
            }
        }    


        else if (!strcmp(src_lines[i].operand, "IF"))
        {
            char* args = (char *) calloc(strlen(src_lines[i].args), sizeof(char));
            strcpy(args, src_lines[i].args);

            // Parse condition
            char* val1 = strtok(args, " ");
            char* sign = strtok(NULL, " ");
            char* val2 = strtok(NULL, " ");

            if (isdigit(*val1)) {
                ERROR_MSG
                printf("%s: line %d: can compare onli Var ? Var or Var ? 0: \033[1m%s\033[0m",     \
                        filename, src_lines[i].line_number, src_lines[i].args);                       

                return EXIT_FAILURE;
            } else if (isdigit(*val2) && *val2 != '0') {
                ERROR_MSG
                printf("%s: line %d: can compare onli Var ? Var or Var ? 0: \033[1m%s\033[0m",     \
                        filename, src_lines[i].line_number, src_lines[i].args);                       

                return EXIT_FAILURE;
            } 
            

            char* operation = strtok(NULL, " ");
            
            if(!strcmp(operation, "GOTO"))
            {
                int jump_line = (int)strtol(strtok(NULL, " "), NULL, 10);
                if (jump_line < 0)
                {
                    ERROR_MSG
                    printf("%s: line %d: invalid line to GOTO: \033[1m%d\033[0m",  \
                            filename, src_lines[i].line_number, jump_line);                       

                    return EXIT_FAILURE;
                }

                switch (*sign)
                {
                    case '=':  // v1 - v2 LOAD v1  SUB v2 JZ jump_line       
                        if (*val2 == '0')
                        {
                            CHECK_VARIABLE(val1)
                            int index = index_of_var(*val1);
                            if (index == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }

                            SET_LINE_NUMBER
            
                            src_lines[i].code_begining_pos = line_num;
                            line_num++;
                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " JZ ",  \
                                    src_lines[jump_line - 1].code_begining_pos);

                            if (src_lines[jump_line - 1].code_begining_pos < 10)
                                code.current_pos += 5;
                            else 
                                code.current_pos += 6;   
                            line_num++;
                        }
                        else
                        {
                            CHECK_VARIABLE(val1)
                            CHECK_VARIABLE(val2)
                            int index1 = index_of_var(*val1);
                            if (index1 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }
                            int index2 = index_of_var(*val2);
                            if (index2 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val2);                       

                                return EXIT_FAILURE;
                            }


                            SET_LINE_NUMBER
            
                            src_lines[i].code_begining_pos = line_num;
                            line_num++;
                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index1 + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " SUB ", index2 + HEAP_MEMORY_OFFSET);

                            code.current_pos += 7;
                            line_num++;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " JZ ",  \
                                    src_lines[jump_line - 1].code_begining_pos);

                            if (src_lines[jump_line - 1].code_begining_pos < 10)
                                code.current_pos += 5;
                            else 
                                code.current_pos += 6;   
                        }
                        
                        break;

                    case '>':  // v2 - v1 -> JNEG
                        if (*val2 == '0')
                        {
                            CHECK_VARIABLE(val1)
                            int index = index_of_var(*val1);
                            if (index == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }

                            SET_LINE_NUMBER
            
                            src_lines[i].code_begining_pos = line_num;
                            line_num++;
                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " JNS ",  \
                                    src_lines[jump_line - 1].code_begining_pos);

                           if (src_lines[jump_line - 1].code_begining_pos < 10)
                                code.current_pos += 6;
                            else 
                                code.current_pos += 7;   
                            line_num++;
                        }
                        else
                        {
                            CHECK_VARIABLE(val1)
                            CHECK_VARIABLE(val2)
                            int index1 = index_of_var(*val1);
                            if (index1 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }
                            int index2 = index_of_var(*val2);
                            if (index2 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val2);                       

                                return EXIT_FAILURE;
                            }

                            SET_LINE_NUMBER
            
                            src_lines[i].code_begining_pos = line_num;
                            line_num++;
                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index2 + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " SUB ", index1 + HEAP_MEMORY_OFFSET);

                            code.current_pos += 7;
                            line_num++;

                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " JNEG ",  \
                                    src_lines[jump_line - 1].code_begining_pos);
                            if (src_lines[jump_line - 1].code_begining_pos < 10)
                                code.current_pos += 7;
                            else 
                                code.current_pos += 8;   
                            line_num++;
                        }
                        break;

                    case '<':  // v1 = v2 -> JNEG
                        if (*val2 == '0')
                        {
                            CHECK_VARIABLE(val1)
                            int index = index_of_var(*val1);
                            if (index == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }

                            SET_LINE_NUMBER
            
                            src_lines[i].code_begining_pos = line_num;
                            line_num++;
                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " JNEG ",  \
                                    src_lines[jump_line - 1].code_begining_pos);

                            if (src_lines[jump_line - 1].code_begining_pos < 10)
                                code.current_pos += 7;
                            else 
                                code.current_pos += 8;   
                            line_num++;
                        }
                        else
                        {
                            CHECK_VARIABLE(val1)
                            CHECK_VARIABLE(val2)
                            int index1 = index_of_var(*val1);
                            if (index1 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val1);                       

                                return EXIT_FAILURE;
                            }
                            int index2 = index_of_var(*val2);
                            if (index2 == -1)
                            {
                                ERROR_MSG
                                printf("%s: line %d: uninitialized variable: \033[1m%s\033[0m",  \
                                        filename, src_lines[i].line_number, val2);                       

                                return EXIT_FAILURE;
                            }


                            SET_LINE_NUMBER
            
                            src_lines[i].code_begining_pos = line_num;
                            line_num++;
                            sprintf(&(result[code.current_pos]), "%s%d", " LOAD ", index1 + HEAP_MEMORY_OFFSET);
                            code.current_pos += 8;
                        
                            result[code.current_pos++] = '\n';

                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " SUB ", index2 + HEAP_MEMORY_OFFSET);

                            code.current_pos += 7;
                        
                            result[code.current_pos++] = '\n';

                            line_num++;
                            SET_LINE_NUMBER

                            sprintf(&(result[code.current_pos]), "%s%d", " JNEG ",  \
                                    src_lines[jump_line - 1].code_begining_pos);
                            
                            if (src_lines[jump_line - 1].code_begining_pos < 10)
                                code.current_pos += 7;
                            else 
                                code.current_pos += 8;    

                            line_num++;
                        }
                        break;    
                    default:
                        ERROR_MSG
                        printf("%s: line %d: unknown sign: \033[1m%c\033[0m",  \
                                filename, src_lines[i].line_number, *sign);                       

                        return EXIT_FAILURE;
                }
            }
            else
            {
                ERROR_MSG
                printf("%s: line %d: only GOTO operation able in IF: \033[1m%s\033[0m",  \
                        filename, src_lines[i].line_number, src_lines[i].args);                       

                return EXIT_FAILURE;
            }
              
        }    
  
        result[code.current_pos++] = '\n';
    }

    result[--code.current_pos] = '\0';

    DEBUG_ONLY(printf("\n Translation result:\n%s\n\n", result);)    
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
            printf(" Line num: %d\n Operand: %s \n Args: %s\n",     \
                    src_lines[i].line_number,                       \
                    src_lines[i].operand,                           \
                    src_lines[i].args);                                            
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
        
        if(!compile(code, src_len, filename))
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
                printf("\nfailed to write result to file %s\n", file);

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