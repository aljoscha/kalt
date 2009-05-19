#include <stdio.h>
#include <string.h>
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
    /*parse_result parse = parse_tokenize("test = foo(testc=[1,2,3],testc)");
    parse_dump_result(parse);
    syntax_result syn_parse = syntax_check(parse);
    if (syn_parse.success == 1)
    {
        printf("Syntax correct.\n");
    }
    else
    {
        printf("Syntax error: %s\n", syn_parse.error_str);
        parse_dispose_result(parse);
        return 1;
    }
    bytecode_program program = compile_program(parse);
    compile_dump_program(program);
    
    function_list functions = functions_load();
    functions_dump_list(functions);

    functions_dispose_list(functions);   
    compile_dispose_program(program);
    parse_dispose_result(parse);*/
    parse_result parse;
    syntax_result syntax;
    bytecode_program program;
    function_list functions = functions_load();
    kvar_storage_ptr storage = kvar_create_storage();
    char input[MAX_INPUT];
    char *var_name;

    printf("available functions:\n");
    functions_dump_list(functions);

    int running = 1;
    while (running)
    {
        printf("kalt> ");
        gets(input);
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
                    printf("variable: \"%s\"\n", var_name);
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
                
            compile_dispose_program(program);    
            parse_dispose_result(parse);
        }
    }    
    functions_dispose_list(functions);
    kvar_dispose_storage(storage);
    return 0;
}
