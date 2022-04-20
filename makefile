execute: hash_table.exe
	./build/hash_table.exe

hash_table.exe: hash_table.o linked_list.o
	 gcc -o build/hash_table.exe build/hash_table.o build/linked_list.o -mavx2 -O3

hash_table.o: hash_table.h hash_table.c
	 gcc -c hash_table.c -o build/hash_table.o -mavx2 -O3

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c -o build/linked_list.o -mavx2 -O3
     