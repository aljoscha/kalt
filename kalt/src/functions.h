#ifndef INCLUDE_FUNCTIONS_H
#define INCLUDE_FUNCTIONS_H

#include "kvar.h"
typedef kvar_var_ptr (*function_ptr_def)(int param_count, ...);

typedef struct function_t
{
    char *name;
    function_ptr_def func;
} function, *function_ptr;

typedef struct function_list_t
{
    int function_count;
    function_ptr functions;
    void **libs;
} function_list, *function_list_ptr;

function_list functions_load(void);
void functions_dispose_list(function_list list);
void functions_dump_list(function_list list);

int functions_exists(function_list list, char *name);
kvar_var_ptr functions_execute(function_list list, char *name, int param_count, kvar_store store);
#endif
