#include "kvar.h"
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>

#define VERSION 1
#define DEBUG 1

#ifdef DEBUG
#define DOUT(text) (printf("%s",text))
#else
#define DOUT(text) ()
#endif

kvar_storage_ptr kvar_create_storage(void)
{
    kvar_storage_ptr temp = malloc(sizeof(kvar_storage));
    temp->version = VERSION;
    temp->num_vars = 0;
    int index;
    for (index = 0; index < MAX_VARS; index++)
    {
       temp->vars[index].content = NULL;
    }
    DOUT("kvar_create_storage: created kvar storage\n");
    return temp;
}

void kvar_dispose_storage(kvar_storage_ptr store)
{
    DOUT("kvar_dispose_storage: disposing kvar storage\n");
    int index;
    for (index = 0; index < MAX_VARS; index++)
    {
       kvar_delete(store, store->vars[index].name);
    }

}

kvar_var_ptr kvar_get(kvar_storage_ptr store, char *name)
{
    int index;
    for (index = 0; index < MAX_VARS; index++)
    {
        if (strcmp(store->vars[index].name, name) == 0)
        {
            DOUT("kvar_get: found variable \""); DOUT(name); DOUT("\"\n");
            return store->vars[index].content;
        }
    }
    DOUT("kvar_get: did not find variable \""); DOUT(name); DOUT("\"\n");
    return NULL;
}


void kvar_set(kvar_storage_ptr store, char *name, kvar_var_ptr var)
{
    int index;
    for (index = 0; index < MAX_VARS; index++)
    {
        if (store->vars[index].content != NULL && strcmp(store->vars[index].name, name) == 0)
        {
            kvar_delete(store, name);
            kvar_set(store, name, var);
            return;
        }
    }
    for (index = 0; index < MAX_VARS; index++)
    {
        if (store->vars[index].content == NULL)
        {
            DOUT("kvar_set: storing variable \""); DOUT(name); DOUT("\"\n");
            strcpy(store->vars[index].name, name);
            store->vars[index].content = var;
            return;
        }
    }
}

void kvar_delete(kvar_storage_ptr store, char *name)
{
    int index;
    kvar_array_ptr arr;
    kvar_var_ptr var;
    for (index = 0; index < MAX_VARS; index++)
    {
        if (store->vars[index].content != NULL && strcmp(store->vars[index].name, name) == 0)
        {
            var = store->vars[index].content;
            switch (var->type)
            {
                case kvar_type_number:
                    {
                        DOUT("kvar_delete: deleting variable \""); DOUT(name); DOUT("\" of type number\n");
                        free(var->content);
                        free(var);
                    } break;
                case kvar_type_array:
                    {
                        DOUT("kvar_delete: deleting variable \""); DOUT(name); DOUT("\" of type array\n");
                        arr = (kvar_array*)var->content;
                        free(arr->array);
                        free(arr);
                        free(var);
                    } break;
            } 
            
            store->vars[index].content = NULL;
            return;
        }
    }

}

kvar_var_ptr kvar_create_int(int value)
{
    kvar_var_ptr temp = malloc(sizeof(kvar_var));
    temp->type = kvar_type_number;
    temp->content = malloc(sizeof(int));
    *((int*)temp->content) = value;
    return temp;
}

kvar_var_ptr kvar_create_array(int *content, int length)
{
    kvar_var_ptr temp = malloc(sizeof(kvar_var));
    kvar_array_ptr arr;
    temp->type = kvar_type_array;
    arr = malloc(sizeof(kvar_array));
    arr->length = length;
    arr->array = malloc(sizeof(int)*length);
    memcpy(arr->array, content, sizeof(int)*length);
    temp->content = (void*)arr;
    return temp;
}

int kvar_extract_int(kvar_var_ptr var)
{
    if (var == NULL)
    {
        return -1;
    }
    if (var->type == kvar_type_number)
    {
        return *((int*)var->content);
    }
    else
    {
        return -1;
    }
}

int* kvar_extract_array(kvar_var_ptr var)
{
    if (var == NULL)
    {
        return NULL;
    }
    if (var->type == kvar_type_array)
    {
        return ((kvar_array_ptr)var->content)->array;
    }
    else
    {
        return NULL;
    }
}

int kvar_extract_array_length(kvar_var_ptr var)
{
    if (var == NULL)
    {
        return -1;
    }
    if (var->type == kvar_type_array)
    {
        return ((kvar_array_ptr)var->content)->length;
    }
    else
    {
        return -1;
    }
}

void kvar_print_vars(kvar_storage_ptr store)
{
    DOUT("kvar_print_vars: dumping all variables\n");
    int index,index2;
    kvar_array_ptr arr;
    for (index = 0; index < MAX_VARS; index++)
    {
        if (store->vars[index].content != NULL)
        {
            switch (store->vars[index].content->type)
            {
                case kvar_type_number:
                    {
                        printf("%s: %d\n", store->vars[index].name, *((int*)store->vars[index].content->content));
                    } break;
                case kvar_type_array:
                    {
                        printf("%s: ", store->vars[index].name);
                        arr = (kvar_array_ptr)store->vars[index].content->content;
                        for (index2 = 0; index2 < arr->length; index2++)
                        {
                            printf("%d ", arr->array[index2]);
                        }
                        printf("\n");
                    } break;
            } 
        }
    }
}
