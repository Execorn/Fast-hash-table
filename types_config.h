#pragma once

#include <string.h>
// ----------------------------------------------------------------
// ? Here you can define your custom types for hash table 
// ! Also, don't forget to define default values for your types
// ? Also, you can edit function compare_key() function
// ----------------------------------------------------------------

#define NODE_VALUE_TYPE     char*
#define NODE_KEY_TYPE       char*

#define POISON_PTR          (void*)         0xDEADDEAD;
#define POISON_KEY          (NODE_KEY_TYPE) 0xDEADBEEF;
#define POISON_VALUE        (NODE_KEY_TYPE) 0xDEADBEEF;

#define NODE_DEFAULT_VALUE  NULL
#define NODE_DEFAULT_KEY    NULL

static int compare_key(const NODE_KEY_TYPE a, const NODE_KEY_TYPE b) {
    if (a == NULL || b == NULL) {
        return 0;
    }    
    return strcmp(a, b);
}