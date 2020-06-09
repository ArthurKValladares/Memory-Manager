bench: bench.c freemem.c getmem.c print_heap.c mem_utils.c get_mem_stats.c
	gcc -std=c11 -Wall -g -o bench  bench.c freemem.c getmem.c print_heap.c mem_utils.c get_mem_stats.c

debug_bench: bench.c freemem.c getmem.c print_heap.c mem_utils.c get_mem_stats.c
	gcc -DNDEBUG -std=c11 -Wall -g -o bench  bench.c freemem.c getmem.c print_heap.c mem_utils.c get_mem_stats.c

freemem.o: freemem.c
	gcc -std=c11 -Wall -g -c freemem.c

getmem.o: getmem.c
	gcc -std=c11 -Wall -g -c getmem.c

get_mem_stats.o: get_mem_stats.c
	gcc -std=c11 -Wall -g -c get_mem_stats.c

print_heap.o: print_heap.c
	gcc -std=c11 -Wall -g -c print_heap.c

test: bench
	./bench

large: bench
	./bench 10000000

get_large: bench
	./bench 10000 80 80

get_small: bench
	./bench 10000 80 5 

get_tons_small:
	./bench 1000000 80 5

get_tons_large:
	./bench 1000000 80 80

clean:
	rm -f *.o bench *~
