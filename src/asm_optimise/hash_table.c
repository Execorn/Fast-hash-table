#include "stdio.h"
#include "stdlib.h"
#include "hash_table.h"
#include "macro.h" 

#define GET_HASH HASH_6


static uint64_t HASH_1(const NODE_KEY_TYPE key);
static uint64_t HASH_2(const NODE_KEY_TYPE key);
static uint64_t HASH_3(const NODE_KEY_TYPE key);
static uint64_t HASH_4(const NODE_KEY_TYPE key);
static uint64_t HASH_5(const NODE_KEY_TYPE key);
static uint64_t HASH_6(const NODE_KEY_TYPE key);

#if (HT_DO_EXPAND == 1)
static int ht_expand(ht_t* table);
#endif

static int ht_set_key(node_t** nodes, const size_t capacity, const NODE_KEY_TYPE key, const NODE_VALUE_TYPE value, size_t* size);


ht_t* ht_init(size_t capacity) {
#if (DO_ASSERTS == 1)    
    if (capacity <= 0) {
        fprintf(stderr, "Capacity must be positive.\n");
        return NULL;
    }
#endif    
    ht_t* new_ht = (ht_t*) calloc(1, sizeof(ht_t));
    if (new_ht == NULL) {
        fprintf(stderr, "Can't allocate memory for new hash_table.\n");
        return NULL;
    }

    new_ht->capacity = capacity;
    new_ht->size     =        0;

    new_ht->nodes = (node_t**) calloc(new_ht->capacity, sizeof(node_t*));
    if (new_ht->nodes == NULL) {
        fprintf(stderr, "Can't allocate memory for linked lists.\n");
        free(new_ht); // ! IMPORTANT: do not forget to free(new_ht), else we'll get memory leak
        return NULL;
    }

    return new_ht;
}

ht_t* ht_free(ht_t* table) {
#if (DO_ASSERTS == 1)        
    if (table == NULL) {
        fprintf(stderr, "Nothing to free in non-existent table.\n");
        return NULL;
    }
#endif
    for (size_t node_idx = 0; node_idx < table->capacity; ++node_idx) {
        table->nodes[node_idx] = free_list(table->nodes[node_idx]);
    }

    table->size      = 0;
    table->capacity  = 0;

    free(table->nodes);
    free(table);

    return NULL;
}

int ht_insert_key(ht_t* table, const NODE_KEY_TYPE key, const NODE_VALUE_TYPE value) {
#if (DO_ASSERTS == 1)        
    if (table == NULL) {
        fprintf(stderr, "Can't insert key into non-existent table.\n");
        return 1;
    }
    
    if (key == NULL) {
        fprintf(stderr, "Can't insert NODE_DEFAULT_KEY to the table.\n");
        return 1;
    }
#endif    
#if (HT_DO_EXPAND == 1)
    if (table->size >= table->capacity * LOAD_FACTOR) { // ? If table is filled more than 72% already, we expand it
        if (ht_expand(table)) {
            fprintf(stderr, "Expanding hash table failed.\n");
            return 1;
        }
    }
#endif    
    
    if (ht_set_key(table->nodes, table->capacity, key, value, &table->size)) {
        fprintf(stderr, "Insertion failed.\n");
        return 1;
    };
    
    return 0;
}

void ht_erase_key(ht_t* table, const NODE_KEY_TYPE key) {
#if (DO_ASSERTS == 1)        
    if (table == NULL) {
        fprintf(stderr, "Nothing to erase in non-existent table.\n");
        return;
    }

    if (key == NULL) {
        fprintf(stderr, "Can't erase NODE_DEFAULT_KEY from the table.\n");
        return;
    }
#endif
    uint64_t hash_value = GET_HASH(key);
    table->nodes[hash_value % table->capacity] = erase_node(table->nodes[hash_value % table->capacity], key); 
}

#if (HT_DO_EXPAND == 1)
static int ht_expand(ht_t* table) {
    if (table == NULL) {
        fprintf(stderr, "Can't expand non-existent table.\n");
        return 1;
    }

    size_t new_capacity = (size_t) (table->capacity * GROWTH_FACTOR + 1);
    // ! + 1 is needed because if capacity is 1 -> (size_t) (table->capacity * GROWTH_FACTOR) is also 1

    node_t** nodes = (node_t**) calloc(new_capacity, sizeof(node_t*));
    if (nodes == NULL) {
        fprintf(stderr, "Can't expand table: not enough memory.\n");
        return 1;
    }
    
    for (size_t node_idx = 0; node_idx < table->capacity; ++node_idx) {
        if (table->nodes[node_idx] != NULL) {
            uint64_t hash_value = GET_HASH(table->nodes[node_idx]->key);
            nodes[hash_value % new_capacity] = merge_lists(nodes[hash_value % new_capacity], table->nodes[node_idx]);
        }
    }

    free(table->nodes);

    table->nodes    =        nodes;
    table->capacity = new_capacity;

    return 0;
}
#endif

