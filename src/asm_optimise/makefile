CXXFLAGS = -mavx2 -Wall -Werror -Wno-unused-function -ggdb3 -O3

VALGRIND = valgrind -s --leak-check=full --track-origins=yes --verbose --log-file=valgrind_asm_optimise.txt --show-reachable=yes --track-fds=yes
WINDOWS = C:/Programming/hash_table/txt_files/war_and_peace.txt txt_files/ht_result_asm_optimise.txt
UBUNTU = /mnt/c/Programming/hash_table/txt_files/war_and_peace.txt txt_files/ht_result_asm_optimise.txt

execute: ht_test_asm_optimise
	build/ht_test_asm_optimise $(WINDOWS)

ht_test_asm_optimise: hash_table.o linked_list.o ht_test_asm_optimise.o
	gcc $(CXXFLAGS) -o build/ht_test_asm_optimise build/hash_table.o build/linked_list.o build/ht_test_asm_optimise.o

ht_test_asm_optimise.o: ht_test_asm_optimise.c
	gcc -c ht_test_asm_optimise.c $(CXXFLAGS) -o build/ht_test_asm_optimise.o

hash_table.o: hash_table.h hash_table.c
	gcc -c hash_table.c $(CXXFLAGS) -o build/hash_table.o

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c $(CXXFLAGS) -o build/linked_list.o
     