// Header file with declarations of internal implementation details
// shared by more than one of theprogram files.
// This is information that is not part of the public interface,
// which is declared in file mem.h.


#ifndef MEM_IMPL_H
#define MEM_IMPL_H

#include <inttypes.h>

// smallest possible block of memory
// all blocks are multiples of MIN_BLOCK
#define MIN_BLOCK 16
// smallest memory chunk to be acquired from the underlying system (malloc_node)
#define MIN_CHUNK 4096

// Structure of node used in free list.
// Holds a uinptr_t with the size of the blocl of memory being held by the node,
// and a free_node pointer pointing to the next free_node.
typedef struct free_node {
  uintptr_t size;
  struct free_node* next;
} free_node;

/*---helper functions for getmem---*/
// Splits a free_node into two nodes of size 'size_requested'
// and the remaining memory.
// The node with remaining memory is added to the free list.
void split_node(free_node *node, uintptr_t size_requested);

// creates new free_node with size of parameter 'size'
free_node *malloc_node(uintptr_t size);

// iterates through the list to find the most correctly sized node
free_node* get_node(uintptr_t minSize);

// deletes node from free list. returns 1 if node was found
int delete_node(free_node* node);

// Rounds the size given to the nearesr multiple of 16 above.
uintptr_t round_16(uintptr_t size);
/*---end of helper functions for getmem ---*/

/*---helper functions for freemem---*/
// Starting at node, merges it with all neighbors to the right in free list
// unbtil it cannot merge anymore.
void combine_nodes(free_node *node);
/*---end of helper functions for freemem ---*/

// Store statistics about the current state of the memory manager
// in the three integer variables whose addresses are given as arguments.
// The infomation store is the total size, total amount of free storage
// and number of free blocks
void get_mem_stats(uintptr_t* total_size,
                   uintptr_t* total_free,
                   uintptr_t* n_free_blocks);

// frees all memory allocations from malloc
void free_all_mem();

// doubles the length of chunk_pointers
void double_chunk_pointers();
/*---end of helper functions for freemem ---*/

// Checks for possible problems with the free list data structure.
void check_heap();

#endif
