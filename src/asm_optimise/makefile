#*--------------------------------------------------------------------------------------------------------------------------------
CXXFLAGS = -mavx2 -ggdb3 -O3

WINDOWS   =     C:/Programming/hash_table/txt_files/war_and_peace.txt   txt_files/ht_result_asm_optimize.txt
LINUX     = /mnt/c/Programming/hash_table/txt_files/war_and_peace.txt   txt_files/ht_result_asm_optimize.txt


ifeq ($(OS), Windows_NT) 
	SYSTEM = $(WINDOWS)
else 
	SYSTEM = $(LINUX)	
endif	
#*--------------------------------------------------------------------------------------------------------------------------------


#*--------------------------------------------------------------------------------------------------------------------------------
#? callgrind is a valgrind tool that records the count of instructions and call history in a program's run as a call-graph


CALLGRIND_LOG_FILE = memcheck/callgrind_asm_optimize_bad_hash.txt
CALLGRIND_COMMAND  = valgrind 												 \
					--tool=callgrind 										 \
					--callgrind-out-file=$(CALLGRIND_LOG_FILE)

VALGRIND_LOG_FILE = memcheck/valgrind_asm_optimize.txt
VALGRIND_COMMAND = valgrind 												 \
					-s 													 	 \
					--leak-check=full 										 \
					--track-origins=yes 									 \
					--verbose 									 			 \
					--log-file=$(VALGRIND_LOG_FILE)			                 \
					--show-reachable=yes 									 \
					--track-fds=yes
#*--------------------------------------------------------------------------------------------------------------------------------

execute: ht_test_asm_optimize
#	build/ht_test_asm_optimize $(SYSTEM)
#	$(VALGRIND_COMMAND)  build/ht_test_asm_optimize  $(SYSTEM)
	$(CALLGRIND_COMMAND) build/ht_test_asm_optimize  $(SYSTEM)


ht_test_asm_optimize: hash_table.o linked_list.o ht_test_asm_optimize.o
	gcc $(CXXFLAGS) -o 	build/ht_test_asm_optimize                           \
						build/hash_table.o                                   \
						build/linked_list.o                                  \
						build/ht_test_asm_optimize.o

ht_test_asm_optimize.o: ht_test_asm_optimize.c
	gcc -c ht_test_asm_optimize.c $(CXXFLAGS) -o build/ht_test_asm_optimize.o

hash_table.o: hash_table.h hash_table.c
	gcc -c hash_table.c           $(CXXFLAGS) -o build/hash_table.o

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c          $(CXXFLAGS) -o build/linked_list.o	 