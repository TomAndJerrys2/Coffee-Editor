// Libraries
#include <stdio.h>
#include "../headers/memory.h"

// Allocation definition
#define _BIT_SIZE_ 1024 * 1024 
#define MAX_ALLOCATION 20
static unsigned char memory[_BIT_SIZE_];

// alloc num and the heap address
static int g_allocatedNum = 0;
static int g_heap_base_addr = 0;

typedef struct allocate_info {
	int address; // address of the allocated memory
	int size; // signed size of said memory
} allocateInfo;

allocateInfo metaData[MAX_ALLOCATION] = {0};

void copymetaData() {
	for(int j = MAX_ALLOCATION - 1; j > (index + 1); j--) {
		memcpy(&metaData[j], &metaData[j - 1], sizeof(metaData));
	}
}

void* allocate(int alloc_size) {
	int num, index, gap, initGap = 0;

	int flag = 0; // bool - false
	int initFlag = 0; // bool - false
	void* address = NULL;
	int heapIndex = 0;
	allocateInfo tempInfo = {0};

	if(g_allocatedNum >= MAX_ALLOCATION) { return NULL; }

	for(index = 0; index < g_allocatedNum; index++) {
		if(metaData[index + 1].address != 0) {
			initGap = metaData[0].address - g_heap_base_addr; // check
			if(initGap >= alloc_size) {
				initFlag = 1; // true
				break;
			} else {
				gap = (metaData[index + 1].address - (metaData[index].address +
						metaData[index].size));
				if(gap >= size) {
					flag = 1; // true
					break;
				}
			}
		}
	}

	if(flag == 1) {
		heapIndex = ((metaData[index].address + metaData[index].size) - g_heap_base_addr);
		copymetaData();	
	} else if(initFlag == 1) {
		heapIndex = 0;
		copymetaData();
	} else {
		if(g_allocatedNum != 0) {
			heapIndex = ((metaData[index - 1].address + metaData[index - 1].size)
					- g_heap_base_addr);
		} else {
			heapIndex = 0;
		}

		address = &memory[heapIndex];
		metaData[index].address = g_heap_base_size + heapIndex;
		metaData[index].size = size;
		g_allocatedNum += 1;
		return address;
	}
}
