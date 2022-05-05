#pragma once

#include <string.h>
#include <immintrin.h>
// ----------------------------------------------------------------
// ? Here you can define your custom types for hash table 
// ! Also, don't forget to define default values for your types
// ! Also, you can edit function compare_key()
// ? Also, you can edit function print_node() 
// ----------------------------------------------------------------

#define NODE_VALUE_TYPE     size_t
#define NODE_KEY_TYPE       char*

#define POISON_PTR          (void*)           0xDEADDEAD;
#define POISON_KEY          (NODE_KEY_TYPE)   0xDEADBEEF;
#define POISON_VALUE        (NODE_VALUE_TYPE) 0xDEADDEAD;

#define NODE_DEFAULT_VALUE  0
#define NODE_DEFAULT_KEY    NULL

#define HT_DO_EXPAND        1 // ! 1 = true, 0 = false ONLY 0, 1 req. 

// ! ----------------------------------------------------------------
//                  ! DO NOT EDIT THIS PART !
struct node_t {
    struct node_t*   next;

    NODE_KEY_TYPE     key;
    NODE_VALUE_TYPE value;
};

typedef struct node_t node_t;
// ! ----------------------------------------------------------------

//extern int my_strcmp(const char* a, const char* b);


static int compare_key(const NODE_KEY_TYPE a, const NODE_KEY_TYPE b) {
    if (a == NULL || b == NULL) {
        return 0;
    }    
    return strcmp(a, b);
}

static void print_node(const node_t* node, FILE* stream) {
    if (node == NULL) {
        return;
    }

    fprintf(stream, "word: %22s  ->  usages: [%7zu]\n", node->key, node->value);
}