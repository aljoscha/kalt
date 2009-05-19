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
    free(store);
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

void kvar_copy(kvar_storage_ptr store, char *src, char *dest)
{
    if (!(kvar_exists(store, src)))
        return;
        
    printf("kvar_copy: copying %s to %s\n", src, dest);
    kvar_delete(store, dest);
    kvar_var_ptr src_var = kvar_get(store, src);
    if (src_var->type == kvar_type_number)
    {
        kvar_var_ptr number_var = kvar_create_number(kvar_extract_number(src_var));
        kvar_set(store, dest, number_var);
    }
    else if (src_var->type == kvar_type_array)
    {
        int *array = kvar_extract_array(src_var);
        int length = kvar_extract_array_length(src_var);
        if (array == NULL)
            return;
        kvar_var_ptr array_var = kvar_create_array(array, length);
        kvar_set(store, dest, array_var);
    }
}

void kvar_delete(kvar_storage_ptr store, char *name)
{
    int index;
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
                        free(var);
                    } break;
                case kvar_type_array:
                    {
                        DOUT("kvar_delete: deleting variable \""); DOUT(name); DOUT("\" of type array\n");
                        free(var->array->array);
                        free(var->array);
                        free(var);
                    } break;
            } 
            
            store->vars[index].content = NULL;
            return;
        }
    }

}

int kvar_exists(kvar_storage_ptr store, char *name)
{
    int index;
    for (index = 0; index < MAX_VARS; index++)
    {
        if (store->vars[index].content != NULL && strcmp(store->vars[index].name, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

kvar_var_ptr kvar_create_number(int value)
{
    kvar_var_ptr temp = malloc(sizeof(kvar_var));
    temp->type = kvar_type_number;
    temp->number = value;
    return temp;
}

kvar_var_ptr kvar_create_array(int *content, int length)
{
    kvar_var_ptr temp = malloc(sizeof(kvar_var));
    temp->type = kvar_type_array;
    temp->array = malloc(sizeof(kvar_array));
    temp->array->length = length;
    temp->array->array = malloc(sizeof(int)*length);
    memcpy(temp->array->array, content, sizeof(int)*length);
    return temp;
}

int kvar_extract_number(kvar_var_ptr var)
{
    if (var == NULL)
    {
        return -1;
    }
    if (var->type == kvar_type_number)
    {
        return var->number;
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
        return var->array->array;
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
        return var->array->length;
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
                        printf("%s: %d\n", store->vars[index].name, store->vars[index].content->number);
                    } break;
                case kvar_type_array:
                    {
                        printf("%s: ", store->vars[index].name);
                        arr = store->vars[index].content->array;
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
