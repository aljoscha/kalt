#include "functions.h"
#include "test_results.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// For file descriptors
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



kvar_var_ptr dummy(int param_count, kvar_storage_ptr store)
{
    printf("called dummy with %d params\n", param_count);
    int nums[10] = {0,1,2,3,4,5,6,7,8,9};
    return kvar_create_array(nums,10);
}

int dyson_rand(void)
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
        arr[index] = dyson_rand();
    }
    result = kvar_create_array(arr, count);
    free(arr);
    return result;
}

kvar_var_ptr c_random(int param_count, kvar_storage_ptr store)
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
        arr[index] = rand();
    }
    result = kvar_create_array(arr, count);
    free(arr);
    return result;
}


kvar_var_ptr urandom_random(int param_count, kvar_storage_ptr store)
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
    int file = open("/dev/urandom",O_RDONLY);
    for (index = 0; index < count; index++)
    {
        read(file, &arr[index], sizeof(int));
        arr[index] = rand(); //my_random();
    }
    close(file);
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
int bubblesort(int arr[], int first, int last)
{
    int index, temp;
    int running = 1;
    while (running && last >= 0) // the second comparison is not counted
    {
        quicksort_c++; // we only call bubblesort from quicksort
        running = 0;
        for (index = first; index < last; index++)
        {
            quicksort_c++;
            if (arr[index] > arr[index+1])
            {
                running = 1;
                temp = arr[index];
                arr[index] = arr[index+1];
                arr[index+1] = temp;
                quicksort_s++;
            }
        }
        last--;
    }
}

