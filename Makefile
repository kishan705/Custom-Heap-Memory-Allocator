CC = gcc
CFLAGS = -Wall -Wno-deprecated-declarations

.PHONY: all clean

all: benchmark

benchmark: benchmark.c my_allocator.c my_allocator.h
	$(CC) $(CFLAGS) benchmark.c my_allocator.c -o benchmark

clean:
	rm -f benchmark
