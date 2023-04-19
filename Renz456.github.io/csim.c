#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cachelab.h"

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


// From write up
// typedef struct {
//     unsigned long hits;            /* number of hits */
//     unsigned long cold_misses;          /* number of misses */
//     unsigned long conflict_misses;
//     unsigned long true_sharing_misses;
//     unsigned long false_sharing_misses;
//     unsigned long upgrades;

//     unsigned long evictions;       /* number of evictions */
//     unsigned long dirty_bytes;     /* number of dirty bytes in cache
//        at end of simulation */
//     unsigned long dirty_evictions; /* number of bytes evicted
//    from dirty lines */
// } csim_stats_t;

cacheline_t** init_cache(unsigned long s, unsigned long e){
    cacheline_t** cache = malloc(e * (0x1L << s) * sizeof(cacheline_t));
    if (!cache) {
        perror("cache array alloc failed!");
        return NULL;
    }

    for (unsigned long i = 0; i < e * (0x1L << s); i++) {
        cache[i]->isValid = false;
    }

    return cache; 

}

// TODO helpers
// void sendBusReq();

csim_stats_t* initStats(void) {
    csim_stats_t* final_stats = malloc(sizeof(csim_stats_t));
    final_stats->cold_misses=0;
    final_stats->upgrades=0;
    final_stats->conflict_misses=0;
    final_stats->dirty_bytes=0;
    final_stats->dirty_evictions=0;
    final_stats->evictions=0;
    final_stats->false_sharing_misses=0;
    final_stats->hits=0;
    final_stats->true_sharing_misses=0;
    return final_stats;
}

// function to check if all processors have finished their respective traces or not
bool check_processors(processor_t** processors, unsigned int p){
    for (unsigned int i = 0; i < p; i++){
        if (!processors[i]->done) return false;
    }

    return true;
}

