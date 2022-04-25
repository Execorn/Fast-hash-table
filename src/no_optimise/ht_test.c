#include "hash_table.h"
#include <sys/stat.h>

void print_pairs(node_t* head, FILE* stream) {
    if (stream == NULL) {
        return;
    }

    while (head != NULL) {
        print_node(head, stream);
        head = head->next;
    }
}


int main() {
    size_t default_table_cap = 100;

    struct stat st;
    stat("./../../txt_files/war_and_peace.txt", &st);

    size_t total_bytes = st.st_size;

    // ---------------------------------------------------------------
    char* source = (char*) calloc(total_bytes + 1, sizeof(char));
    FILE* war_and_piece = fopen("./../../txt_files/war_and_peace.txt", "r");
    if (war_and_piece == NULL) {
        fprintf(stderr, "Can't open war_and_peace.txt.\n");
        return 1;
    }

    char     cur     = 0;
    size_t   buf_ptr = 0;

    while ((cur = fgetc(war_and_piece)) != EOF) {
        if (cur != '\n' && cur != '\r' && cur != '\0' && cur >= 32) {
            source[buf_ptr++] = cur;
        } else {
            source[buf_ptr++] = ' '; // ! removing special characters, adding space separator instead
        }
    }
    fclose(war_and_piece);
    // ----------------------------------------------------------------
    char* safe_source = source;
    char* token       =   NULL;

    ht_t* words_ht = ht_init(default_table_cap);
    if (words_ht == NULL) {
        fprintf(stderr, "Can't create hash table.\n");
        return 1;
    }

    size_t tokens_scanned = 0;
    while ((token = strtok_r(safe_source, " ()\",.!?~<>=:;`-=*0123456789", &safe_source)) != NULL) {
        ++tokens_scanned;

        if (ht_get(words_ht, token) == NODE_DEFAULT_VALUE) {
            int is_success = ht_insert_key(words_ht, token, 1);
            if (is_success) {
                fprintf(stderr, "Can't insert key %s.\n", token);
                break;
            }
        } else {
            int is_success = ht_insert_key(words_ht, token, ht_get(words_ht, token) + 1);
            if (is_success) {
                fprintf(stderr, "Can't insert key %s.\n", token);
                break;
            }
        }
    }

    FILE* war_and_piece_result = fopen("txt_files/ht_result_no_optimise.txt", "w");
    if (war_and_piece_result == NULL) {
        fprintf(stderr, "Can't open output file.\n");
        return 1;
    }

    for (size_t idx = 0; idx < words_ht->capacity; ++idx) {
        if (words_ht->nodes[idx] != NULL) {
            print_pairs(words_ht->nodes[idx], war_and_piece_result);
        }
    }

    fclose(war_and_piece_result);
    free(source);
    words_ht = ht_free(words_ht);

    return 0;
}  