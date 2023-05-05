#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cachelab.h"


/*
    File containing the implemetation of a directory based multicore 
    cache that follows the MESI protocol
*/




// Function to allocate memory for the cache of a processor
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

// intitialise directory, 
// conservatively assumes that there are number of cachelines x number of processors in memory
directory_t* init_directory(unsigned long s, unsigned long e,  unsigned long p){
    directory_t* directory = malloc(sizeof(directory));

    directory->cachelines = calloc((0x1L << s)*e*p, sizeof(directory_cacheline_t));
    for (unsigned long i = 0; i < (0x1L << s)*e*p; i++) directory->cachelines[i].processors = calloc(sizeof(unsigned int), p);
    return directory;
}

// updates directory to remove a line so it can be replaced with a new one
void update_memory(directory_t* directory, unsigned long address, 
                unsigned int processor, unsigned long b, unsigned long mem_size, unsigned int p){
    unsigned long ind = 0;
    for(unsigned long i = 0; i < mem_size; i++){
        if (directory->cachelines[i].address>>b == address>>b){
            ind = i;
            directory->cachelines[i].processors[processor] = 0;
            break;
        }
    }
    bool invalid = true;
    for (unsigned int i = 0; i < p; i++){
        if (directory->cachelines[ind].processors[i]==1) {
            invalid = false;
            break;
        }
    }
    if (invalid){
        directory->cachelines[ind].valid = false;
        directory->cachelines[ind].dirty = false;
    }
}

// updates processor's cacheline to a desired state
void update_processor(processor_t processor, unsigned long address, 
                        state_t state, unsigned long b, unsigned long s, unsigned long e){

    unsigned long mask1 = ~((~(unsigned long)0x0L) << s);
    unsigned long set = (address >> b) & mask1;

    for (unsigned long i = 0; i < e; i++){
        if (processor.cache[set * e + i].address>>b == address>>b){
            processor.cache[set * e + i].state = state;
            break;
        }
    }
    return;
}


