CC = gcc
CFLAGS = -Wall -Wno-deprecated-declarations

.PHONY: all clean test

all: benchmark

benchmark: benchmark.c my_allocator.c my_allocator.h
	$(CC) $(CFLAGS) benchmark.c my_allocator.c -o benchmark

test_allocator: test_allocator.c my_allocator.c my_allocator.h
	$(CC) $(CFLAGS) test_allocator.c my_allocator.c -o test_allocator

test: test_allocator
	./test_allocator

clean:
	rm -f benchmark test_allocator
