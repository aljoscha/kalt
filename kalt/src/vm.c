#include "vm.h"

#include <stdio.h>
#include <stdlib.h>

//#define DEBUG 0

#ifdef DEBUG
#define DOUT(text) (printf("%s",text))
#else
#define DOUT(text) 
#endif


int vm_execute(bytecode_program program, function_list functions, kvar_storage_ptr storage)
{
    int index;
    char *fname;
    int param_count;
    kvar_var_ptr kvar;
    for (index = 0; index < program.opcode_count; index++)
    {
        switch (program.opcodes[index].op)
        {
            case OP_NUMBER_LITERAL: // put the number in _result
                DOUT("vm: OP_NUMBER_LITERAL\n");
                kvar = kvar_create_number(program.opcodes[index].number_literal);
                kvar_set(storage, "_result", kvar);
                break;

            case OP_ARRAY_LITERAL: // put the array in _result
                DOUT("vm: OP_ARRAY_LITERAL\n");
                kvar = kvar_create_array(program.opcodes[index].array_literal.elems, program.opcodes[index].array_literal.count);
                kvar_set(storage, "_result", kvar);
                break;
            case OP_COPY_VAR: // copy src to dest
                DOUT("vm: OP_COPY_VAR\n");
                kvar_copy(storage, program.opcodes[index].parameters.names[0], program.opcodes[index].parameters.names[1]);
                break;
            case OP_VAR: // copy the var to _result
                DOUT("vm: OP_VAR\n");
                if (!kvar_exists(storage, program.opcodes[index].parameters.names[0]))
                {
                    printf("variable %s does not exist, aborting execution\n", program.opcodes[index].parameters.names[0]);
                    return 0;
                }
                kvar_copy(storage, program.opcodes[index].parameters.names[0], "_result");
                break;
            case OP_FCALL: // call the function and put the result in _result kvar
                DOUT("vm: OP_FCALL\n");
                fname = program.opcodes[index].parameters.names[0];
                param_count = program.opcodes[index].parameters.count;
                param_count--;
                printf("about to call %s with %d parameters\n", fname, param_count);
                if (!functions_exists(functions, fname))
                {
                    printf("function %s does not exist, aborting execution\n", fname);
                    return 0;
                }
                kvar = functions_execute(functions, fname, param_count, storage);
                if (kvar == NULL)
                {
                    printf("aborting execution\n");
                    return 0;
                }
                kvar_set(storage, "_result", kvar);
                break;
        }   
    }
    return 1;
}
