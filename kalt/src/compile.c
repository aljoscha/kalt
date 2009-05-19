#include "compile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_OPCODES 100

int create_array(parse_result parse, int** arr)
{
    int index;
    int count = 0;
    for(index = 0; index < parse.token_count; index++)
    {
        if(parse.tokens[index].type == TOKEN_NUMBER)
            count++;
    }
    count = 0;
    (*arr) = malloc(sizeof(int)*count);
    for(index = 0; index < parse.token_count; index++)
    {
        if(parse.tokens[index].type == TOKEN_NUMBER)
        {
            (*arr)[count] = atoi(parse.tokens[index].text);
            count++;
        }
    }
    return count;
}

bytecode_program _compile_program(parse_result parse, int first_temp)
{
    int index;
    bytecode_program result;
    bytecode_op ops[MAX_OPCODES];
    int num_opcodes = 0;
    parse_result subset;

    switch(parse.tokens[0].type)
    {
        case TOKEN_NUMBER:
            {
                ops[num_opcodes].op = OP_NUMBER_LITERAL;
                ops[num_opcodes].literal_number = atoi(parse.tokens[0].text);
                num_opcodes++;
            } break;
        case TOKEN_LSPAREN:
            {
                ops[num_opcodes].op = OP_ARRAY_LITERAL;
                ops[num_opcodes].literal_array.count = create_array(parse, &ops[num_opcodes].literal_array.elems);
                num_opcodes++;  
            } break;
        case TOKEN_IDENTIFIER:
            {
                if (parse.token_count == 1) // a single variable, so OP_VAR which puts the variable value in _result
                {
                    ops[num_opcodes].op = OP_VAR;
                    ops[num_opcodes].parameters.count = 1;
                    ops[num_opcodes].parameters.names = malloc(sizeof(char*));
                    ops[num_opcodes].parameters.names[0] = malloc(strlen(parse.tokens[0].text)+1);
                    strcpy(ops[num_opcodes].parameters.names[0], parse.tokens[0].text);
                    num_opcodes++;
                }  
                else if(parse.tokens[1].type == TOKEN_ASSIGN)
                {
                    bytecode_program intermediate_program;
                    parse_result parse_subset = parse_create_subset(parse, 2, parse.token_count-1);
                    intermediate_program = _compile_program(parse_subset, 0);
                    memcpy(ops, intermediate_program.opcodes, sizeof(bytecode_op)*intermediate_program.op_count);
                    free(intermediate_program.opcodes);
                    num_opcodes+=intermediate_program.op_count;

                    ops[num_opcodes].op = OP_COPY_VAR;
                    ops[num_opcodes].parameters.count = 2;
                    ops[num_opcodes].parameters.names = malloc(sizeof(char*)*2);
                    ops[num_opcodes].parameters.names[0] = malloc(strlen("_result")+1);
                    ops[num_opcodes].parameters.names[1] = malloc(strlen(parse.tokens[0].text)+1);
                    strcpy(ops[num_opcodes].parameters.names[0], "_result");
                    strcpy(ops[num_opcodes].parameters.names[1], parse.tokens[0].text);
                    num_opcodes++;
                }
                else if(parse.tokens[1].type == TOKEN_LPAREN) // A function call, we assume that the syntax is correct
                {
                    int num_params = 0;
                    if (parse.tokens[2].type == TOKEN_RPAREN)
                    {
                    }
                    else
                    {
                        int current_end = 2;
                        int current_start = 2;
                        int paren_depth = 0;
                        int running = 1;
                        while(current_end < parse.token_count && running)
                        {
                            switch(parse.tokens[current_end].type)
                            {
                                case TOKEN_LPAREN:
                                    {
                                        paren_depth++;    
                                    } break;
                                case TOKEN_LSPAREN:
                                    {
                                        paren_depth++;    
                                    } break;
                                case TOKEN_RSPAREN:
                                    {
                                        paren_depth--;    
                                    } break;
                                case TOKEN_RPAREN:
                                    {
                                        if (paren_depth == 0)
                                        {
                                            // The final parameter of the function call
                                        }
                                        else
                                        {
                                            paren_depth--;
                                            break;
                                        }
                                    }; 
                                case TOKEN_COMMA: // THis will handle the above case too
                                    {
                                        if (paren_depth == 0)
                                        {
                                            // One parameter of the function call 
                                            num_params++;
                                            bytecode_program intermediate_program;
                                            parse_result parse_subset = parse_create_subset(parse, current_start, current_end-1);
                                            intermediate_program = _compile_program(parse_subset, num_params);
                                            memcpy(&ops[num_opcodes], intermediate_program.opcodes, sizeof(bytecode_op)*intermediate_program.op_count);
                                            free(intermediate_program.opcodes);
                                            num_opcodes+=intermediate_program.op_count;

                                            //copy the _result to the temp_var
                                            ops[num_opcodes].op = OP_COPY_VAR;
                                            ops[num_opcodes].parameters.count = 2;
                                            ops[num_opcodes].parameters.names = malloc(sizeof(char*)*2);
                                            ops[num_opcodes].parameters.names[0] = malloc(strlen("_result")+1);
                                            ops[num_opcodes].parameters.names[1] = malloc(strlen("_temp   ")+1);
                                            strcpy(ops[num_opcodes].parameters.names[0], "_result");
                                            sprintf(ops[num_opcodes].parameters.names[1], "_temp%d", num_params);
                                            num_opcodes++;
                                            current_start = current_end+1;
                                        
                                        }
                                    } break;
                            }
                            current_end++;
                        }
                    }
                    ops[num_opcodes].op = OP_FCALL;
                    ops[num_opcodes].parameters.count = num_params+1; // +1 for the function name
                    ops[num_opcodes].parameters.names = malloc(sizeof(char*)*(num_params+1));
                    ops[num_opcodes].parameters.names[0] = malloc(strlen(parse.tokens[0].text)+1);
                    strcpy(ops[num_opcodes].parameters.names[0], parse.tokens[0].text);
                    for(index = 0; index < num_params; index++)
                    {
                        ops[num_opcodes].parameters.names[index+1] = malloc(strlen("_temp   ")+1);
                        sprintf(ops[num_opcodes].parameters.names[index+1], "_temp%d", index+1);
                    }
                    num_opcodes++;
                }
            } break;
    }
    result.op_count = num_opcodes;
    result.opcodes = malloc(sizeof(bytecode_op)*num_opcodes);
    memcpy(result.opcodes, ops, sizeof(bytecode_op)*num_opcodes);
    return result;
}

