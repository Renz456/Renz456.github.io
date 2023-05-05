#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cachelab.h"


/* File that simulates a snooping based multicore cache with a MESI protocol */


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





csim_stats_t* MultiCoreCacheSim_MESI(unsigned int p, unsigned long s,
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
