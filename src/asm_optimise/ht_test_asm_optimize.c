#include "hash_table.h"
#include "macro.h"

#include <sys/stat.h>
#include <immintrin.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>


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
    char* source_buffer_ptr = source_buffer;

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

    size_t source_scanned_bytes = 0;
    if ((source_scanned_bytes = fread(source_buffer, 1, source_file_size_in_bytes, source_file_stream)) != source_file_size_in_bytes) {
        fprintf(stderr, "Error reading '%s': expected %zu bytes, got %zu.\n",   source_file_path, 
                                                                                source_scanned_bytes, 
                                                                                source_file_size_in_bytes);
        
        free(source_buffer);
        fclose(source_file_stream);
        return EXIT_FAILURE;
    }

    fclose(source_file_stream);
    // ----------------------------------------------------------------
    size_t tokens_counter =   0;
    size_t max_tokens     = 100;

    char** tokens = (char**) calloc(max_tokens, sizeof(char*));
    if (tokens == NULL) {
        fprintf(stderr, "Can't allocate memory for tokens array.\n");
        
        free(source_buffer);
        fclose(source_file_stream);
        return EXIT_FAILURE;
    }

    char prev    = '\0';
    char current = '\0';

    for (size_t source_idx = 0; source_idx < source_scanned_bytes; ++source_idx) {
        current = source_buffer[source_idx];
        // ! if prev is the end of C-string, current is the beginning of new string
        if (prev == '\0') {
            // * if current if end of C-string, string is empty, we just skip it
            if (current == '\0') {
                continue; 
            }

            if (tokens_counter >= max_tokens) {
                size_t new_max_tokens = (size_t) (max_tokens * 1.5);
                char** new_tokens = (char**) realloc(tokens, sizeof(char*) * new_max_tokens);
                if (new_tokens == NULL) {
                    fprintf(stderr, "Not enough memory for tokens array.\n");

                    free(source_buffer);
                    free(tokens);

                    return EXIT_FAILURE;
                } else {
                    tokens = new_tokens;
                    max_tokens = new_max_tokens;
                }
            } 
            // ? put pointer to source_buffer[source_idx] to tokens
            tokens[tokens_counter++] = source_buffer + source_idx;
        } 

        prev = current;
    }

    char* aligned_tokens = (char*) aligned_alloc(32, tokens_counter * ALIGNED_TOKEN_SIZE * sizeof(char));
    if (aligned_tokens == NULL) {
        fprintf(stderr, "Not enough memory for aligned tokens.\n");

        free(tokens);
        free(source_buffer);

        return EXIT_FAILURE;
    }

    for (size_t token_idx = 0; token_idx < tokens_counter; ++token_idx) {
        strncpy(aligned_tokens + token_idx * ALIGNED_TOKEN_SIZE, tokens[token_idx], ALIGNED_TOKEN_SIZE);
    }

    ht_t* file_content_ht = ht_init(DEFAULT_HT_CAPACITY);
    if (file_content_ht == NULL) {
        fprintf(stderr, "Can't create hash table.\n");

        free(tokens);
        free(source_buffer);
        free(aligned_tokens);
        return EXIT_FAILURE;
    }

    for (size_t token_idx = 0; token_idx < tokens_counter; ++token_idx) {
        char* current_token        = aligned_tokens + token_idx * ALIGNED_TOKEN_SIZE;
        size_t current_token_value = ht_get(file_content_ht, current_token);

        // ! ht_get returns 0 as default value (if key not in table), so we just need to insert key with value (ht_get + 1)
        int is_success = ht_insert_key(file_content_ht, current_token, current_token_value + 1);
        if (is_success != 0) {
            fprintf(stderr, "Insertion of token '%s' with value %zu failed.\n", current_token, current_token_value + 1);

            free(tokens);
            free(source_buffer);
            free(aligned_tokens);
            file_content_ht = ht_free(file_content_ht);
            return EXIT_FAILURE;
        }
    }   

    size_t total_counter = 0;
    for (size_t pass = 0; pass < 32; ++pass) {
        for (size_t token_idx = 0; token_idx < tokens_counter; ++token_idx) {
            total_counter += ht_get(file_content_ht, aligned_tokens + token_idx * ALIGNED_TOKEN_SIZE);
        }
    }

    fprintf(stderr, "Scanned %zu usages.\n", tokens_counter);

    FILE* output_file_stream = fopen(output_file_path, "w");
    if (output_file_stream == NULL) {
        fprintf(stderr, "Can't open '%s'.\n", output_file_path);
        
        free(tokens);
        free(source_buffer);
        free(aligned_tokens);
        file_content_ht = ht_free(file_content_ht);
        return EXIT_FAILURE;
    }

    size_t printed_nodes = ht_print_content(file_content_ht, output_file_stream);
    fprintf(stderr, "Printed %zu nodes to '%s'.\n", printed_nodes, output_file_path);

    free(tokens);
    free(source_buffer);
    fclose(output_file_stream);
    free(aligned_tokens);
    file_content_ht = ht_free(file_content_ht);

    return EXIT_SUCCESS;
}  