// simulator function 
csim_stats_t* MultiCoreCacheSim_directory_MESI(unsigned int p, unsigned long s,
                        unsigned long e, unsigned long b, int v, processor_t* processors){
    
    csim_stats_t* final_stats = initStats();

    directory_t* directory = init_directory(s, e, p);
    
    unsigned int last_mem_pop = 0;
    unsigned long mem_size = (unsigned long)p*(0x1L << s)*e;

    queue_t* bus_queue = malloc(sizeof(queue_t));

    int global_clock = 0;
    
    while(!check_processors(processors, p)){

        // taking in each request from processors
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

                // checked to see if address was being parsed properly
                unsigned long address = (unsigned long)strtol(constant, NULL, 16);

                unsigned long mask1 = ~((~(unsigned long)0x0L) << s);
                
                unsigned long set = (address >> b) & mask1;
                
                bool did_hit = false;

                
                cacheline_t* cache = processors[j].cache;
                
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
                            // if (cache[set * e + i].address==address) final_stats->true_sharing_misses += 1;
                            // else final_stats->false_sharing_misses+=1;

                        }
                        else{
                            // silently upgrade state
                            if (!read && cache[set * e + i].state == EXCLUSIVE) cache[set * e + i].state = MODIFY;
                            final_stats->hits += 1;

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
                

            }
        else processors[j].done = true;
        }
        
        // cleairng bus queue
        while(bus_queue->head != NULL){
            // printf("check clock %d\n", global_clock);
            node_t* current_request = pop_bus_queue(bus_queue);

            unsigned int processor = current_request->curr_req.processor_id;
            unsigned long address = current_request->curr_req.address;
            bus_request_t req_type = current_request->curr_req.request;

            unsigned long mask1 = ~((~(unsigned long)0x0L) << s);    
            unsigned long set = (address >> b) & mask1;

            bool did_hit = false;
            bool read = req_type == BUS_READ;

            bool sharing = false;
            
            // check directory
            unsigned long insert_mem = mem_size;
            unsigned long found_mem = mem_size;
            
            for(unsigned int i = 0; i < mem_size; i++){
                if (directory->cachelines[i].valid && 
                    directory->cachelines[i].address>>b == address >> b){
                        found_mem = i;
                        break;
                    }
                if (!directory->cachelines[i].valid && insert_mem==mem_size) insert_mem = i;
                
            }
            
            
            // memory already exits, update any lines that are sharing 
            if (found_mem < mem_size){
                //
                if (read){
                    if(directory->cachelines[found_mem].dirty){
                        // unsigned int dirty_p = 0;
                        for (unsigned int i = 0; i < p; i++){
                            if (directory->cachelines[found_mem].processors[i] == 1){
                                update_processor(processors[i], address, SHARED, b, s, e);
                                sharing = true;
                                final_stats->communication_cost += 1;
                            }
                        }
                        
                    }
                    directory->cachelines[found_mem].dirty = false;

                }
                else{
                    
                    for (unsigned int i = 0; i < p; i++){
                        if (directory->cachelines[found_mem].processors[i] == 1){
                            update_processor(processors[i], address, INVALID, b, s, e);
                            directory->cachelines[found_mem].processors[i] = 0;
                            final_stats->communication_cost += 1;
                        }
                    }
                    directory->cachelines[found_mem].dirty = true;
                }
                directory->cachelines[found_mem].processors[processor] = 1;

            }
            else if (insert_mem < mem_size) {
                
                directory->cachelines[insert_mem].valid = true;
                
                if (!read) directory->cachelines[insert_mem].dirty = true;
                else directory->cachelines[insert_mem].dirty = false;
                
                directory->cachelines[insert_mem].address = address;
                // printf("seg boi %d %lu %lu %u\n", global_clock, insert_mem, found_mem, directory->cachelines[insert_mem].processors[processor]);
                directory->cachelines[insert_mem].processors[processor] = 1;
                // printf("post seg\n");
                
            
            }
            // need new memory case
            else{
                for (unsigned int i = 0; i < p; i++){
                    if (directory->cachelines[last_mem_pop].processors[i] == 1){
                        update_processor(processors[i], directory->cachelines[last_mem_pop].address, INVALID, b, s, e);
                        directory->cachelines[last_mem_pop].processors[i] = 0;
                        final_stats->communication_cost += 1;
                    }
                }
                directory->cachelines[last_mem_pop].address = address;
                if (!read) directory->cachelines[last_mem_pop].dirty = true;
                else directory->cachelines[last_mem_pop].dirty = false;
                directory->cachelines[last_mem_pop].processors[processor] = 1;

            }
            
            // printf("seg?\n");
            
            cacheline_t* cache = processors[processor].cache;
            
            // upgrade miss
            for (unsigned long i = 0; i < e; i++) {
                if (cache[set * e + i].isValid &&
                    cache[set * e + i].address >> b == address >> b) {
                    if (cache[set * e + i].state == INVALID ){

                        if (cache[set * e + i].address==address) final_stats->true_sharing_misses += 1;
                        else final_stats->false_sharing_misses+=1;
                        did_hit = true;
                        if (!sharing){
                            cache[set * e + i].state = EXCLUSIVE;
                        }
                        else if (read) {
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
                            // cache[set * e + i].state = SHARED;

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

                unsigned long old_address = cache[lo_ind].address;
                update_memory(directory, old_address, processor, b, mem_size, p); 
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
                    // cache[lo_ind].state= SHARED;
                }
                cache[lo_ind].LRU_count = processors[processor].count;
                final_stats->evictions += 1;
                final_stats->conflict_misses += 1;
            }

            processors[processor].count += 1;
            

            free(current_request);

            if(sharing) processors[processor].count += 0;
            
            
        }
        // printf("post req %d\n", bus_queue->size);
        global_clock += 1;   

    }
    printf("global clock %d\n", global_clock);
    free(directory->cachelines);
    free(directory);
    free(bus_queue);

    return final_stats;
}
