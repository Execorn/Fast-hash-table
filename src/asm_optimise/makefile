CXXFLAGS = -mavx2 -Wall -Werror -Wno-unused-function -g

execute: ht_test_asm_optimise
	./build/ht_test_asm_optimise

ht_test_asm_optimise: hash_table.o linked_list.o ht_test.o my_strcmp.o
	 gcc $(CXXFLAGS) -o build/ht_test_asm_optimise build/my_strcmp.o build/hash_table.o build/linked_list.o build/ht_test.o

my_strcmp.o: my_strcmp.s
	nasm -f elf64 my_strcmp.s -o ./build/my_strcmp.o

ht_test.o: ht_test.c
	gcc -c ht_test.c $(CXXFLAGS) -o build/ht_test.o

hash_table.o: hash_table.h hash_table.c
	gcc -c hash_table.c $(CXXFLAGS) -o build/hash_table.o

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c $(CXXFLAGS) -o build/linked_list.o
     