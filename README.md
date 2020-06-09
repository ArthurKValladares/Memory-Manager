Memory management program replicating the functionality of malloc and free.

Available functions:

void* getmem(uintptr_t size):

    Returns a pointer to a new block of storage with at least size bytes of memory. The pointer to the returned block is aligned on an 16-byte boundary.

void freemem(void* p):

    Returns the block of storage at location p to the pool of available free storage. The pointer value p must be one that was obtained as the result of a call to getmem. If p is NULL, then the call to freemem has no effect and returns immediately. If p has some value other than one returned by getmem, or if the block it points to has previously been released by another call to freemem, then the operation of freemem is undefined. If the block is physically located in memory adjacent to one or more other free blocks, then the free blocks involved are combined into a single larger block.

void get_mem_stats(
  uintptr_t* total_size,
  uintptr_t* total_free,
  uintptr_t* n_free_blocks):

    Store statistics about the current state of the memory manager in the three integer variables whose addresses are given as arguments. The information stored is as follows:

    total_size: total amount of storage in bytes acquired by the memory manager so far to use in satisfying allocation requests.

    total_free: the total amount of storage in bytes that is currently stored on the free list, including any space occupied by header information or links in the free blocks.

    n_free_blocks: the total number of individual blocks currently stored on the free list.

void print_heap(FILE * f):

    Print a formatted listing on file f showing the blocks on the free list. Each line of output describes one free block and begins with two hexadecimal numbers giving the address and length of that block. 
    

void check_heap():	

    Checks for possible problems with the free list data structure.
    Verifies the following properties:

    -Blocks are ordered with strictly increasing memory addresses.
    -Block sizes are positive numbers and no smaller than whatever minimum size you are using in your implementation
    -Blocks do not overlap (the start + length of a block is not an address in the middle of a later block on the list)
    -Blocks are not touching (the start + length of a block should not be the same address as the next block on the list since in that case the two blocks should have been combined into a single, larger block.)