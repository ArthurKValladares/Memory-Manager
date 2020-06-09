// Contains code for print_heap function.
// Return the block of storage at location p to the
//  pool of available free storage.

#include "mem.h"

void print_heap(FILE * f) {
  // Iterator for free list
  free_node *node = head;
  while (node != NULL) {
    fprintf(f, "%p: %lu\n", node, node->size);
    node = node->next;
  }
}
