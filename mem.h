// Header file containing declarations of the public
// functions in the memory manager.

#ifndef MEM_H
#define MEM_H

#include<stdio.h>
#include<stdlib.h>

#include "mem_impl.h"

// free list head
// declared with keyword exern in header, because it will be used in both
// getmem and freemem. Will be 'redeclared' and initialized in getmem
// the linked list will always be sorted in order of ascending memmory address
extern free_node* head;


// Used to remember the addresses of the memory chunks received by malloc
// so that they can be freed at the end of the program
extern uintptr_t* chunk_pointers;
// the number of times malloc has been called,
// aka. number of pointers in chunk_pointers
extern int malloc_call_cnt;

// total amount of storage in bytes acquired by the memory manager so far
// to use in satisfying allocation requests. (In other words, the total amount
// requested from the underlying system.)
extern int mem_stat_total_size;

// Returns pointer to new block of storage with at least size bytes of memory.
void* getmem(uintptr_t size);

// Returns block of storage at location p to the pool of available free storage.
void freemem(void* p);

// Print a formatted listing on file f showing the blocks on the free list.
void print_heap(FILE * f);

#endif