static int ht_set_key(node_t** nodes, const size_t capacity, const NODE_KEY_TYPE key, const NODE_VALUE_TYPE value, size_t* size) {
#if (DO_ASSERTS == 1)    
    if (nodes == NULL || key == NULL || size == NULL) {
        return 1;
    }
#endif
    uint64_t hash_value = GET_HASH(key);

    node_t* current_node = find_node(nodes[hash_value % capacity], key);
    if (current_node != NULL) {
        current_node->value = value;
        return 0;
    }

    nodes[hash_value % capacity] = insert_node(nodes[hash_value % capacity], key, value);
    ++(*size);

    return 0;
}

size_t ht_length(ht_t* table) {
#if (DO_ASSERTS == 1)    
    if (table == NULL) {
        fprintf(stderr, "Non-existent table has no length.\n");
        return 0;
    }
#endif
    return table->size;
}

NODE_VALUE_TYPE ht_get(ht_t* table, const NODE_KEY_TYPE key) {
#if (DO_ASSERTS == 1)    
    if (table == NULL) {
        fprintf(stderr, "Can't get value from non-existent table.\n");
        return NODE_DEFAULT_VALUE;
    }   
    
    if (key == NULL) {
        fprintf(stderr, "Can't get value from NODE_DEFAULT_KEY.\n");
        return NODE_DEFAULT_VALUE;
    }
     
    if (table->nodes == NULL) {
        return NODE_DEFAULT_VALUE;
    }
#endif    
    uint64_t hash_value = GET_HASH(key);
     
    node_t* needed_node = find_node(table->nodes[hash_value % table->capacity], key);
    if (needed_node == NULL) {
        return NODE_DEFAULT_VALUE;
    }
     
    return needed_node->value;
}

size_t ht_print_content(ht_t* table, FILE* output_stream) {
#if (DO_ASSERTS == 1)    
    if (table == NULL || output_stream == NULL) {
        return 0; // ! print out zero nodes if nothing happens
    }

    if (table->nodes == NULL) {
        return 0;
    }
#endif
    size_t printed_nodes = 0;
    
    for (size_t node_idx = 0; node_idx < table->capacity; ++node_idx) {
        node_t* current_head = table->nodes[node_idx];
        while (current_head != NULL) {
            print_node(current_head, output_stream);
            ++printed_nodes;
            current_head = current_head->next;
        }
    }

    return printed_nodes;
}


//----------------------------------------------------------------
// !              HASH FUNCTIONS IMPLEMENTATION
// ! 6 different hash functions provided, from terrible to crc32
// !         Default function is set using macro GET_HASH
//----------------------------------------------------------------

static uint64_t HASH_1(const NODE_KEY_TYPE key) {
    return 1;
}

static uint64_t HASH_2(const NODE_KEY_TYPE key) {
    return (uint64_t) (*key);
}

static uint64_t HASH_3(const NODE_KEY_TYPE key) {
    uint32_t sum = 0;
    __asm__ __volatile__ (".intel_syntax noprefix;"
            "push %1;"
            "mov ecx, 32;"
            "xor eax, eax;"
            "lop%=:"
            "cmp ecx, 0;"
            "je end%=;"
            "add eax, [%1];"
            "inc %1;"
            "dec ecx;"
            "jmp lop%=;"
            "end%=:"
            "mov %0, eax;"
            "pop %1;"
            ".att_syntax prefix;"
            : "=r" (sum)
            : "r" (key)
            : "ecx", "eax");

    return sum;        
}

static uint64_t HASH_4(const NODE_KEY_TYPE key) {
    return strlen(key);
}

static uint64_t HASH_5(const NODE_KEY_TYPE key) {
    uint64_t hash = *(key++);

    while (*key) {
        hash = (hash << 1 | hash >> (64 - 1)) + *(key++);
    }

    return hash;
}

static uint64_t HASH_6(const NODE_KEY_TYPE key) {
    uint64_t hash = 0;

    // ! size of string is 32 bytes, it is guaranteed

    for (unsigned i = 0; i < 4; ++i) {
        hash += _mm_crc32_u64(hash, *((uint64_t*) key));
        key += 8;
    }

    return hash;
}