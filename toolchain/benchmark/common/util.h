// See LICENSE for license details.

#ifndef __UTIL_H
#define __UTIL_H

#ifdef __riscv
#include "encoding.h"   
#endif

/*操作memmap 寄存器 */
#define REG32(addr)  (*((volatile unsigned int*) (addr))) 

#define OFF (0)
#define ON  (1)

#if defined(CORE_M130) || defined(CORE_M132) || defined(CORE_M134) || defined(CORE_M136) || defined(CORE_HM134S)
    #define X4_EXIST        ON
    #define X6_EXIST        ON
    #define MTVEC_EXIST     ON
    #define MCYCLE_EXIST    ON
#elif defined(CORE_M052) 
    #define X4_EXIST        OFF
    #define X6_EXIST        OFF
    #define MTVEC_EXIST     OFF    
    #define MCYCLE_EXIST    OFF
#elif defined(CORE_M056)
    #define X4_EXIST        OFF
    #define X6_EXIST        ON
    #define MTVEC_EXIST     ON    
    #define MCYCLE_EXIST    OFF
#elif defined(CORE_HM112S)
    #define X4_EXIST        OFF
    #define X6_EXIST        ON
    #define MTVEC_EXIST     ON
    #define MCYCLE_EXIST    OFF
#else
    #define X4_EXIST        ON
    #define X6_EXIST        ON
    #define MTVEC_EXIST     ON
    #define MCYCLE_EXIST    ON
#endif



/*Time相关寄存器*/
#define MTIME_BASE_ADDR      (0xE0000000)
#define MTIME_LO_REG_ADDR    (MTIME_BASE_ADDR + 0x0)
#define MTIMECMP_LO_REG_ADDR (MTIME_BASE_ADDR + 0x8)
#define MTIMECTL_REG_ADDR    (MTIME_BASE_ADDR + 0x10)
#define MSIP_REG_ADDR        (MTIME_BASE_ADDR + 0x20)


#if (MCYCLE_EXIST == ON)
    #define getCounter() read_csr(mcycle)
#else
    #define getCounter() REG32(MTIME_LO_REG_ADDR)
#endif
#endif //__UTIL_H