#pragma once

#include <string.h>
#include <immintrin.h>
#include <stdio.h>

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

static int avx_strcmp(const char* a, const char* b) {
    // ! 'a' and 'b' are guaranteed to be not-NULL 
    __m256i first  = _mm256_load_si256((const __m256i*) a);
    __m256i second = _mm256_load_si256((const __m256i*) b);

    // ? compare packed 8-bit integers in a and b for equality
    __m256i compare = _mm256_cmpeq_epi8(first, second);

    // ? create mask from the most significant bit of each 8-bit element in a
    int mask = _mm256_movemask_epi8(compare);

    return ~mask; // ! invert mask to get zero if strings are equal
}

static int compare_key(const NODE_KEY_TYPE a, const NODE_KEY_TYPE b) {
    return avx_strcmp(a, b); // * returns non-zero value if strings are not equal, not necessary positive (>) / neg. (<)
}

static void print_node(const node_t* node, FILE* stream) {
    if (node == NULL) {
        return;
    }

    fprintf(stream, "word: %22s  ->  usages: [%7zu]\n", node->key, node->value);
}