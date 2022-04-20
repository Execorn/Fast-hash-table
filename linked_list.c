#include "linked_list.h"

node_t* new_node(const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value) {
    assert("NODE_DATA_ERROR" && key != NULL && value != NULL);

    node_t* created_node = (node_t*) calloc(1, sizeof(node_t)); 
    assert("NO_MEM_ERROR" && created_node != NULL);

    created_node->key   =  ( KEY_VALUE_TYPE)   key;
    created_node->value =  (NODE_VALUE_TYPE) value;
    created_node->next  =                     NULL; 
    
    return created_node;
}

node_t* free_list(node_t* head) {
    if (head == NULL) {
        return NULL;
    }

    if (head->next) {
        return free_list(head->next);
    }    

    return free_node(head);   
}

node_t* free_node(node_t* node) {
    if (node == NULL) {
        return NULL;
    }

    free(node);

    return NULL;
}

node_t* insert_node(node_t* head, const KEY_VALUE_TYPE key, const NODE_VALUE_TYPE value) {
    assert("NODE_DATA_ERROR" && key != NULL && value != NULL);
    
    if (head == NULL) {
        return new_node(key, value);
    }

    node_t* current_node = find_node(head, key);
    if (current_node != NULL) {
        current_node->value = (NODE_VALUE_TYPE) value;
        return head;
    } else {
        current_node = head;
    }

    while (current_node != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node(key, value);
    return head;
}

node_t* find_node(node_t* head, const KEY_VALUE_TYPE key) {
    assert("NODE_DATA_ERROR" && key != NULL);

    while (head) {
        if (strcmp(head->key, key) == 0) {
            return head;
        }
        head = head->next;
    }

    return NULL;
}

NODE_VALUE_TYPE get_node_value(node_t* head, const KEY_VALUE_TYPE key) {
    assert("NODE_DATA_ERROR" && key != NULL);

    if (head == NULL) {
        return NULL;
    }

    node_t* requested = find_node(head, key);
    if (requested != NULL) {
        return requested->value;
    } 

    return NULL;
}

node_t* erase_node(node_t* head, const KEY_VALUE_TYPE key) {
    assert("NODE_DATA_ERROR" && key != NULL);
    
    if (head == NULL) {
        return NULL;
    }


    if (strcmp(head->key, key) == 0) {
        node_t* new_head = head->next;

        free_node(head);
        return new_head;
    }

    node_t *prev = head, *cur = head->next;

    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            prev->next = cur->next; // (prev -> cur -> cur.next), so we connect (prev -> cur.next) and skip cur

            free(cur);
            return head;
        }
        prev = cur;
        cur  = cur->next;
    }

    return head;
}