/* Regression tests for my_malloc / my_free.  Run with: make test */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "my_allocator.h"

static int failures = 0;

#define CHECK(cond, msg) do {                                   \
        if (cond) { printf("  PASS  %s\n", msg); }               \
        else      { printf("  FAIL  %s\n", msg); failures++; }   \
    } while (0)

static void test_basic_alloc_and_reuse(void) {
    char *a = my_malloc(100);
    CHECK(a != NULL, "my_malloc(100) returns memory");
    CHECK(((uintptr_t)a % 8) == 0, "returned pointer is 8-byte aligned");
    memset(a, 0xAB, 100);
    my_free(a);
    char *b = my_malloc(100);
    CHECK(b == a, "a freed block is reused for a same-sized request");
    my_free(b);
}

static void test_zero_and_huge_sizes(void) {
    CHECK(my_malloc(0) == NULL, "my_malloc(0) returns NULL");
    CHECK(my_malloc(SIZE_MAX) == NULL, "my_malloc(SIZE_MAX) returns NULL");
    CHECK(my_malloc(SIZE_MAX - 3) == NULL, "my_malloc(SIZE_MAX - 3) returns NULL");
    CHECK(my_malloc((size_t)INTPTR_MAX + 1) == NULL,
          "my_malloc(INTPTR_MAX + 1) returns NULL");
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Running allocator tests...\n");
    test_basic_alloc_and_reuse();
    test_zero_and_huge_sizes();
    if (failures) { printf("%d test(s) failed\n", failures); return 1; }
    printf("All tests passed\n");
    return 0;
}
