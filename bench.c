// Code for Benchmark program.
// When run, executes a large number of calls to getmem and freemem to allocate
//  and free blocks of random sizes and in random order.
// Allows the user to specify parameters that control the test.
// Parameters should come in order:
// [ntrials] [pctget] [pctlarge] [small_limit] [large_limit] [random_seed]
// Where:
// ntrials: total number of getmem plus freemem calls to randomly
//           perform during this test.Default 10000.
// pctget: percent of the total getmem/freemem calls that should be getmem.
//         Default 50.
// pctlarge: percent of the getmem calls that should request "large" blocks
//           with a size greater than small_limit. Default 10.
// small_limit: largest size in bytes of a "small" block. Default 200.
// large_limit: largest size in bytes of a "large" block. Default 20000.
// random_seed: initial seed value for the random number generator.
//              Default: System time-of-day clock
// All parameters should be integers >= 0.
// percentages larger than 100 work as 100%.

#include <time.h>
#include <stdlib.h>

#include "mem.h"

#define N_TRIALS 0
#define PCTGET 1
#define PCTLARGE 2
#define SMALL_LIMIT 3
#define LARGE_LIMIT 4
#define RANDOM_SEED 5

int main(int argc, char **argv) {
  // Array holding command line parameters
  int params[6] = {10000, 50, 10, 200, 20000, (int) time(NULL)};
  for (int i = 1; i < argc; i++) {
    params[i - 1] = atoi(argv[i]);
  }
  srand(params[RANDOM_SEED]);
  printf("Output Format:\n");
  printf("T: Time elapsed S: Total storage acquired B: # of blocks on free list"
         " A: Average # of bytes in free blocks\n");
  // Array holding memory adresses returned by getmem
  void **list = (void *)malloc(sizeof(void *) * params[N_TRIALS]);
  // Count of how many memory adresses are currently in list
  int count = 0;
  // Number of iterations that correnponds to 10% of trials executes
  int pct_count = params[N_TRIALS]/10;
  // Percentage of trials currently executed
  int percentage = 0;
  // Start time of benchmark trials
  clock_t start = clock();
  for (int i = 0; i < params[N_TRIALS]; i++) {
    int rand_choice = (rand() % 100) + 1;
    if (rand_choice <= params[PCTGET]) {
      int rand_choice_2 = rand() % 100 + 1;
      int size;
      if (rand_choice_2 <= params[PCTLARGE]) {
        // GET LARGE
        size = (rand() % (params[LARGE_LIMIT]
 -             params[LARGE_LIMIT] + 1)) + params[SMALL_LIMIT];
      } else {
        // GET SMALL
        size = (rand() % params[SMALL_LIMIT]) + 1;
      }
      list[count] = getmem(size);
      count++;
    } else {
      if (count > 0) {
        int index = rand() % count;
        freemem(list[index]);
        // Fixing gap in array, adjusting count
        list[index] = list[count - 1];
        list[count - 1] = NULL;
        count--;
      }
    }

    if ((i+1)%pct_count == 0 && i != 0) {
      clock_t end = clock();
      double elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
      uintptr_t total_size = 0;
      uintptr_t  total_free = 0;
      uintptr_t n_free_blocks = 0;
      get_mem_stats(&total_size, &total_free, &n_free_blocks);
      double avg = 0;
      if (n_free_blocks != 0) {
        avg = ((double) total_free)/n_free_blocks;
      }
      percentage += 10;
      printf("At %d%%: T: %0.10f S: %lu B: %lu A: %f\n",
              percentage, elapsed, total_size, n_free_blocks, avg);
    }
  }

  free_all_mem();
  free(list);
  return 0;
}
