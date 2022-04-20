#include "stdio.h"
#include "stdlib.h"
#include "hash_table.h" 

#define GET_HASH HASH_6


ht_t* ht_init(size_t capacity) {
    if (capacity == 0) {
        fprintf(stderr, "Capacity must be positive.\n");
        return NULL;
    }
    
    ht_t* new_ht = (ht_t*) calloc(1, sizeof(ht_t));
    if (new_ht == NULL) {
        fprintf(stderr, "Can't allocate memory for new hash_table.\n");
        return NULL;
    }

    new_ht->capacity = capacity;
    new_ht->size     =        0;

    new_ht->nodes = (node_t**) calloc(capacity, sizeof(node_t*));
    if (new_ht->nodes == NULL) {
        fprintf(stderr, "Can't allocate memory for lists.\n");
        free(new_ht);
        return NULL;
    }

    return new_ht;
}

ht_t* ht_free(ht_t* table) {
    if (table == NULL) {
        return NULL;
    }

    table->size     = 0;
    table->capacity = 0;

    for (size_t node = 0; node < table->capacity; ++node) {
        table->nodes[node] = free_list(table->nodes[node]);
    }

    free(table);

    return NULL;
}

int ht_insert_key(ht_t* table, const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value) {
    assert("DATA_ERROR" && key != NULL && value != NULL);

    if (table == NULL) {
        fprintf(stderr, "Can't insert key into non-existent table.\n");
        return 1;
    }

    if (table->size >= table->capacity * LOAD_FACTOR) {
        if (ht_expand(table)) {
            fprintf(stderr, "Expanding hash table failed.\n");
            return 1;
        }
    }

    if (ht_set_key(table->nodes, table->capacity, key, value, &table->size)) {
        fprintf(stderr, "Insertion failed.\n");
        return 1;
    };

    return 0;
}

void ht_erase_key(ht_t* table, const KEY_VALUE_TYPE key) {
    if (table == NULL || key == NULL) {
        return;
    }

    uint64_t hash_value = GET_HASH(key);
    table->nodes[hash_value % table->capacity] = erase_node(table->nodes[hash_value % table->capacity], key); 
}

int ht_expand(ht_t* table) {
    if (table == NULL) {
        fprintf(stderr, "Can't expand non-existent table.\n");
        return 1;
    }

    size_t new_capacity = (size_t) (table->capacity * GROWTH_FACTOR);

    node_t** nodes = (node_t**) calloc(new_capacity, sizeof(node_t*));
    if (nodes == NULL) {
        fprintf(stderr, "Can't expand table: not enough memory.\n");
        return 1;
    }
    
    for (size_t node = 0; node < table->capacity; ++table) {
        if (table->nodes[node]) {
            uint64_t hash_value = GET_HASH(table->nodes[node]->key);
            nodes[hash_value % new_capacity] = table->nodes[node];
        }
    }
    free(nodes);

    return 0;
}

int ht_set_key(node_t** nodes, const size_t capacity, const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value, size_t* size) {
    if (nodes == NULL || key == NULL || value == NULL || size == NULL) {
        return 1;
    }

    uint64_t hash_value = GET_HASH(key);

    nodes[hash_value % capacity] = insert_node(nodes[hash_value % capacity], key, value);
    
    ++(*size); // ! INCREASE HASH TABLE SIZE

    return 0;
}

size_t ht_length(ht_t* table) {
    if (table == NULL) {
        return 0;
    }

    return table->size;
}

NODE_VALUE_TYPE ht_find(ht_t* table, const KEY_VALUE_TYPE key) {
    if (table == NULL || key == NULL) {
        return NULL;
    }

    uint64_t hash_value = GET_HASH(key);

    if (table->nodes == NULL) {
        return NULL;
    }
    NODE_VALUE_TYPE required = get_node_value(table->nodes[hash_value % table->capacity], key);
    return required ? required : NULL;
}

// ? Unused function, I didn't remove it because it may be useful for different hash implementation
int isPrime(size_t number) {
    if (number % 2 == 0) return number == 2;

    size_t divider = 3;
    while (divider * divider <= number && number % divider != 0) divider += 2;

    return divider * divider > number;
}


uint64_t HASH_1(const KEY_VALUE_TYPE key) {
    if (key == NULL) {
        return 0;
    }

    return 1;
}

uint64_t HASH_2(const KEY_VALUE_TYPE key) {
    if (key == NULL) {
        return 0;
    }

    return (uint64_t) (*key);
}

uint64_t HASH_3(const KEY_VALUE_TYPE key) {
    if (key == NULL) {
        return 0;
    }

    uint64_t hash = 0;
    while (*key) {
        hash += *(key++);
    }

    return hash;
}

uint64_t HASH_4(const KEY_VALUE_TYPE key) {
    if (key == NULL) {
        return 0;
    }

    return strlen(key);
}

uint64_t HASH_5(const KEY_VALUE_TYPE key) {
    if (key == NULL) {
        return 0;
    }

    uint64_t hash = *(key++);

    while (*key) {
        hash = (hash << 1 | hash >> (64 - 1)) + *(key++);
    }

    return hash;
}

uint64_t HASH_6(const KEY_VALUE_TYPE key) {
    if (key == NULL) {
        return 0;
    }

    uint64_t hash = 0;

    while (*key) {
        hash = _mm_crc32_u8(hash, *(key++));
    }

    return hash;
}

int main() {
    ht_t* new_ht =  ht_init(100);
    ht_insert_key(new_ht, "a", "hello");
    //ht_erase_key(new_ht, "a");

    char* retval = ht_find(new_ht, "a");
    if (retval == NULL) {
        printf("found nothing 1\n");
    } else {
        printf("i got %s\n", retval);
    }
    ht_free(new_ht);
}