#include "my_alloc.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define HEADER_SIZE 32
#define FOOTER_SIZE 8

typedef struct block_header {
    size_t size;
    uint8_t is_free;
    uint8_t _pad[7];
    struct block_header *prev;
    struct block_header *next;
} block_header_t;

typedef struct {
    void *heap_start;
    size_t heap_size;
    block_header_t *free_list_head;
    size_t current_in_use;
    size_t peak_usage;
    size_t total_allocated;
    size_t num_free_blocks;
    size_t num_alloc_blocks;
} heap_t;

extern heap_t g_heap;

void my_heap_dump(void) {
    if (!g_heap.heap_start) {
        printf("Heap uninitialized.\n");
        return;
    }

    printf("--- HEAP DUMP ---\n");
    printf("Heap Start: %p, Heap Size: %zu\n", g_heap.heap_start, g_heap.heap_size);
    printf("Current In Use: %zu, Peak Usage: %zu\n", g_heap.current_in_use, g_heap.peak_usage);
    printf("Allocated Blocks: %zu, Free Blocks: %zu\n", g_heap.num_alloc_blocks, g_heap.num_free_blocks);

    char *current = (char *)g_heap.heap_start;
    char *heap_end = current + g_heap.heap_size;
    
    size_t total_free_space = 0;
    size_t largest_free_block = 0;

    while (current < heap_end) {
        block_header_t *hdr = (block_header_t *)current;
        if (hdr->size == 0) break; 

        printf("Block %p | Size: %zu | Status: %s | Payload: %p\n",
               (void *)hdr, hdr->size, hdr->is_free ? "F" : "A", (void *)(current + HEADER_SIZE));

        if (hdr->is_free) {
            total_free_space += hdr->size;
            if (hdr->size > largest_free_block) {
                largest_free_block = hdr->size;
            }
        }

        current += HEADER_SIZE + hdr->size + FOOTER_SIZE;
    }

    double ef = 0.0;
    if (total_free_space > 0) {
        ef = 1.0 - ((double)largest_free_block / total_free_space);
    }
    
    printf("External Fragmentation (Larson): %.4f\n", ef);
    printf("-----------------\n");
}
