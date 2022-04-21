#include "hash_table.h"

int main() {
    ht_t* my_ht = ht_init(100);
    ht_insert_key(my_ht, "nice", "cock");
    ht_insert_key(my_ht, "awesome", "balls");

    printf("hey bro, %s %s, %s %s\n", "nice", ht_get(my_ht, "nice"), "awesome", ht_get(my_ht, "awesome"));

    my_ht = ht_free(my_ht);

    ht_insert_key(my_ht, "i cant", "insert it here");
    return 0;
}