csim_stats_t* MultiCoreCacheSim(unsigned int p, unsigned long s,
                        unsigned long e, unsigned long b, int v, processor_t** processors, char** traces){
    csim_stats_t* final_stats = initStats();
    
    // open and intitialise trace files
    for (unsigned int i = 0; i < p; i++){
        processors[i]->tfp = fopen(traces[i], "rt");
        processors[i]->done = false;
    }

    // int global_clock = 0;

    bus_interconnect* bus = malloc(sizeof(bus_interconnect));


    while(!check_processors(processors, p)){
        for (unsigned int j = 0; j < p; j++){
            char linebuff[100];
            bool read = true;
            if (!processors[j]->done && fgets(linebuff, 100, processors[j]->tfp)){
                if (linebuff[0] == 'w') read = false;
                char adress_string[64] = "";
                int check = 2;               // Starting index of address in linebuff

                while (linebuff[check] != ',') {

                    strncat(adress_string, &linebuff[check], 1);
                    check += 1;
                }
                const char *constant = adress_string;

                // printf("%s check this\n", adress_string);
                // checked to see if address was being parsed properly
                unsigned long address = (unsigned long)strtol(constant, NULL, 16);

                unsigned long mask1 = ~((~(unsigned long)0x0L) << s);

                unsigned long set = (address >> b) & mask1;
                bus->address = address;
                bus->processor_id=j;
                // if read bring other processors with same address back into shared state
                if (read){
                    bus->request=BUS_READ;
                }
                // if write invalidate other caches with same cacheline
                else{
                    bus->request=BUS_READ_X;
                }

                // update relvant cachelines in processors
                for (unsigned long i = 0; i < p; i++){
                    if (i == j) continue;
                    cacheline_t** other_cache = processors[i]->cache;
                    for (unsigned long k = 0; k < e; k++){
                        if (other_cache[set * e + i]->isValid &&
                            other_cache[set * e + i]->address >> b == address>>b){
                                if (read) {
                                    other_cache[set * e + i]->state = SHARED; // consider flush case
                                    other_cache[set * e + i]->dirty = false;
                                }
                                else other_cache[set * e + i]->state = INVALID; 
                            }
                    }
                }
                
                bool did_hit = false;

                cacheline_t** cache = processors[j]->cache;
                // rest of the cache can behave like csim, maybe no need for flush?
                for (unsigned long i = 0; i < e; i++) {
                    if (processors[i]->cache[set * e + i]->isValid &&
                        processors[i]->cache[set * e + i]->address >> b == address >> b) {
                        if (processors[i]->cache[set * e + i]->state == INVALID){
                            if (processors[i]->cache[set * e + i]->address==address) final_stats->true_sharing_misses += 1;
                            else final_stats->false_sharing_misses+=1;
                        }
                        // Store case if line has a 0 dirtybit
                        if (!read && !cache[set * e + i]->dirty) {
                            cache[set * e + i]->dirty = true;
                            final_stats->dirty_bytes += 1;
                            if (cache[set * e + i]->state == SHARED) final_stats->upgrades+=1;
                            cache[set * e + i]->state = MODIFY;
                        }
                        else if (cache[set * e + i]->state != MODIFY) cache[set * e + i]->state = SHARED;
                        final_stats->hits += 1;
                        did_hit = true;
                        cache[set * e + i]->LRU_count = processors[i]->count;
                        cache[set * e + i]->address = address;
                        
                        // cache[set*e + i][3] = 1;   removed this as it's redundant
                    }
                }


                if (!did_hit) {
                    for (unsigned long i = 0; i < e; i++) {
                        // Initialise line if unitialized so no eviction needs to occur
                        // yet
                        if (!cache[set * e + i]->isValid) {
                            cache[set * e + i]->address = address;
                            if (!read) {
                                final_stats->dirty_bytes += 1;
                                cache[set * e + i]->dirty = true;
                                cache[set * e + i]->state = MODIFY;
                            } else{
                                cache[set * e + i]->dirty = false;
                                cache[set * e + i]->state = SHARED;
                            }

                            cache[set * e + i]->LRU_count = processors[i]->count;
                            final_stats->cold_misses += 1;
                            cache[set * e + i]->isValid = true;
                            did_hit = true;
                            i += e;
                        }
                    }
                }

                // Eviction Case
                if (!did_hit) {
                    // To check LRU line, find line w the lowest count
                    // Line with the lowest count was used last
                    unsigned long lo = 0xFFFFFFFFFFL;
                    unsigned long lo_ind = 0;
                    for (unsigned long i = 0; i < e; i++) {
                        if (cache[set * e + i]->LRU_count < lo) {
                            lo = cache[set * e + i]->LRU_count;
                            lo_ind = set * e + i;
                        }
                    }
                    // If dirtybit is 1 for the line being evicted or dirtybit is 1
                    // for the new line being stored, update stats accordingly
                    cache[lo_ind]->address = address;
                    if (cache[lo_ind]->dirty) {
                        final_stats->dirty_bytes -= 1;
                        final_stats->dirty_evictions += 1;
                        cache[lo_ind]->dirty = false;
                    }
                    if (!read) {
                        cache[lo_ind]->dirty = true;
                        final_stats->dirty_bytes += 1;
                        cache[lo_ind]->state = MODIFY;
                    } else cache[lo_ind]->state= SHARED;
                    cache[lo_ind]->LRU_count = processors[j]->count;
                    final_stats->evictions += 1;
                    final_stats->conflict_misses += 1;
                }

                processors[j]->count += 1;



            }
            else processors[j]->done = true;

            
        }
    }
    return final_stats;
    
}

void printSummary(const csim_stats_t *stats);

