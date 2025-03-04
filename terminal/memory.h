// -- 
//    Dynamic Memory Library implementation
//    by Kyle brady for Minimal-C
// --

#ifndef MEMORY_H_
#define MEMORY_H_

// Heap/block shifting
#define HEAP_SHIFT(start) ((void*)start + sizeof(heap))
#define BLOCK_SHIFT(start) ((void*)start + sizeof(block))
// tiny and small Heap/block allocation sizes
#define THEAP_ALLOC_SIZE (4 * getpagesize())
#define TBLOCK_SIZE (THEAP_ALLOC_SIZE / 128)
#define SHEAP_ALLOC_SIZE (16 * getpagesize())
#define SBLOCK_SIZE (SHEAP_ALLOC_SIZE / 128)


#include <sys/mman.h>
#include <sys/resource.h>

// Dynamic-Memory function definitions
void allocate(size_t alloc_size);
void deallocate(void ptr);
void reallocate(void ptr, size_t size);
void scaledAllocate(size_t containers, size_t size);

// Mapping and Un-Mapping Memory address' and lengths etc;
void mapMemory(void memAddr, size_t length, int prot, int flags, int fd, off_t 	offset);
void unmapMemory(void memAddr, size_t length);

// Directly limit Memory calls
int getrLimit(int res, struct rlimit rlp);
int setrLimit(int res, const struct rlimit rlp);

typedef struct heap {
	struct heap* prev;
	struct heap* next;
	heapGroup group;
	size_t totalSize;
	size_t availableSize;
	size_t blockCount;
} Heap;

typedef struct block {
	struct block* prev;
	struct block* next;
	size_t dataSize;
	int isFree;
} Block;

#endif
