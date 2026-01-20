#ifndef MEMORY_H
#define MEMORY_H

// Shifts the heap or block pointer depending on there current size.
#define HEAP_SHIFT(start) ((void*)start + sizeof(heap))
#define BLOCK_SHIFT(start) ((void*)start + sizeof(block))

// Tiny head allocation of 4 bytes
#define T_HEAP_ALLOC_SIZE (4 * getpagesize())
#define T_BLOCK_SIZE (THEAP_ALLOC_SIZE / 128)

// Small Heap allocation block of 16 bytes depending on the page size
#define S_HEAP_ALLOC_SIZE (16 * getpagesize())
#define S_BLOCK_SIZE (SHEAP_ALLOC_SIZE / 128)

#include <sys/mman.h>
#include <sys/resource.h>

// Allocates a particular size in memory via system calls.
void editor_allocate(size_t alloc_size);

// Works like 'Free'.
void editor_deallocate(void ptr);

// Reallocates a new size in memory from an already existing reference
void editor_reallocate(void ptr, size_t size);

void editor_scaled_allocate(size_t containers, size_t size);

// Mapping and un-Mapping memory address' from the heap
void mem_map(void memAddr, size_t length, int prot, int flags, int fd, off_t 	offset);
void umem_map(void memAddr, size_t length);

// Memory Limits
int getrLimit(int res, struct rlimit rlp);
int setrLimit(int res, const struct rlimit rlp);

typedef struct heap {
	struct heap* prev;	// Points to the previous heap value
	struct heap* next;	// Points to the next heap value
		
	heapGroup group;

	size_t totalSize;
	size_t availableSize;
	size_t blockCount;
} Heap;

typedef struct block {
	struct block* prev;	// Points to the previous block 
	struct block* next;	// Points to the next block
				// Represented as a Linked List
	size_t dataSize;
	int isFree;		// Check when the memory is free
				// this will be important later for syntax highlighting as well;
} Block;

#endif
