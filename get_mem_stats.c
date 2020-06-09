// Contains code for the get_mem_stats function
// updates variables total_size, total_free, and n_free_blocks
// given to it as arguments

#include "mem.h"

// Store statistics about the current state of the memory manager in the
// three integer variables whose addresses are given as arguments.
void get_mem_stats(uintptr_t* total_size,
                   uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {

  // total amount of storage in bytes acquired by the memory manager so far.
  *total_size = mem_stat_total_size;
  // total amount of storage on free list. Includes node size.
  *total_free = 0;
  // total number of individual blocks currently stored on the free list.
  *n_free_blocks = 0;

  // iterates through list and updates *total_free and *n_free_blocks for each step
  free_node* node = head;
  while(node != NULL) {
    (*n_free_blocks)++;
    *total_free += node->size + sizeof(free_node);
    node = node->next;
  }
}
