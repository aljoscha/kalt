#include "kvar.h"
#include <stdio.h>

int main(void)
{
    kvar_storage_ptr store = kvar_create_storage();
    kvar_print_vars(store);    
    kvar_var_ptr num = kvar_create_number(17);
    kvar_set(store, "bhallo", num);
    num = kvar_create_number(11);
    kvar_set(store, "foo", num);

    int foo = kvar_extract_number(kvar_get(store, "foo"));
    printf ("foo is: %d\n", foo);

    kvar_print_vars(store);    

    int nums[10] = {13,6,3,4,5,6,7,8,9,0};
    kvar_var_ptr arr = kvar_create_array(nums, 10);
    kvar_set(store, "arr", arr);
    
    foo = kvar_extract_number(kvar_get(store, "arr"));
    printf ("foo is: %d\n", foo);

    num = kvar_create_number(30);
    kvar_set(store, "foo", num);

    kvar_delete(store, "hallo");
    
    kvar_copy(store, "arr", "arr_two");
    kvar_delete(store, "arr");
    kvar_copy(store, "foo", "foo");
    kvar_delete(store, "foo");
    kvar_print_vars(store);    
    kvar_dispose_storage(store);    

    return 0;
}
