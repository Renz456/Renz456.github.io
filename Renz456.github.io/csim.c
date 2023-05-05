#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cachelab.h"
#include "MSI.h"
#include "MESI.h"


// What does csim.c do?
// csim.c simulates a cache by taking in commmands and outputs the
// the expected behavior of a cache. It takes in -s -E -b and a
// traace file as arguments. -s is the arugment for number of sets,
// -E is the size of each set and -b is the bit offset.
// My implemenation of this uses a two dimensional array
// The outer array is used to represent each line in the cache.
// The number of lines can be caculated from the input as 2^s * E
// To access line j of set i in the cache, one would use cache[i*e + j]
// I used arrays for this as compared to datastructures like linked lists,
// I am unlikely to access null pointers and they have a O(1) access time.
// To modify the way this program simulates a cache, you must modify the
// function cache_sim which takes the s, E,b and the trace file name as
// arguments.
// The inner array is used to represent each line in the cache and consists
// of 4 elements: The address stored in the cache, the dirtybit, the cycle
// during which it was last called (used for LRU eviction), and the valid
// bit for intializing a line.
// cache_sim() takes a set of instructions and checks 3 cases:
// hit, intialize and evict to simulate a cache in memory










unsigned int find_arg_len(const char *trace) {
    FILE *tfp = fopen(trace, "rt");
    if (!tfp) {
        printf("Error opening '%s'", trace);
        return 1;
    }
    // unsigned long LINELEN = 68;
    char linebuf[100]; // How big should LINELEN be?
    unsigned int count = 0;

    while (fgets(linebuf, 100, tfp)) {
        count += 1;
    }
    fclose(tfp);
    return count;
}


void free_cache(unsigned int p, processor_t* processors) {
    for (unsigned long i = 0; i < p; i++) {
        free(processors[i].cache);
        // free(processors[i].tfp);
    }
    free(processors);
}



processor_t * initialise_cpu(unsigned int p, unsigned long e, unsigned long s, char traces[1][100]){
    processor_t * processors = malloc(sizeof(processor_t)*p);
    
    for (unsigned int i = 0; i < p; i++){

        processors[i].cache = init_cache(s, e);
        
        processors[i].myid=i;
        processors[i].tfp = fopen(traces[i], "rt");
        processors[i].done = false;
        processors[i].count = 0;
    }
    
    return processors;
}

int main(int argc, char **argv) {

    if (argc < 9) {
        perror("not enough arugments bro\n");
        return -1;
    }
    
    // printf("%d\n", argc);
    // for (int i = 0; i < argc; i++){
    //     printf("%s\n",argv[i]);
    // }
    // This is what the first part of the write-up asked us to try ^

    int v = 0;
    unsigned long s;
    unsigned long e;
    unsigned long b;
    char *m;
    int opt;

    // assign trace files here;
    while ((opt = getopt(argc, argv, "fvs:E:b:m:")) != -1) {
        switch (opt) {
        case 'f':
            return 0;
        case 'v':
            v = 1;
            break;
        case 's':
            s = (unsigned)atol(optarg);
            break;
        case 'E':
            e = (unsigned)atol(optarg);
            break;
        case 'b':
            b = (unsigned)atol(optarg);
            break;
        case 't':
            m = optarg;
            break;
        default:
            printf("wrong argument\n");
            return -1;
        }
    }

    if (s < 0 || s > 64) {
        perror("invalid set size\n");
        return -1;
    }

    if (e < 0) {
        perror("invalid byte offset\n");
        return -1;
    }

    if (b < 0 || b > 64) {
        perror("invalid e size\n");
        return -1;
    }

    if (b + s > 64) {
        perror("address must be 64 bits long\n");
        return -1;
    }

    
    

    unsigned int p = 4;

    // char traces[4][100] = {"traces/csim/ocean1.trace", "traces/csim/ocean2.trace", "traces/csim/ocean3.trace", "traces/csim/ocean4.trace"};
    // char traces[4][100] = {"traces/csim/yi3.trace", "traces/csim/yi2.trace", "traces/csim/yi.trace", "traces/csim/yi4.trace"};
    // char traces[4][100] = {"traces/traces/true_share1.trace", "traces/traces/true_share2.trace", "traces/traces/true_share3.trace", "traces/traces/true_share4.trace"};
    char traces[4][100] = {"traces/traces/false_sharing1.trace", "traces/traces/false_sharing2.trace", "traces/traces/false_sharing3.trace", "traces/traces/false_sharing4.trace"};
    // char traces[4][100] = {"traces/traces/test_check1.trace", "traces/traces/test_check2.trace", "traces/traces/test_check3.trace", "traces/traces/test_check4.trace"};
    
    processor_t * processors = initialise_cpu(p, e, s, traces);
    
    csim_stats_t* final_stats;

    switch (m)
    {
        case "MSI":
            final_stats = MultiCoreCacheSim_MSI(p, s, e, b, v, processors);
            break;
        
        case "MESI":
            final_stats = MultiCoreCacheSim_MESI(p, s, e, b, v, processors);
            break;
        case "directory_MSI":
            final_stats = MultiCoreCacheSim_directory_MSI(p, s, e, b, v, processors);
            break;
        case "directory_MESI":
            final_stats = MultiCoreCacheSim_directory_MESI(p, s, e, b, v, processors);
            break;
        default:
            final_stats = MultiCoreCacheSim_MSI(p, s, e, b, v, processors);
            break;    
    }
    
    
    final_stats->dirty_bytes = final_stats->dirty_bytes * (0x1L << b);
    final_stats->dirty_evictions = final_stats->dirty_evictions * (0x1L << b);

    if (!v)
        printSummary(final_stats);
    else {
        printf("\n");
        printSummary(final_stats);
    }

    free_cache(p, processors);
    free(final_stats);

    return 0;
}
