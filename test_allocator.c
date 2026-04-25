#include "my_alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

void test_basic_alloc_free() {
    printf("Running Basic Alloc/Free Test...\n");
    void *p1 = my_malloc(128);
    assert(p1 != NULL);
    memset(p1, 0xAA, 128);
    
    void *p2 = my_malloc(256);
    assert(p2 != NULL);
    
    my_free(p1);
    my_free(p2);
    printf("Basic Alloc/Free Passed.\n");
}

void test_coalescing_and_checkerboard() {
    printf("Running Fragmentation Stress Test...\n");
    int N = 1000;
    void *ptrs[1000];
    
    for (int i = 0; i < N; i++) {
        ptrs[i] = my_malloc(64);
        assert(ptrs[i] != NULL);
    }
    
    for (int i = 0; i < N; i += 2) {
        my_free(ptrs[i]);
    }
    
    for (int i = 1; i < N; i += 2) {
        my_free(ptrs[i]);
    }
    
    void *large = my_malloc(32000);
    assert(large != NULL);
    my_free(large);
    
    printf("Fragmentation Stress Test Passed.\n");
}

void test_calloc_realloc() {
    printf("Running Calloc/Realloc Test...\n");
    int *arr = (int *)my_calloc(10, sizeof(int));
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == 0);
    }
    
    arr = (int *)my_realloc(arr, 20 * sizeof(int));
    assert(arr != NULL);
    my_free(arr);
    printf("Calloc/Realloc Passed.\n");
}

void benchmark() {
    printf("Running Benchmark Trace...\n");
    clock_t start = clock();
    
    void *ptrs[500];
    for(int i = 0; i < 500; i++) {
        ptrs[i] = NULL;
    }
    
    for(int i = 0; i < 10000; i++) {
        int idx = rand() % 500;
        if (ptrs[idx]) {
            my_free(ptrs[idx]);
            ptrs[idx] = NULL;
        } else {
            size_t sz = (rand() % 1024) + 8;
            ptrs[idx] = my_malloc(sz);
        }
    }
    
    for(int i = 0; i < 500; i++) {
        if (ptrs[i]) {
            my_free(ptrs[i]);
        }
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Benchmark finished in %.4f seconds.\n", time_spent);
    my_heap_dump();
}

int main() {
    srand(time(NULL));
    printf("=== Custom Memory Allocator Tests ===\n");
    test_basic_alloc_free();
    test_calloc_realloc();
    test_coalescing_and_checkerboard();
    benchmark();
    printf("=== All tests completed ===\n");
    return 0;
}
