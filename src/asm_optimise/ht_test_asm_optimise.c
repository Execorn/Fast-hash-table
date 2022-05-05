#include "hash_table.h"
#include <sys/stat.h>
#include <immintrin.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>


void print_pairs(node_t* head, FILE* stream) {
    if (stream == NULL) {
        return;
    }

    while (head != NULL) {
        print_node(head, stream);
        head = head->next;
    }
}


int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Please provide two arguments: source text path, and path of an output file.\n");
        return EXIT_FAILURE;
    } else if (argc >= 4) {
        fprintf(stderr, "Too many arguments.\n");
        return EXIT_FAILURE;
    }

    char* source_file_path = argv[1];
    char* output_file_path = argv[2];
    
    struct stat source_file_stat;
    if (stat(source_file_path, &source_file_stat) != 0) {
        fprintf(stderr, "Can't get '%s' stat. Please check if path is correct and try again.\n", source_file_path);
        return EXIT_FAILURE;
    }
    
    size_t source_file_size_in_bytes = source_file_stat.st_size;

    // ---------------------------------------------------------------
    char* source_buffer = (char*) calloc(source_file_size_in_bytes + 1, sizeof(char));
    if (source_buffer == NULL) {
        fprintf(stderr, "Can't allocate memory for source file buffer.\n");
        return EXIT_FAILURE;
    }
    
#ifdef __WIN32   
    #define OPEN_TYPE "rb"
#else
    #define OPEN_TYPE "r"    
#endif
    FILE* source_file_stream = fopen(source_file_path, OPEN_TYPE);
    if (source_file_stream == NULL) {
        fprintf(stderr, "Can't open '%s'. Path is invalid.\n", source_file_path);

        free(source_buffer);
        return EXIT_FAILURE;
    }
#undef OPEN_TYPE

    size_t read_bytes = 0;
    if ((read_bytes = fread(source_buffer, sizeof(char), source_file_size_in_bytes, source_file_stream)) != source_file_size_in_bytes) {
        fprintf(stderr, "Something went wrong while reading. Read %zu bytes, %zu expected.\n", read_bytes, source_file_size_in_bytes);

        free(source_buffer);
        fclose(source_file_stream);
        return EXIT_FAILURE;
    }
    // ----------------------------------------------------------------
    
    
    size_t default_table_cap = 100;
    ht_t* file_content_ht = ht_init(default_table_cap);
    if (file_content_ht == NULL) {
        fprintf(stderr, "Can't create hash table.\n");

        free(source_buffer);
        fclose(source_file_stream);
        return EXIT_FAILURE;
    }
    /*
    char** tokens = (char**) calloc(default_table_cap, sizeof(char*));
    if (tokens == NULL) {
        fprintf(stderr, "Can't allocate memory for token's array.\n");
        
        free(source_buffer);
        fclose(source_file_stream);
        file_content_ht = ht_free(file_content_ht);
        return EXIT_FAILURE;
    }
    
    size_t tokens_capacity = default_table_cap;
    size_t current_tokens  =                 0;
    */
    
    char* safe_source = NULL;
    char* token       = strtok_r(source_buffer, " ()\",.!?~<>=:;`-=*0123456789\t\r\n", &safe_source);
    
    while (token != NULL) {
        /*
        if (tokens_capacity <= current_tokens) {
            char** new_tokens = (char**) realloc(tokens, (size_t) (current_tokens * 1.5) * sizeof(char*));
            if (new_tokens == NULL) {
                free(source_buffer);
                fclose(source_file_stream);

                free(tokens);
                file_content_ht = ht_free(file_content_ht);
                return EXIT_FAILURE;
            }
            tokens = new_tokens;
            tokens_capacity = (size_t) (current_tokens * 1.5);
        }
        tokens[current_tokens++] = token; // ! Putting token into the array, so we can access it later
        */
        
        if (ht_insert_key(file_content_ht, token, ht_get(file_content_ht, token) + 1)) {
            break;
        }

        token = strtok_r(NULL, " ()\",.!?~<>=:;`-=*0123456789\t\r\n", &safe_source);
        
    }
    
    fclose(source_file_stream);
    /*
    size_t find_counter = 0;
    for (size_t current_cycle = 0; current_cycle < 64; ++current_cycle) {
        for (size_t token_idx = 0; token_idx < current_tokens; ++token_idx) {
            if (ht_get(file_content_ht, tokens[token_idx]) != NODE_DEFAULT_VALUE) {
                ++find_counter;
            }
        }
    }

    fprintf(stderr, "I successfully found %zu words with 64 loops.\n", find_counter);
    */

    FILE* output_file_stream = fopen(output_file_path, "w");
    if (output_file_stream == NULL) {
        fprintf(stderr, "Can't open output file.\n");

        free(source_buffer);
        //free(tokens);
        
        file_content_ht = ht_free(file_content_ht);
        return EXIT_FAILURE;
    }
    
    for (size_t idx = 0; idx < file_content_ht->capacity; ++idx) {
        if (file_content_ht->nodes[idx] != NULL) {
            print_pairs(file_content_ht->nodes[idx], output_file_stream);
        }
    }
    
    fclose(output_file_stream);
    free(source_buffer);
    //free(tokens);
    file_content_ht = ht_free(file_content_ht);

    return EXIT_SUCCESS;
}  