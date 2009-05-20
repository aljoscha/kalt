#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "syntax.h"
#include "compile.h"
#include "functions.h"
#include "kvar.h"

#define VERSION 1
#define MAX_INPUT 200

int main(void)
{
    printf("Welcome to kalt - Krettek Algorithmus Tester version %d\n", VERSION);
    printf("  have fun :D\n\n"); 
    parse_result parse;
    syntax_result syntax;
    bytecode_program program;
    function_list functions = functions_load();
    kvar_storage_ptr storage = kvar_create_storage();
    int index;
    int *arr;
    int length;
    char input[MAX_INPUT];
    char *var_name;
    kvar_var_ptr kvar;

    printf("available functions:\n");
    functions_dump_list(functions);

    int running = 1;
    while (running)
    {
        printf("kalt> ");
        gets(input);
        //fgets(input, MAX_INPUT, stdin);
        printf("input: %s\n", input);
        if (input[0] == ':')
        {
            switch (input[1])
            {
                case '?':
                    printf(":? print this help\n");
                    printf(":q quit\n");
                    printf(":s show value of variable\n");
                    break;
                case 'q':
                    running = 0;
                    break;
                case 's':
                    var_name = strtok(&input[2]," ");    
                    if (!kvar_exists(storage, var_name))
                    {
                        printf("%s is not defined\n", var_name);
                    }
                    kvar = kvar_get(storage, var_name);
                    if (kvar->type == kvar_type_number)
                    {
                        printf("%s = %d\n", var_name, kvar_extract_number(kvar));
                    }
                    else if (kvar->type == kvar_type_array)
                    {
                        arr = kvar_extract_array(kvar);
                        length = kvar_extract_array_length(kvar);
                        printf("%s = [", var_name);
                        for(index = 0; index < length-1; index++)
                        {
                            printf("%d,", arr[index]);
                        }
                        printf("%d]\n", arr[index]);
                    }
                    break;
                default:
                    printf("unrecognized command, :? for help\n");
            }
        }
        else
        {
            //we have a kcode program
            parse = parse_tokenize(input);
            if (parse.success == 0)
            {
                printf("parse not successfull\n");
                parse_dispose_result(parse);
                continue;
            }
            syntax = syntax_check(parse);
            if (syntax.success == 1)
            {
                printf("kcode syntax correct\n");
            }
            else
            {
                printf("%s\n", syntax.error_str);
                parse_dispose_result(parse);
                continue;
            }
            program = compile_program(parse);
            compile_dump_program(program);
            vm_execute(program, functions, storage);    
            compile_dispose_program(program);    
            parse_dispose_result(parse);
        }
        //free(input); // readline uses malloc so we have to do this
    }    
    functions_dispose_list(functions);
    kvar_dispose_storage(storage);
    return 0;
}
