// Contains code for getmem and its helper method
// getmem returns an appropriately chunk of memory from either the freelist
// or the underlying system, then calls it's helper functions to resize
// the memory.

#include "mem.h"
#include "limits.h"

// free list head
free_node* head = NULL;

uintptr_t* chunk_pointers = NULL;
int malloc_call_cnt = 0;
// temporary max length for chunk_pointers,
// will be doubled if it exceeds this amount
int chunk_pointers_max_len = 256;

int mem_stat_total_size = 0;

void* getmem(uintptr_t size_requested) {
  if (size_requested <= 0) return NULL;
  check_heap();
  size_requested = round_16(size_requested);
  // get node from free list
  free_node *node = get_node(size_requested);

  // if there is no large enough node on free list, allocate a new chunk
  if (node == NULL) {
    node = malloc_node((size_requested > MIN_CHUNK) ? size_requested : MIN_CHUNK);
    // Add node to free list, combine with other memory if possible
    freemem((char *)node + sizeof(free_node));
    // In the case where freemem merges the node to the left, the node
    // will point to the middle of memory.
    // Therefore we must search the free list again.
    node = get_node(size_requested);

    // printf("head pointer: %ld\n", (long int) node);
    // printf("head pointer: %ld; size: %ld\n", (long int) node, node->size);
    if (node == NULL) {
      fprintf(stderr, "Error: In getmem\n");
      fprintf(stderr, "       get_node returns NULL when it shouldn't\n");
      fprintf(stderr, "       Exiting program.\n");
      exit(EXIT_FAILURE);
    }
  }

  // If node->size is too big, split it
  if (node->size > ((size_requested > MIN_BLOCK) ? size_requested : MIN_BLOCK)
      + sizeof(free_node) + MIN_BLOCK) {
    split_node(node, size_requested);
  }

  // delete node from free list
  delete_node(node);

  // printf("head pointer: %ld; size: %ld\n", (long int) node, node->size);
  check_heap();
  // we need a way to release all these calls to malloc once the program ends
  return (char *) node + sizeof(free_node);
}

// Splits node in free list into a node with size_requested, and a new node
// with the remaining size (size - size_requested - sizeof(free_node))
void split_node(free_node *node, uintptr_t size_requested) {
  //NOTE: Way too many comments for now, they are here just to explain implementation.
  // Will remove most of them later.

  // Creates new node pointinf to memory space after node of size_requested.
  free_node *new_node = (free_node *)((char *) node + sizeof(free_node) + size_requested);
  // Sets size of new node
  new_node->size = node->size - size_requested - sizeof(free_node);
  // Sets size of old node
  node->size = size_requested;
  // points new node to next of old node
  new_node->next = node->next;
  // points next of old node to new node
  node->next = new_node;
}

// TODO: Assumes size is multiple of 16
// Change to handle cases with different sizes.
free_node *malloc_node(uintptr_t size) {
  // if first call to function, initialize chunk pointers
  if (malloc_call_cnt == 0) chunk_pointers = malloc(chunk_pointers_max_len * sizeof(uintptr_t));

  // allocate space for new node and initialize variables
  free_node *new_node = (free_node *) malloc(size + sizeof(free_node));
  new_node->size = size;
  new_node->next = NULL;

  // update global variable mem_stat_total_size
  mem_stat_total_size += size + sizeof(free_node);

  // add the pointer returned by malloc to the chunk_pointers array
  chunk_pointers[malloc_call_cnt] = (uintptr_t) new_node;
  malloc_call_cnt++;
  // double the length of chunk_pointers if malloc_call_cnt is larger than the array
  if (malloc_call_cnt > chunk_pointers_max_len) double_chunk_pointers();

  return new_node;
}


// iterates through the list to find the most correctly sized node
free_node* get_node(uintptr_t minSize) {
  if (head == NULL) return NULL;

  // pointer to iterate through list
  free_node* node = head;
  // current best memory fit. Memory is not divided further in this function
  uintptr_t bestSizeFit = UINT_MAX;
  // pointer to the best fitting node
  free_node* bestNode = NULL;

  // Iterates over the linked list to find the best fitting node
  while (node != NULL) {
    if (node->size >= minSize && node->size < bestSizeFit) {
      bestSizeFit = node->size;
      bestNode = node;
      // Perfect match: break loop
      if (node->size - minSize < MIN_BLOCK) {
        break;
      }
    }
    node = node->next;
  }

  return bestNode;
}

// deletes node from free list. returns 1 if node was found
int delete_node(free_node* node) {
  // free list is empty, exit function
  if (head == NULL) return 0;
  // node is the first element of the free list
  if (head == node) {
    head = head->next;
    node->next = NULL;
    return 1;
  }

  // iterate through free list
  free_node* itrNode = head;
  while (itrNode->next != NULL) {
    if (itrNode->next == node) {
      itrNode->next = node->next;
      node->next = NULL;
      return 1;
    }
    itrNode = itrNode->next;
  }
  node->next = NULL;
  return 0;
}

// doubles the length of chunk_pointers
void double_chunk_pointers() {
  chunk_pointers_max_len *= 2;
  uintptr_t* temp = malloc(chunk_pointers_max_len * sizeof(uintptr_t));
  for (int i = 0; i < chunk_pointers_max_len / 2; i++) {
    temp[i] = chunk_pointers[i];
  }
  free(chunk_pointers);
  chunk_pointers = temp;
}

// rounds up to the closest multiple of 16
uintptr_t round_16(uintptr_t size) {
  int modulus = size % 16;
  if (modulus > 0) return size + 16 - modulus;
  else return size;
}
