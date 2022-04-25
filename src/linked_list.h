#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types_config.h"

node_t* free_list(node_t* head);

node_t* insert_node(node_t* head, const NODE_KEY_TYPE key, const NODE_VALUE_TYPE value);

node_t* erase_node(node_t* head, const NODE_KEY_TYPE key);

node_t* find_node(node_t* head, const NODE_KEY_TYPE key);