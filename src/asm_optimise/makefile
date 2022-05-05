CXXFLAGS = -mavx2 -Wall -Werror -Wno-unused-function -ggdb3 -O3

CALLGRIND = valgrind --tool=callgrind --callgrind-out-file=memcheck/callgrind_asm_optimize
VALGRIND = valgrind -s --leak-check=full --track-origins=yes --verbose --log-file=memcheck/valgrind_asm_optimize.txt --show-reachable=yes --track-fds=yes
WINDOWS = C:/Programming/hash_table/txt_files/war_and_peace.txt txt_files/ht_result_asm_optimize.txt
UBUNTU = /mnt/c/Programming/hash_table/txt_files/war_and_peace.txt txt_files/ht_result_asm_optimize.txt

execute: ht_test_asm_optimize
	$(VALGRIND)  build/ht_test_asm_optimize  $(UBUNTU)
	$(CALLGRIND) build/ht_test_asm_optimize  $(UBUNTU)


ht_test_asm_optimize: hash_table.o linked_list.o ht_test_asm_optimize.o
	gcc $(CXXFLAGS) -o build/ht_test_asm_optimize build/hash_table.o build/linked_list.o build/ht_test_asm_optimize.o

ht_test_asm_optimize.o: ht_test_asm_optimize.c
	gcc -c ht_test_asm_optimize.c $(CXXFLAGS) -o build/ht_test_asm_optimize.o

hash_table.o: hash_table.h hash_table.c
	gcc -c hash_table.c $(CXXFLAGS) -o build/hash_table.o

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c $(CXXFLAGS) -o build/linked_list.o
     