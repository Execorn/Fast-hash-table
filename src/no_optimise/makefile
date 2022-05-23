#*--------------------------------------------------------------------------------------------------------------------------------
CXXFLAGS = -ggdb3

WINDOWS   =     C:/Programming/hash_table/txt_files/war_and_peace.txt   txt_files/ht_result.txt
LINUX     = /mnt/c/Programming/hash_table/txt_files/war_and_peace.txt   txt_files/ht_result.txt


ifeq ($(OS), Windows_NT) 
	SYSTEM = $(WINDOWS)
else 
	SYSTEM = $(LINUX)	
endif	
#*--------------------------------------------------------------------------------------------------------------------------------


#*--------------------------------------------------------------------------------------------------------------------------------
#? callgrind is a valgrind tool that records the count of instructions and call history in a program's run as a call-graph


CALLGRIND_LOG_FILE = memcheck/callgrind.txt
CALLGRIND_COMMAND  = valgrind 												 \
					--tool=callgrind 										 \
					--callgrind-out-file=$(CALLGRIND_LOG_FILE)

VALGRIND_LOG_FILE = memcheck/valgrind.txt
VALGRIND_COMMAND = valgrind 												 \
					-s 													 	 \
					--leak-check=full 										 \
					--track-origins=yes 									 \
					--verbose 									 			 \
					--log-file=$(VALGRIND_LOG_FILE)			                 \
					--show-reachable=yes 									 \
					--track-fds=yes
#*--------------------------------------------------------------------------------------------------------------------------------

execute: ht_test
#	build/ht_test $(SYSTEM)
#	$(VALGRIND_COMMAND)  build/ht_test  $(SYSTEM)
#	$(CALLGRIND_COMMAND) build/ht_test  $(SYSTEM)


ht_test: hash_table.o linked_list.o ht_test.o
	gcc $(CXXFLAGS) -o 	build/ht_test                           			 \
						build/hash_table.o                                   \
						build/linked_list.o                                  \
						build/ht_test.o

ht_test.o: ht_test.c
	gcc -c ht_test.c $(CXXFLAGS) -o build/ht_test.o

hash_table.o: hash_table.h hash_table.c
	gcc -c hash_table.c           $(CXXFLAGS) -o build/hash_table.o

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c          $(CXXFLAGS) -o build/linked_list.o	 