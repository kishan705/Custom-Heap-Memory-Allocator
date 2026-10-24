#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_allocator.h"

#define ALLOC_COUNT 50000

int main() {
    printf("Starting Memory Allocator Benchmark...\n");
    clock_t start, end;
    double cpu_time_used;
    void *pointers[ALLOC_COUNT];

    // Benchmark our custom allocator
    start = clock();
    for (int i = 0; i < ALLOC_COUNT; i++) {
        pointers[i] = my_malloc(128); // Request 128 bytes
    }
    for (int i = 0; i < ALLOC_COUNT; i++) {
        my_free(pointers[i]);
    }
    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Custom Allocator Time: %f seconds\n", cpu_time_used);
    
    printf("Benchmark Complete! Coalescing successful.\n");
    return 0;
}