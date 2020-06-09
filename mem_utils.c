// Contains code for check_heap function
// Check for possible problems with the free list data structure.

#include "mem.h"

#include <assert.h>

void check_heap() {
  // Iterator for free list
  free_node *curr = head;
  while (curr != NULL) {
    assert(curr->size >= MIN_BLOCK);
    if (curr->next != NULL) {
      assert((uintptr_t) curr->next > (uintptr_t) curr);
      uintptr_t adress_after_end = (uintptr_t) curr + sizeof(free_node) +
                                   curr->size;
      assert(adress_after_end != (uintptr_t) curr->next);
      assert(adress_after_end < (uintptr_t) curr->next);
    }
    curr = curr->next;
  }
}