void compile_dispose_program(bytecode_program program)
{
    int index;
    int index2;
    for(index = 0; index < program.op_count; index++)
    {
        switch(program.opcodes[index].op)
        {
            case OP_ARRAY_LITERAL:
                {
                    free(program.opcodes[index].literal_array.elems);
                } break;
            case OP_VAR:
                {
                    for (index2 = 0; index2 < program.opcodes[index].parameters.count; index2++)
                    {
                        free(program.opcodes[index].parameters.names[index2]);
                    }
                    free(program.opcodes[index].parameters.names);
                } break;
            case OP_COPY_VAR:
                {
                    for (index2 = 0; index2 < program.opcodes[index].parameters.count; index2++)
                    {
                        free(program.opcodes[index].parameters.names[index2]);
                    }
                    free(program.opcodes[index].parameters.names);
                } break;
            case OP_FCALL:
                {
                    for (index2 = 0; index2 < program.opcodes[index].parameters.count; index2++)
                    {
                        free(program.opcodes[index].parameters.names[index2]);
                    }
                    free(program.opcodes[index].parameters.names);
                } break;
        }
    }
    free(program.opcodes);
}

bytecode_program compile_program(parse_result parse)
{
    return _compile_program(parse,0); 
}

void compile_dump_program(bytecode_program program)
{
    printf("Dumping bytecode program...\n");
    int index, index2;
    for (index = 0; index < program.op_count; index++)
    {
        bytecode_op op = program.opcodes[index];
        switch (op.op)
        {
            case OP_NUMBER_LITERAL:
                {
                    printf("OP_NUMBER_LITERAL %d\n", op.literal_number);
                } break;
            case OP_ARRAY_LITERAL:
                {
                    printf("OP_ARRAY_LITERAL [");
                    for(index2 = 0; index2 < op.literal_array.count-1; index2++)
                    {
                        printf("%d,", op.literal_array.elems[index2]);
                    }
                    printf("%d]\n",op.literal_array.elems[index2]);
                } break;
            case OP_VAR:
                {
                    printf("OP_VAR %s\n", op.parameters.names[0]);
                } break;
            case OP_COPY_VAR:
                {
                    printf("OP_COPY_VAR %s %s\n", op.parameters.names[0], op.parameters.names[1]);
                } break;
            case OP_FCALL:
                {
                    printf("OP_FCALL ");
                    for(index2 = 0; index2 < op.parameters.count-1; index2++)
                    {
                        printf("%s,", op.parameters.names[index2]);
                    }
                    printf("%s\n",op.parameters.names[index2]);
                } break;
        } 
    }
}
