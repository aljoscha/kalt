#include "functions.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


kvar_var_ptr dummy(int param_count, kvar_storage_ptr store)
{
    printf("called dummy with %d params\n", param_count);
    int nums[10] = {0,1,2,3,4,5,6,7,8,9};
    return kvar_create_array(nums,10);
}

int my_random(void)
{
    static int x = 0;
    x = (5*x+7) % 1024;
    return x;
}

function_list functions_load(void)
{
    function_list result;

    result.function_count = 1;

    result.functions = malloc(sizeof(function)*result.function_count);
    result.functions[0].name = malloc(strlen("dummy")+1);
    strcpy(result.functions[0].name, "dummy");
    result.functions[0].func = &dummy;

    return result;
}

void functions_dispose_list(function_list list)
{
    if (list.functions == NULL)
      return;
    int index;
    for (index = 0; index < list.function_count; index++)
    {
        free(list.functions[index].name);
    }
    free(list.functions);
    list.functions = NULL;
}

void functions_dump_list(function_list list)
{
    if (list.functions == NULL)
    {
        printf("Function list was not initialized\n");
        return;
    }
    printf("Dumping function list...\n");
    int index;
    for (index = 0; index < list.function_count; index++)
    {
        printf("%s\n", list.functions[index].name);
    }
}


int functions_exists(function_list list, char *name)
{
    if (list.functions == NULL)
    {
        printf("Function list was not initialized\n");
        return;
    }
    int index;
    for (index = 0; index < list.function_count; index++)
    {
        if (strcmp(list.functions[index].name, name) == 0)
            return 1;
    }
    return 0;
}

kvar_var_ptr functions_execute(function_list list, char *name, int param_count, kvar_storage_ptr store)
{
    if (list.functions == NULL)
    {
        printf("Function list was not initialized\n");
        return;
    }
    int index;
    for (index = 0; index < list.function_count; index++)
    {
        if (strcmp(list.functions[index].name, name) == 0)
        {
            return list.functions[index].func(param_count, store);
        }
    }
    return NULL;
}
