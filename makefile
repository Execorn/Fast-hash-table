CXXFLAGS = -mavx2 -O3 -Wall -Werror -Wno-unused-function

execute: ht_test.exe
	./build/ht_test.exe

ht_test.exe: hash_table.o linked_list.o ht_test.o
	 gcc -o build/ht_test.exe build/hash_table.o build/linked_list.o build/ht_test.o $(CXXFLAGS)

ht_test.o: ht_test.c
	gcc -c ht_test.c -o build/ht_test.o $(CXXFLAGS)

hash_table.o: hash_table.h hash_table.c
	gcc -c hash_table.c -o build/hash_table.o $(CXXFLAGS)

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c -o build/linked_list.o $(CXXFLAGS)
     