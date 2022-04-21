#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NODE_VALUE_TYPE char*
#define  KEY_VALUE_TYPE char*

static void*           POISON_PTR   = (void*) 0xDEADDEAD;
static NODE_VALUE_TYPE POISON_VALUE = (char*) 0xDEADBEEF;;

struct node_t {
    struct node_t*   next;

    KEY_VALUE_TYPE    key;
    NODE_VALUE_TYPE value;
};

typedef struct node_t node_t;


static node_t* new_node(const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value);

node_t* free_list(node_t* head);

static node_t* free_node(node_t* node);

node_t* insert_node(node_t* head, const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value);

node_t* erase_node(node_t* head, const KEY_VALUE_TYPE key);

node_t* find_node(node_t* head, const KEY_VALUE_TYPE key);