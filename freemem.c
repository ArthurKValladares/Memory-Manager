// Contains freemem method and its helper methods.
// Freemem Return the block of storage at location p to the pool of
//  available free storage.

#include "mem.h"

void freemem(void* p) {
  if (p != NULL) {
    check_heap();
    // Point to memory sizeof(free_node) before 'p' pointer
    // Should correspond to start of free_node associated with p
    free_node *node = (free_node *)((char *) p - sizeof(free_node));
    // Memory adress of node from p
    uintptr_t adress = (uintptr_t) node;
    // Iterator for free list
    free_node *curr = head;
    if (head == NULL) {
      head = node;
      return;
    }

    if (adress < (uintptr_t) head) {
      node->next = head;
      head = node;
      combine_nodes(head);
      check_heap();
      return;
    }

    if (head->next == NULL) {
      if ((uintptr_t) head > adress) {
        node->next = head;
        head = node;
      } else {
        head->next = node;
        node->next = NULL;
      }
      combine_nodes(head);
    } else {
      while (curr != NULL && curr->next != NULL) {
        uintptr_t prev_adress = (uintptr_t) curr;
        uintptr_t next_adress = (uintptr_t) curr->next;
        if (adress > prev_adress && adress < next_adress) {
          break;
        }
        curr = curr->next;
      }
      // Add node to list.
      node->next = curr->next;
      curr->next = node;
      // Then try to combine nodes.
      // Starting from right to not try to combine new node twice.
      combine_nodes(node);
      combine_nodes(curr);
    }
    check_heap();
  }
}


// Starting at node, merges it with all neighbors to the right in free list
// unbtil it cannot merge anymore.
void combine_nodes(free_node *node) {
  while (node->next != NULL) {
    // If two nodes are next to each other in memory.
    if ((char *) node + sizeof(free_node) + node->size == (char *) node->next) {
      // set add size of second node to size of first node
      node->size = node->size + sizeof(free_node) + node->next->size;
      // save next of second node
      free_node *temp = node->next->next;
      // set next of second node to NULL and size to 0
      node->next->next = NULL;
      node->next->size = 0;
      // Set next of first node to next of second node
      node->next = temp;
    } else {
      break;
    }
  }
}

void free_all_mem() {
  for (int i = 0; i < malloc_call_cnt; i++) {
    free((void*) chunk_pointers[i]);
  }
  free(chunk_pointers);
}
