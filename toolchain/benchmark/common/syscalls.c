// See LICENSE for license details.

#include <stdint.h>
#include <stdio.h>
#include "util.h"

#define NUM_COUNTERS 2
static uintptr_t counters[NUM_COUNTERS];
static char* counter_names[NUM_COUNTERS];

void setStats(int enable)
{
  int i = 0;
#define READ_CTR(name) do { \
    while (i >= NUM_COUNTERS) ; \
    uintptr_t csr = read_csr(name); \
    if (!enable) { csr -= counters[i]; counter_names[i] = #name; } \
    counters[i++] = csr; \
  } while (0)

#if (MCYCLE_EXIST == ON)
  READ_CTR(mcycle);
  READ_CTR(minstret);
#endif

#undef READ_CTR
}



uintptr_t __attribute__((weak)) handle_trap(uintptr_t cause, uintptr_t epc, uintptr_t regs[32])
{
   printf("enter handle_trap, please check code... \r\n");
}

