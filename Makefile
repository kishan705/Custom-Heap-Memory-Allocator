CC = gcc
CFLAGS = -Wall -Wno-deprecated-declarations

all: benchmark

benchmark: benchmark.c my_allocator.c
	$(CC) $(CFLAGS) benchmark.c my_allocator.c -o benchmark

clean:
	rm -f benchmark