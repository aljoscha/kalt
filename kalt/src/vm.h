#ifndef INCLUDE_VM_H
#define INCLUDE_VM_H

#include "compile.h"
#include "functions.h"
#include "kvar.h"

int vm_execute(bytecode_program program, function_list functions, kvar_storage_ptr storage);

#endif