#define FIRST 1
#define MEDIAN 2
#define RANDOM_MEDIAN 3
#define BUBBLE_LENGTH 0 // The partition length at which to resort to bubblesort
int bubble_length = BUBBLE_LENGTH;
int quicksort_pivot_selection_method = 1;
int quicksort(int arr[], int first, int last)
{
    int i, pivot, new_val;
    int m1, m2, m3;
    quicksort_c++;
    if (last > first && (((last-first)+1) <= bubble_length))
    {
        quicksort_p++;
        return bubblesort(arr,first,last);
    }
    if (last > first)
    {
        quicksort_p++;
        switch(quicksort_pivot_selection_method)
        {
            case FIRST:
                //Nothing to do because pivot will select first elem
                break;
            case MEDIAN:
                m1 = first;
                m2 = (first+last) / 2;
                m3 = last;
                if ((arr[m1] <= arr[m3] && arr[m3] <= arr[m2]) || (arr[m2] <= arr[m3] && arr[m3] <= arr[m1]))
                    pivot = m3;
                else if ((arr[m2] <= arr[m1] && arr[m1] <= arr[m3]) || (arr[m3] <= arr[m1] && arr[m1] <= arr[m2]))
                    pivot = m1;
                else
                    pivot = m2; 
                new_val = arr[first];
                arr[first] = arr[pivot];
                arr[pivot] = new_val;
                break;
            case RANDOM_MEDIAN:
                m1 = first + (rand() %(last-first+1));
                m2 = first + (rand() %(last-first+1));
                m3 = first + (rand() %(last-first+1));
                if ((arr[m1] <= arr[m3] && arr[m3] <= arr[m2]) || (arr[m2] <= arr[m3] && arr[m3] <= arr[m1]))
                    pivot = m3;
                else if ((arr[m2] <= arr[m1] && arr[m1] <= arr[m3]) || (arr[m3] <= arr[m1] && arr[m1] <= arr[m2]))
                    pivot = m1;
                else
                    pivot = m2;
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
            quicksort_c++;
            if (new_val < arr[pivot])
            {   
                arr[i] = arr[pivot+1];
                arr[pivot+1] = arr[pivot];
                arr[pivot] = new_val;
                quicksort_s+= 1;
                pivot++;
            }
        }
        /*for ( i = first; i < pivot; i++)
            printf("%d ",arr[i]);
        printf("(%d) ", arr[pivot]);
        for ( i = pivot+1; i <= last; i++)
            printf("%d ",arr[i]);
        printf("\n");*/
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
    bubble_length = 0;
    count = kvar_extract_array_length(param1);
    arr = kvar_extract_array(param1);
    quicksort_p = quicksort_s = quicksort_c = 0;
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
    kvar_var_ptr result;
    test_results results = test_create_results();
    long int time_diff;
    int count;
    int *arr;
    if (param_count != 1)
    {
        printf("quicksort: usage: quicksort(array)\n");
        return NULL;
    }
    int selected_bubble_length = 9;

    // so we can use the array several times, we can restore _temp1 from _temp
    kvar_copy(store,"_temp1", "_temp");

    param1 = kvar_get(store, "_temp1");
    if (param1->type != kvar_type_array)
    {
        printf("param 1 must be an array\n");
        return NULL;
    }

    // without bubble sort
    bubble_length = 0;
    count = kvar_extract_array_length(param1);
    arr = kvar_extract_array(param1);
    quicksort_p = quicksort_s = quicksort_c = 0;
    quicksort_pivot_selection_method = FIRST; 
    time_diff = clock();
    quicksort(arr,0, count-1);
    test_add_result(&results,"quicksort variant 1 (select first element as pivot)", quicksort_p, quicksort_s, quicksort_c, (clock()-time_diff)/1000);

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
    quicksort_pivot_selection_method = MEDIAN; 
    time_diff = clock();
    quicksort(arr,0, count-1);
    test_add_result(&results,"quicksort variant 2 (select median of start median and end as pivot)", quicksort_p, quicksort_s, quicksort_c, (clock()-time_diff)/1000);

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
    quicksort_pivot_selection_method = RANDOM_MEDIAN; 
    time_diff = clock();
    quicksort(arr,0, count-1);
    test_add_result(&results,"quicksort variant 3 (select median of 3 randoms as pivot)", quicksort_p, quicksort_s, quicksort_c, (clock()-time_diff)/1000);

    test_add_separator(&results);
    //with bubblesort
    bubble_length = selected_bubble_length;
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
    quicksort_pivot_selection_method = FIRST; 
    time_diff = clock();
    quicksort(arr,0, count-1);
    test_add_result(&results,"quicksort variant 1 (select first element as pivot)(change to bubblesort at partitionsize 9)", quicksort_p, quicksort_s, quicksort_c, (clock()-time_diff)/1000);

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
    quicksort_pivot_selection_method = MEDIAN; 
    time_diff = clock();
    quicksort(arr,0, count-1);
    test_add_result(&results,"quicksort variant 2 (select median of start median and end as pivot)(change to bubblesort at partitionsize 9)", quicksort_p, quicksort_s, quicksort_c, (clock()-time_diff)/1000);

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
    quicksort_pivot_selection_method = RANDOM_MEDIAN; 
    time_diff = clock();
    quicksort(arr,0, count-1);
    test_add_result(&results,"quicksort variant 3 (select median of 3 randoms as pivot)(change to bubblesort at partitionsize 9)", quicksort_p, quicksort_s, quicksort_c, (clock()-time_diff)/1000);

    test_print_results(results);
    test_dispose_results(&results);

    result = kvar_create_array(arr, count);
    return result;
}

function_list functions_load(void)
{
    function_list result;

    result.function_count = 7;

    result.functions = malloc(sizeof(function)*result.function_count);

    result.functions[0].name = malloc(strlen("dummy")+1);
    strcpy(result.functions[0].name, "dummy");
    result.functions[0].func = &dummy;

    result.functions[1].name = malloc(strlen("drandom")+1);
    strcpy(result.functions[1].name, "drandom");
    result.functions[1].func = &dyson_random;

    result.functions[2].name = malloc(strlen("urandom")+1);
    strcpy(result.functions[2].name, "urandom");
    result.functions[2].func = &urandom_random;

    result.functions[3].name = malloc(strlen("crandom")+1);
    strcpy(result.functions[3].name, "crandom");
    result.functions[3].func = &c_random;

    result.functions[4].name = malloc(strlen("descending")+1);
    strcpy(result.functions[4].name, "descending");
    result.functions[4].func = &dyson_descending;

    result.functions[5].name = malloc(strlen("quicksort")+1);
    strcpy(result.functions[5].name, "quicksort");
    result.functions[5].func = &quicksort_interface;

    result.functions[6].name = malloc(strlen("quicksorttest")+1);
    strcpy(result.functions[6].name, "quicksorttest");
    result.functions[6].func = &quicksort_test_interface;

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
