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




cacheline_t* init_cache(unsigned long s, unsigned long e){
    cacheline_t* cache = malloc(e * (0x1L << s) * sizeof(cacheline_t));
    if (!cache) {
        perror("cache array alloc failed!");
        return NULL;
    }

    for (unsigned long i = 0; i < e * (0x1L << s); i++) {
        cache[i].isValid = false;
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
bool check_processors(processor_t* processors, unsigned int p){
    for (unsigned int i = 0; i < p; i++){
        if (!processors[i].done) return false;
    }

    return true;
}

csim_stats_t* MultiCoreCacheSim(unsigned int p, unsigned long s,
                        unsigned long e, unsigned long b, int v, processor_t* processors){
    csim_stats_t* final_stats = initStats();
    
    queue_t* bus_queue = malloc(sizeof(queue_t));
    
    int global_clock = 0;
    
    while(!check_processors(processors, p)){

        for (unsigned int j = 0; j < p; j++){
            
            char linebuff[100];
            bool read = true;
            
            if (!processors[j].done && fgets(linebuff, 38, processors[j].tfp)){
                
                if (linebuff[0] == 'w') read = false;
                
                char adress_string[64] = "";
                int check = 5;               // Starting index of address in linebuff
                
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
                
                
                bool did_hit = false;

                cacheline_t* cache = processors[j].cache;
                // rest of the cache can behave like csim, maybe no need for flush?

                // bus upgrade bus req
                for (unsigned long i = 0; i < e; i++) {
                    if (cache[set * e + i].isValid &&
                        cache[set * e + i].address >> b == address >> b) {
                        if (cache[set * e + i].state == INVALID ||
                            (cache[set * e + i].state == SHARED && !read)){

                            node_t* request = malloc(sizeof(node_t));
                            request->next = NULL;
                            request->curr_req.address = address;
                            request->curr_req.processor_id = j;
                            if (read) request->curr_req.request = BUS_READ;
                            else request->curr_req.request = BUS_READ_X;  
                            push_bus_queue(bus_queue, request);
                            // if (cache[set * e + i].address == address) final_stats->true_sharing_misses += 1;
                            // else final_stats->false_sharing_misses+=1;

                        }
                        else{
                            final_stats->hits += 1;
                            // silently upgrade to modify state if line is exclusive
                            if (!read && cache[set * e + i].state == EXCLUSIVE) cache[set * e + i].state = MODIFY;
                        }
                        did_hit = true;
                        break;
                    }
                }

                // miss case bus req
                if (!did_hit){  
                    node_t* request = malloc(sizeof(node_t));
                    request->next = NULL;
                    request->curr_req.address = address;
                    request->curr_req.processor_id = j;
                    if (read) request->curr_req.request = BUS_READ;
                    else request->curr_req.request = BUS_READ_X;  
                    push_bus_queue(bus_queue, request);
                    
                            
                }

                
                


            
            } else processors[j].done = true;
        }
        
        while(bus_queue->head != NULL){
            node_t* current_request = pop_bus_queue(bus_queue);

            unsigned int processor = current_request->curr_req.processor_id;
            unsigned long address = current_request->curr_req.address;
            bus_request_t req_type = current_request->curr_req.request;

            unsigned long mask1 = ~((~(unsigned long)0x0L) << s);    
            unsigned long set = (address >> b) & mask1;

            bool did_hit = false;
            bool read = req_type == BUS_READ;

            cacheline_t* cache = processors[processor].cache;


            // update relvant cachelines in processors
            bool sharing = false;
            
            final_stats->communication_cost += p;

            for (unsigned long i = 0; i < p; i++){
                if (i == processor) continue;
                cacheline_t* other_cache = processors[i].cache;
                for (unsigned long k = 0; k < e; k++){
                    if (other_cache[set * e + k].isValid &&
                        other_cache[set * e + k].address >> b == address>>b){
                            if (read) {
                                other_cache[set * e + k].state = SHARED; // consider flush case
                            }
                            else {
                                other_cache[set * e + k].state = INVALID; 
                            }
                            other_cache[set * e + k].dirty = false;
                            sharing = true; // check if this needed?
                        
                        }
                }
            }

            // in cache but upgrade required case
            for (unsigned long i = 0; i < e; i++) {
                if (cache[set * e + i].isValid &&
                    cache[set * e + i].address >> b == address >> b) {
                    if (cache[set * e + i].state == INVALID ){

                        if (cache[set * e + i].address==address) final_stats->true_sharing_misses += 1;
                        else final_stats->false_sharing_misses+=1;
                        did_hit = true;
                        if (read) {
                            cache[set * e + i].state = SHARED;
                        }
                        else {
                            cache[set * e + i].state = MODIFY;
                            final_stats->dirty_bytes += 1;

                        }
                    }
                    // Store case if line has a 0 dirtybit
                    if (!read && cache[set * e + i].state == SHARED) {

                        cache[set * e + i].dirty = true;
                        final_stats->dirty_bytes += 1;
                        final_stats->upgrades+=1;
                        cache[set * e + i].state = MODIFY;
                
                    
                    }
                    

                    did_hit = true;
                    cache[set * e + i].LRU_count = processors[processor].count;
                    cache[set * e + i].address = address;
                    break;
                    // cache[set*e + i][3] = 1;   removed this as it's redundant
                }
            }

            //cold miss
            if (!did_hit) {
                for (unsigned long i = 0; i < e; i++) {
                    // Initialise line if unitialized so no eviction needs to occur
                    // yet
                    if (!cache[set * e + i].isValid) {
                        cache[set * e + i].address = address;
                        if (!read) {
                            final_stats->dirty_bytes += 1;
                            cache[set * e + i].dirty = true;
                            cache[set * e + i].state = MODIFY;
                        } else{
                            if (sharing) cache[set * e + i].state = SHARED;
                            else cache[set * e + i].state = EXCLUSIVE;

                            cache[set * e + i].dirty = false;
                        }

                        cache[set * e + i].LRU_count = processors[processor].count;
                        final_stats->cold_misses += 1;
                        cache[set * e + i].isValid = true;
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
                    if (cache[set * e + i].LRU_count < lo) {
                        lo = cache[set * e + i].LRU_count;
                        lo_ind = set * e + i;
                    }
                }
                // If dirtybit is 1 for the line being evicted or dirtybit is 1
                // for the new line being stored, update stats accordingly
                cache[lo_ind].address = address;
                if (cache[lo_ind].dirty) {
                    final_stats->dirty_bytes -= 1;
                    final_stats->dirty_evictions += 1;
                    cache[lo_ind].dirty = false;
                }
                if (!read) {
                    cache[lo_ind].dirty = true;
                    final_stats->dirty_bytes += 1;
                    cache[lo_ind].state = MODIFY;
                } else {
                    if (sharing) cache[lo_ind].state= SHARED;
                    else cache[lo_ind].state= EXCLUSIVE;
                }
                cache[lo_ind].LRU_count = processors[processor].count;
                final_stats->evictions += 1;
                final_stats->conflict_misses += 1;
            }

            processors[processor].count += 1;

            free(current_request);

        }
        
        global_clock += 1;
    }
        
    
    
    printf("global_clock %d\n", global_clock);
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

    // if (s < 0 || s > 64) {
    //     perror("invalid set size\n");
    //     return -1;
    // }

    if (e < 0) {
        perror("invalid byte offset\n");
        return -1;
    }

    // if (b < 0 || b > 64) {
    //     perror("invalid e size\n");
    //     return -1;
    // }

    // if (b + s > 64) {
    //     perror("something's wrong with input\n");
    //     return -1;
    // }

    if (process_trace_file(t))
        perror("invalid instruction\n");
    
    

    unsigned int p = 4;

    // char traces[4][100] = {"traces/csim/ocean1.trace", "traces/csim/ocean2.trace", "traces/csim/ocean3.trace", "traces/csim/ocean4.trace"};
    // char traces[4][100] = {"traces/csim/yi3.trace", "traces/csim/yi2.trace", "traces/csim/yi.trace", "traces/csim/yi4.trace"};
    // char traces[4][100] = {"traces/traces/true_share1.trace", "traces/traces/true_share2.trace", "traces/traces/true_share3.trace", "traces/traces/true_share4.trace"};
    char traces[4][100] = {"traces/traces/false_sharing1.trace", "traces/traces/false_sharing2.trace", "traces/traces/false_sharing3.trace", "traces/traces/false_sharing4.trace"};
    // char traces[4][100] = {"traces/traces/test_check1.trace", "traces/traces/test_check2.trace", "traces/traces/test_check3.trace", "traces/traces/test_check4.trace"};
    
    processor_t * processors = initialise_cpu(p, e, s, traces);
    
    
    csim_stats_t* final_stats = MultiCoreCacheSim(p, s, e, b, v, processors);

    
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
