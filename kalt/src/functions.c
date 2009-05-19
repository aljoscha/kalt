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
        arr[index] = rand(); //my_random();
    }
    result = kvar_create_array(arr, count);
    free(arr);
    return result;
}

kvar_var_ptr dyson_descending(int param_count, kvar_storage_ptr store)
{
    kvar_var_ptr param1;
    kvar_var_ptr result;;
    int count;
    int index;
    int *arr;
    if (param_count != 1)
    {
        printf("descending: usage: descending(values)\n");
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
        arr[index] = count-index-1;
    }
    result = kvar_create_array(arr, count);
    free(arr);
    return result;
}


long int quicksort_p, quicksort_s, quicksort_c;
#define FIRST 1
#define MEDIAN 2
#define RANDOM_MEDIAN 3
int quicksort_pivot_selection_method = 1;
int quicksort(int arr[], int first, int last)
{
    int i, pivot, new_val;
    int m1, m2, m3;
    if (last > first)
    {
        quicksort_c++;
        //Pivot selection
        switch(quicksort_pivot_selection_method)
        {
            case FIRST:
                //Nothing to do because pivot will select first elem
                break;
            case MEDIAN:
                m1 = first;
                m2 = (first+last) / 2;
                m3 = last;
                if ((arr[m1] <= arr[m2] && arr[m2] <= arr[m3]) || (arr[m3] <= arr[m2] && arr[m2] <= arr[m1]))
                    pivot = m2;
                else if ((arr[m2] <= arr[m1] && arr[m1] <= arr[m3]) || (arr[m3] <= arr[m1] && arr[m1] <= arr[m2]))
                    pivot = m1;
                else
                    pivot = m3; 
                if(pivot == m2)
                    ;//printf("selected middle elem\n");
                else
                    printf("not selected middle elem\n");
                new_val = arr[first];
                arr[first] = arr[pivot];
                arr[pivot] = new_val;
                break;
            case RANDOM_MEDIAN:
                m1 = first + (rand() %(last-first+1));
                m2 = first + (rand() %(last-first+1));
                m3 = first + (rand() %(last-first+1));
                if ((arr[m1] <= arr[m2] && arr[m2] <= arr[m3]) || (arr[m3] <= arr[m2] && arr[m2] <= arr[m1]))
                    pivot = m2;
                else if ((arr[m2] <= arr[m1] && arr[m1] <= arr[m3]) || (arr[m3] <= arr[m1] && arr[m1] <= arr[m2]))
                    pivot = m1;
                else
                    pivot = m3; 
                new_val = arr[first];
                arr[first] = arr[pivot];
                arr[pivot] = new_val;
                break;
            default:
                break;
        }
        pivot = first;
        for (i = first+1; i <= last; i++, quicksort_c++)
        {
            new_val = arr[i];
            if (new_val < arr[pivot])
            {   
                quicksort_c++;
                arr[i] = arr[pivot+1];
                arr[pivot+1] = arr[pivot];
                arr[pivot] = new_val;
                quicksort_s+= 1;
                pivot++;
            }
        }
        quicksort(arr, first, pivot-1);
        quicksort(arr, pivot+1, last);
        quicksort_p += 2;
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
    quicksort_p = quicksort_s = quicksort_c = 0;
    quicksort_p = 1;
    quicksort_pivot_selection_method = FIRST; 
    quicksort(arr,0, count-1);
    printf("quicksort variant 1 (select first element as pivot): \n");
    printf("  p: %ld\n", quicksort_p);
    printf("  s: %ld\n", quicksort_s);
    printf("  c: %ld\n", quicksort_c);
    result = kvar_create_array(arr, count);
    return result;
}

kvar_var_ptr quicksort_test_interface(int param_count, kvar_storage_ptr store)
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

    kvar_copy(store,"_temp1", "_temp");

    param1 = kvar_get(store, "_temp1");
    if (param1->type != kvar_type_array)
    {
        printf("param 1 must be an array\n");
        return NULL;
    }
    count = kvar_extract_array_length(param1);
    arr = kvar_extract_array(param1);

    quicksort_p = quicksort_s = quicksort_c = 0;
    quicksort_p = 1;
    quicksort_pivot_selection_method = FIRST; 
    quicksort(arr,0, count-1);
    printf("quicksort variant 1 (select first element as pivot): \n");
    printf("  p: %ld\n", quicksort_p);
    printf("  s: %ld\n", quicksort_s);
    printf("  c: %ld\n", quicksort_c);

    kvar_copy(store, "_temp", "_temp1");
    param1 = kvar_get(store, "_temp1");
    if (param1->type != kvar_type_array)
    {
        printf("param 1 must be an array\n");
        return NULL;
    }
    count = kvar_extract_array_length(param1);
    arr = kvar_extract_array(param1);
    quicksort_p = quicksort_s = quicksort_c = 0;
    quicksort_p = 1;
    quicksort_pivot_selection_method = MEDIAN; 
    quicksort(arr,0, count-1);
    printf("quicksort variant 2 (select median of start median and end as pivot): \n");
    printf("  p: %ld\n", quicksort_p);
    printf("  s: %ld\n", quicksort_s);
    printf("  c: %ld\n", quicksort_c);

    kvar_copy(store, "_temp", "_temp1");
    param1 = kvar_get(store, "_temp1");
    if (param1->type != kvar_type_array)
    {
        printf("param 1 must be an array\n");
        return NULL;
    }
    count = kvar_extract_array_length(param1);
    arr = kvar_extract_array(param1);
    quicksort_p = quicksort_s = quicksort_c = 0;
    quicksort_p = 1;
    quicksort_pivot_selection_method = RANDOM_MEDIAN; 
    quicksort(arr,0, count-1);
    printf("quicksort variant 3 (select median of 3 randoms as pivot): \n");
    printf("  p: %ld\n", quicksort_p);
    printf("  s: %ld\n", quicksort_s);
    printf("  c: %ld\n", quicksort_c);

    result = kvar_create_array(arr, count);
    return result;
}

function_list functions_load(void)
{
    function_list result;

    result.function_count = 5;

    result.functions = malloc(sizeof(function)*result.function_count);

    result.functions[0].name = malloc(strlen("dummy")+1);
    strcpy(result.functions[0].name, "dummy");
    result.functions[0].func = &dummy;

    result.functions[1].name = malloc(strlen("random")+1);
    strcpy(result.functions[1].name, "random");
    result.functions[1].func = &dyson_random;

    result.functions[2].name = malloc(strlen("descending")+1);
    strcpy(result.functions[2].name, "descending");
    result.functions[2].func = &dyson_descending;

    result.functions[3].name = malloc(strlen("quicksort")+1);
    strcpy(result.functions[3].name, "quicksort");
    result.functions[3].func = &quicksort_interface;

    result.functions[4].name = malloc(strlen("quicksorttest")+1);
    strcpy(result.functions[4].name, "quicksorttest");
    result.functions[4].func = &quicksort_test_interface;

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
