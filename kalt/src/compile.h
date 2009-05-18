#ifndef INCLUDE_COMPILE_H
#define INCLUDE_COMPILE_H
#include "parser.h"

typedef enum bytecode_op_type_t
{
    OP_ARRAY_LITERAL, OP_NUMBER_LITERAL, OP_VAR, OP_COPY_VAR, OP_FCALL
} bytecode_op_type, *bytecode_op_type_ptr;

typedef struct bytecode_op_t
{
    bytecode_op_type op;
    union
    {
        int literal_number;
        struct
        {
            int count;
            int *elems;
        } literal_array;
        struct
        {
            int count;
            char **names;
        } parameters;
    };
} bytecode_op, *bytecode_op_ptr;

typedef struct bytecode_program_t
{
    int op_count;
    bytecode_op_ptr opcodes;
} bytecode_program, *bytecode_program_ptr;

bytecode_program compile_program(parse_result parse);
void compile_dump_program(bytecode_program);
void compile_dispose_program(bytecode_program);

#endif