// taken from write up
int process_trace_file(const char *trace) {
    return true;
    FILE *tfp = fopen(trace, "rt");
    if (!tfp) {
        printf("Error opening '%s'", trace);
        return 1;
    }
    unsigned long LINELEN = 38;
    // size of is 32hex long, +2 for opcode and space,
    // +4 for comma and number of bytes;
    char linebuf[LINELEN]; // How big should LINELEN be?
    int parse_error = 0;

    while (fgets(linebuf, 100, tfp)) {
        // Parse the line of text in linebuf
        // What do you do if the line is incorrect ?
        // What do you do if the line is longer than
        // LINELEN -1 chars?
        if (strlen(linebuf) > LINELEN)
            parse_error = 1; // instruction too big
        if (linebuf[0] != 'L' && linebuf[0] != 'S') {
            parse_error = 1; // invalid op
        }
        if (linebuf[1] != ' ') {
            parse_error = 1;
            // printf("error3\n");
        }
        unsigned int i = 2;
        // printf("%s", linebuf);
        while (i < strlen(linebuf)) {
            if (linebuf[i] != ',' && i + 3 == strlen(linebuf)) {
                // printf("error 1 %c\n", linebuf[i]);
                parse_error = 1;
            } else if (linebuf[i] == ',' && i + 3 != strlen(linebuf)) {
                parse_error = 1;
            }
            i += 1;
        }
    }
    fclose(tfp);
    // Why do I return parse_error here and not 0?
    return parse_error;
}

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

// char** create_ops(const char* trace , unsigned int count){
//     FILE *tfp = fopen(trace , "rt");
//     if (! tfp) {
//         printf("Error opening '%s'", trace);
//         return 1;
//     }
//     char** ops = alloc()
//     unsigned long LINELEN = 68;
//     char linebuf[100]; // How big should LINELEN be?
//     int count = 0;
//     while (fgets(linebuf , 100 , tfp )) {
//         count += 1;
//     }
//     fclose(tfp);
//     return count;
// }
// Tried making a function to return array of commands but turns out it's
// unecessary so Ignore^

void free_cache(unsigned long e, unsigned long s, unsigned long **cache) {
    for (unsigned long i = 0; i < (0x1L << s) * e; i++) {
        free(cache[i]);
    }
    free(cache);
}

// csim_stats_t *cache_sim(char *t, unsigned long **cache, unsigned long s,
//                         unsigned long e, unsigned long b, int v) {
//     // create sim iterating through lines in trace file
//     csim_stats_t *stats = malloc(sizeof(csim_stats_t));
//     if (!stats)
//         perror("alloc for struct failed :(\n");
//     stats->hits = 0;
//     stats->misses = 0;
//     stats->evictions = 0;
//     stats->dirty_bytes = 0;
//     stats->dirty_evictions = 0;
//     FILE *tfp = fopen(t, "rt");
//     char linebuff[38];
//     // same as linelen from earlier
//     int dirtycheck = 0;
//     unsigned long mask1 = ~((~(unsigned long)0x0L) << s);
//     unsigned long count = 0; // counter used for implimenting LRU
//     while (fgets(linebuff, 100, tfp)) {
//         // flags to check if new lines succeeded in a case and
//         // if it writes into memory or not
//         int did_hit = 0;
//         dirtycheck = 0;
//         if (linebuff[0] == 'S')
//             dirtycheck = 1;
//         if (v)
//             printf("%s", linebuff);
//         int check = 2;               // Starting index of address in linebuff
//         char adress_string[64] = ""; // I realise this spelling is wrong but did
//                                      // not feel like changing
//         while (linebuff[check] != ',') {
//             strncat(adress_string, &linebuff[check], 1);
//             check += 1;
//         }
//         const char *constant = adress_string;
//         // printf("%s check this\n", adress_string);
//         // checked to see if address was being parsed properly
//         unsigned long address = (unsigned long)strtol(constant, NULL, 16);
//         unsigned long set = (address >> b) & mask1;
//         // hit case
//         for (unsigned long i = 0; i < e; i++) {
//             if (cache[set * e + i]->isValid &&
//                 cache[set * e + i]->address >> b == address >> b) {
//                 // Store case if line has a 0 dirtybit
//                 if (dirtycheck == 1 && cache[set * e + i][1] != 1) {
//                     cache[set * e + i]->dirty = true;
//                     stats->dirty_bytes += 1;
//                 }
//                 stats->hits += 1;
//                 did_hit = 1;
//                 cache[set * e + i]->LRU_count = count;
//                 cache[set * e + i]->address = address;
//                 did_hit = 1;
//                 // cache[set*e + i][3] = 1;   removed this as it's redundant
//             }
//         }
//         // Initialize a new line case
//         if (did_hit == 0) {
//             for (unsigned long i = 0; i < e; i++) {
//                 // Initialise line if unitialized so no eviction needs to occur
//                 // yet
//                 if (!cache[set * e + i]->isValid) {
//                     cache[set * e + i]->address = address;
//                     if (dirtycheck == 1) {
//                         stats->dirty_bytes += 1;
//                         cache[set * e + i]->dirty = true;
//                     } else
//                         cache[set * e + i]->dirty = false;
//                     cache[set * e + i]->LRU_count = count;
//                     stats->misses += 1;
//                     cache[set * e + i]->isValid = 1;
//                     did_hit = 1;
//                     i += e;
//                 }
//             }
//         }
//         // Eviction Case
//         if (did_hit == 0) {
//             // To check LRU line, find line w the lowest count
//             // Line with the lowest count was used last
//             unsigned long lo = 0xFFFFFFFFFFL;
//             unsigned long lo_ind = 0;
//             for (unsigned long i = 0; i < e; i++) {
//                 if (cache[set * e + i]->LRU_count < lo) {
//                     lo = cache[set * e + i]->LRU_count;
//                     lo_ind = set * e + i;
//                 }
//             }
//             // If dirtybit is 1 for the line being evicted or dirtybit is 1
//             // for the new line being stored, update stats accordingly
//             cache[lo_ind]->address = address;
//             if (cache[lo_ind]->dirty) {
//                 stats->dirty_bytes -= 1;
//                 stats->dirty_evictions += 1;
//                 cache[lo_ind]->dirty = false;
//             }
//             if (dirtycheck == 1) {
//                 cache[lo_ind]->dirty = true;
//                 stats->dirty_bytes += 1;
//             }
//             cache[lo_ind]->LRU_count = count;
//             stats->evictions += 1;
//             stats->misses += 1;
//         }
//         count += 1;
//         if (v)
//             printSummary(stats);
//         // Printing summary after each line was more helpful to me than the
//         // version of -v used by csim-ref
//     }
//     return stats;
// }

