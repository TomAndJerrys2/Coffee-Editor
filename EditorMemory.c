#include "EditorMemory.h"

#define BIT_SIZE 1024 * 1024 
#define MAX_ALLOCATION 20	// can be dynamic through syscalls.

#define None (void*(0))

static unsigned char memory[BIT_SIZE];

// alloc num and the heap address
static int g_allocatedNum = 0;
static int g_heap_base_addr = 0;

typedef struct {

	int address; 		// address of the allocated memory
				//
	signed int size; 	// size of allocation

} allocation_info;

allocateInfo metaData[MAX_ALLOCATION] = {0};

void copy_metadata() {
	for(int j = MAX_ALLOCATION - 1; j > (index + 1); j--) {
		memcpy(&metaData[j], &metaData[j - 1], sizeof(metaData));
	}
}

void* editor_allocate(int alloc_size) {

	int num = 0, 
	    index = 0, 
	    gap = 0, 
	    init_gap = 0;

	unsigned int flag = 0;		// false
	unsigned int init_flag = 0;	// false
				
	void* address = None;	
	size_t heap_index = 0;

	allocatiom_info temp_info = 0;

	if(g_allocatedNum >= MAX_ALLOCATION) { return NULL; }

	for(index = 0; index < g_allocatedNum; index++) {

		if(metaData[index + 1].address != 0) {
			init_gap = metaData[0].address - g_heap_base_addr;

			if(init_gap >= size) {
				init_flag = 1;
				break; 
			}
	
			else {
				// Gap inbetween memory address'
				gap = (metaData[index + 1].address 
						- (metaData[index].address 
							+ metaData[index].size));
				
				// If the gap has grown greater than our allocation size
				// set flag to 1, to show out of bounds - hence 'break'
				if(gap >= size) {
					flag = 1;
					break;
				}
			}
		}
	}
	
	// 
	if(flag == 1) {
		heapIndex = ((metaData[index].address 
					+ metaData[index].size) 
						- g_heap_base_addr);
		copy_metadata();	
	} 

	else if(initFlag == 1) {
		heapIndex = 0;

		copy_metadata();
	} 

	else {
		if(g_allocatedNum != 0) {
			heapIndex = ((metaData[index - 1].address 
						+ metaData[index - 1].size)
							- g_heap_base_addr);
		} 

		else {
			heapIndex = 0;
		}

		address = &memory[heapIndex];
		metaData[index].address = g_heap_base_size + heapIndex;
		metaData[index].size = size;
		g_allocatedNum += 1;

		return address;
	}
}
