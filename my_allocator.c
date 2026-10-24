#include <unistd.h>
#include <stddef.h>
#include "my_allocator.h"

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

struct block_meta {
    size_t size;
    int is_free;
    struct block_meta *next;
};

#define META_SIZE sizeof(struct block_meta)
struct block_meta *global_base = NULL;

struct block_meta *find_free_block(struct block_meta **last, size_t size) {
    struct block_meta *current = global_base;
    while (current && !(current->is_free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

struct block_meta *request_space(struct block_meta* last, size_t size) {
    struct block_meta *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    if (request == (void*) -1) return NULL; 

    if (last) last->next = block;
    
    block->size = size;
    block->next = NULL;
    block->is_free = 0;
    return block;
}

void *my_malloc(size_t size) {
    if (size <= 0) return NULL;
    size_t aligned_size = ALIGN(size);
    struct block_meta *block;

    if (!global_base) { 
        block = request_space(NULL, aligned_size);
        if (!block) return NULL;
        global_base = block;
    } else {
        struct block_meta *last = global_base;
        block = find_free_block(&last, aligned_size);
        if (!block) {
            block = request_space(last, aligned_size);
            if (!block) return NULL;
        } else {
            block->is_free = 0;
        }
    }
    return (block + 1); 
}

void my_free(void *ptr) {
    if (!ptr) return;
    struct block_meta* block_ptr = (struct block_meta*)ptr - 1;
    block_ptr->is_free = 1;

    struct block_meta *curr = global_base;
    while (curr != NULL && curr->next != NULL) {
        if (curr->is_free && curr->next->is_free) {
            curr->size += META_SIZE + curr->next->size;
            curr->next = curr->next->next; 
        } else {
            curr = curr->next;
        }
    }
}