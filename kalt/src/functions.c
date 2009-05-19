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

kvar_var_ptr dyson_random(int param_count, kvar_storage_ptr store)
{
    kvar_var_ptr param1;
    kvar_var_ptr result;;
    int count;
    int index;
    int *arr;
    if (param_count != 1)
    {
        printf("random: usage: random(values), random returns values values\n");
        return NULL;
    }
    param1 = kvar_get(store, "_temp1");
    if (param1->type != kvar_type_number)
    {
        printf("param 1 must be a number\n");
        return NULL;
    }
    count = kvar_extract_number(param1);
    arr = malloc(sizeof(int)*count);
    for (index = 0; index < count; index++)
    {
        arr[index] = my_random();
    }
    result = kvar_create_array(arr, count);
    free(arr);
    return result;
}

int quicksort(int arr[], int first, int last)
{
    int i, pivot, new_val;
    if (last > first)
    {
        pivot = first;
        for (i = first+1; i <= last; i++)
        {
            new_val = arr[i];
            if (new_val < arr[pivot])
            {   
                arr[i] = arr[pivot+1];
                arr[pivot+1] = arr[pivot];
                arr[pivot] = new_val;
                pivot++;
            }
        }
        printf("quicksort: recursing\n");
        quicksort(arr, first, pivot-1);
        quicksort(arr, pivot+1, last);
    }
    return 1;
}

kvar_var_ptr quicksort_interface(int param_count, kvar_storage_ptr store)
{
    kvar_var_ptr param1;
    kvar_var_ptr result;;
    int count;
    int *arr;
    if (param_count != 1)
    {
        printf("quicksort: usage: quicksort(array)\n");
        return NULL;
    }
    param1 = kvar_get(store, "_temp1");
    if (param1->type != kvar_type_array)
    {
        printf("param 1 must be an array\n");
        return NULL;
    }
    count = kvar_extract_array_length(param1);
    arr = kvar_extract_array(param1);
    printf("calling quicksort, first: %d last: %d\n", 0, count-1);
    quicksort(arr,0, count-1);
    result = kvar_create_array(arr, count);
    return result;
}

function_list functions_load(void)
{
    function_list result;

    result.function_count = 3;

    result.functions = malloc(sizeof(function)*result.function_count);

    result.functions[0].name = malloc(strlen("dummy")+1);
    strcpy(result.functions[0].name, "dummy");
    result.functions[0].func = &dummy;

    result.functions[1].name = malloc(strlen("random")+1);
    strcpy(result.functions[1].name, "random");
    result.functions[1].func = &dyson_random;

    result.functions[2].name = malloc(strlen("quicksort")+1);
    strcpy(result.functions[2].name, "quicksort");
    result.functions[2].func = &quicksort_interface;
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
