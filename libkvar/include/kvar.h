#ifndef KVAR_INC
#define KVAR_INC

// consts
#define MAX_NAME 30
#define MAX_VARS 40

// types
typedef enum kvar_type_t
{
    kvar_type_number, kvar_type_boolean, kvar_type_array
} kvar_type;

typedef struct kvar_array_t
{
    int length;
    int* array;
} kvar_array, *kvar_array_ptr;

typedef struct kvar_var_t
{
    kvar_type type;
    union
    {
        int number;
        kvar_array_ptr array;
    };
} kvar_var, *kvar_var_ptr;

typedef struct kvar_node_t
{
    char name[MAX_NAME];
    kvar_var_ptr content;
} kvar_node, *kvar_node_ptr;

typedef struct kvar_storage_t
{
    int version;
    int num_vars;
    kvar_node vars[MAX_VARS];
} kvar_storage, *kvar_storage_ptr;    

// functions
kvar_storage_ptr kvar_create_storage(void);
void kvar_dispose_storage(kvar_storage_ptr);
kvar_var_ptr kvar_get(kvar_storage_ptr, char *name);
void kvar_set(kvar_storage_ptr, char *name, kvar_var_ptr);
void kvar_delete(kvar_storage_ptr, char *name);

kvar_var_ptr kvar_create_number(int value);
kvar_var_ptr kvar_create_array(int *content, int length);

int kvar_extract_number(kvar_var_ptr var);
int* kvar_extract_array(kvar_var_ptr var);
int kvar_extract_array_length(kvar_var_ptr var);

void kvar_print_vars(kvar_storage_ptr store);
#endif
