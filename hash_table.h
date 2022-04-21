#pragma once

// clang-format off
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <smmintrin.h> 

#include "linked_list.h"

static const double GROWTH_FACTOR = 1.61803398875; // 1 + (sqrt(5) - 1) / 2
static const double   LOAD_FACTOR = 0.72f;         // load factor same as in C#  

typedef struct {
    node_t**  nodes;
    size_t     size;
    size_t capacity;
} ht_t;

ht_t* ht_init(size_t capacity);

ht_t* ht_free(ht_t* table);

int ht_insert_key(ht_t* table, const NODE_KEY_TYPE key, const NODE_VALUE_TYPE value);

NODE_VALUE_TYPE ht_find(ht_t* table, const NODE_KEY_TYPE key);

void ht_erase_key(ht_t* table, const NODE_KEY_TYPE key);

NODE_VALUE_TYPE ht_get(ht_t* table, const NODE_VALUE_TYPE value);

int isPrime(size_t number);
