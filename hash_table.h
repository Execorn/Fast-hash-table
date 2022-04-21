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

int ht_insert_key(ht_t* table, const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value);

NODE_VALUE_TYPE ht_find(ht_t* table, const KEY_VALUE_TYPE key);

static int ht_expand(ht_t* table);

void ht_erase_key(ht_t* table, const KEY_VALUE_TYPE key);

static int ht_set_key(node_t** nodes, const size_t capacity, const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value, size_t* size);

NODE_VALUE_TYPE ht_get(ht_t* table, const NODE_VALUE_TYPE value);

int isPrime(size_t number);

static uint64_t HASH_1(const KEY_VALUE_TYPE key);
static uint64_t HASH_2(const KEY_VALUE_TYPE key);
static uint64_t HASH_3(const KEY_VALUE_TYPE key);
static uint64_t HASH_4(const KEY_VALUE_TYPE key);
static uint64_t HASH_5(const KEY_VALUE_TYPE key);
static uint64_t HASH_6(const KEY_VALUE_TYPE key);