processor_t ** initialise_cpu(unsigned int p, unsigned long e, unsigned long s){
    processor_t ** processors = malloc(sizeof(processor_t)*p);
    
    for (unsigned int i = 0; i < p; i++){
        processors[i]->cache = malloc(e * (0x1L << s) * sizeof(cacheline_t));
        
        processors[i]->myid=i;
        processors[i]->done = false;
        processors[i]->count = 0;
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
    char *t;
    int opt;

    // assign trace files here;
    while ((opt = getopt(argc, argv, "fvs:E:b:t:")) != -1) {
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
            t = optarg;
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
        perror("something's wrong with input\n");
        return -1;
    }

    if (process_trace_file(t))
        perror("invalid instruction\n");
    
    // cacheline_t*cache = malloc(e * (0x1L << s) * sizeof(cacheline_t));
    // if (!cache) {
    //     perror("cache array alloc failed!");
    //     return -1;
    // }

    // for (unsigned long i = 0; i < e * (0x1L << s); i++) {
    //     cache[i] = calloc(sizeof(unsigned long), 4);
    //     // each array holds adress, dirty bit, number of times called and valid
    //     // bit
    //     if (!cache[i]) {
    //         perror("set array alloc failed");
    //         return -1;
    //     }
    // }

    unsigned int p = 1;
    
    processor_t ** processors = initialise_cpu(p, e, s);
    printf("hi\n");
    char** traces = malloc(sizeof(char)*100*p);
    
    strcpy(traces[0], "traces/csim/yi.trace");

    
    csim_stats_t* final_stats = MultiCoreCacheSim(p, s, e, b, v, processors, traces);



    // csim_stats_t *states = cache_sim(t, cache, s, e, b, v);

    // dirty bytes = number of dirty bytes * size of b, same for dirty bytes
    // evicted
    final_stats->dirty_bytes = final_stats->dirty_bytes * (0x1L << b);
    final_stats->dirty_evictions = final_stats->dirty_evictions * (0x1L << b);

    if (!v)
        printSummary(final_stats);
    else {
        printf("\n");
        printSummary(final_stats);
    }

    // free_cache(s, e, cache);
    free(final_stats);

    return 0;
}
