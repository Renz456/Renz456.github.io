#include <stdbool.h>
#include <stdlib.h>
#include "cachelab.h"

csim_stats_t* MultiCoreCacheSim_MSI(unsigned int p, unsigned long s,
                        unsigned long e, unsigned long b, int v, processor_t* processors);