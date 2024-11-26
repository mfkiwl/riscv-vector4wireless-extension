/**
 * Wideband Average Power
 *
 * 
 */
 /*		 
SC16 A[2][32]

for (i = 0; i < 32; i++)
{
  R[i] = (pow2(A[0][i].r)  + pow2(A[0][i].i) + pow2(A[1][i].r) + pow2(A[1][i].i)) >> gainshift[i];
}

sum = 0;
for (i = 0; i < 32; i++)
{
   sum += R[i];
}
 */
#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"
int32_t aWapAddr[2][32] = {{0x00030005,0x00090009,0x00070002,0x00020001,0x00040005,0x00030006,0x00050006,0x00030004,
                                  0x00040005,0x00050002,0x00060007,0x00030001,0x00090004,0x00090007,0x00020006,0x00050007,
                                  0x00090007,0x00060008,0x00020008,0x00020001,0x00070008,0x00090004,0x00010007,0x00050004,
                                  0x00080004,0x00030007,0x00090008,0x00090006,0x00010004,0x00050002,0x00010009,0x00090001},
                                 {0x00090002,0x00060006,0x00040006,0x00080004,0x00040003,0x00050007,0x00080003,0x00050007,
                                  0x00050003,0x00040001,0x00050006,0x00020004,0x00020009,0x00040009,0x00060007,0x00090008,
                                  0x00060009,0x00060002,0x00060008,0x00080003,0x00090005,0x00020008,0x00080005,0x00060006,
                                  0x00050003,0x00020002,0x00050002,0x00040009,0x00070001,0x00020008,0x00070009,0x00060001}};


int32_t gainShiftAdrrZvw[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int32_t gainShiftAdrrRvv[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                                                         
int32_t op_testzvwWap()
{
    size_t vl, avl;
    uint32_t vtypeE;
    
    avl = 32;
    vtypeE = TA | MA | M1 | E32;

    vint32m1_t vZero,vGainShift;
    vint32m1_t vA0,vA1,vSum,vR;
    int32_t *a0Addr = aWapAddr[0];
    int32_t *a1Addr = aWapAddr[1];
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));
                             

    asm volatile("vle32.v %[vGainShift], (%[gainShiftAdrrZvw]);\
                  vdsmacini.v %[vGainShift];" 
                  :[vGainShift]"+&vr"(vGainShift)
                  :[gainShiftAdrrZvw]"r"(gainShiftAdrrZvw));                                 

                       
    asm ("vle32.v %[vA0], (%[a0Addr]);\
                  vdscmacj.vv %[vA0],%[vA0];\
                  vle32.v %[vA1], (%[a1Addr]);\
                  vdscmacjor.vv %[vSum], %[vA1], %[vA1];\
                  vdscredsum.v %[vR],%[vSum]" 
                  :[vA0]"+&vr"(vA0),[vA1]"+&vr"(vA1),[vSum]"+&vr"(vSum),[vR]"+&vr"(vR)
                  :[a0Addr]"r"(a0Addr),[a1Addr]"r"(a1Addr));  
                 
  int32_t  volatile result;
  asm volatile("vmv.x.s  %[result], %[vR];"
               :[result]"=r"(result)
               :[vR]"vr"(vR));               
    return result;
}

int32_t op_testrvvWap()
{
    size_t vl, avl;
    uint32_t vtypeE;

    int32_t *a0Addr = aWapAddr[0];
    int32_t *a1Addr = aWapAddr[1];    
    vint16m1_t vA,vB;
    vint32m1_t vTempShift;
    vint32m2_t vAcc,vAPow,vBPow;
    vint32m2_t vAPowShif,vBPowShift,vShift,vPowSum,vPowSumShift,vSum;
                    
    

    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));
    asm volatile("vmv.v.i %[vAcc], 0;"
                 :[vAcc]"=vr"(vAcc)
                 :);    

    asm volatile("vle32.v %[vShift], (%[gainShiftAddr]);\
                  vdsmacini.v %[vShift];" 
                  :[vShift]"+vr"(vShift)
                  :[gainShiftAddr]"r"(gainShiftAdrrRvv)); 
                     
    vtypeE = M1 | E16;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));              
    asm volatile("vle16.v %[vA], (%[a0Addr]);\
                  vwmul.vv %[vAPow],%[vA],%[vA];\
                  vle16.v %[vB], (%[a1Addr]);\
                  vwmul.vv %[vBPow],%[vB],%[vB]"
                 :[vA]"+vr"(vA),[vB]"+vr"(vB),[vAPow]"=&vr"(vAPow),[vBPow]"=&vr"(vBPow)
                 :[a0Addr]"r"(a0Addr),[a1Addr]"r"(a1Addr));

    vtypeE = M2 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));                                 
    asm volatile("vsra.vv %[vAPowShif], %[vAPow], %[vShift];\
                  vsra.vv %[vBPowShift], %[vBPow], %[vShift];\
                  vadd.vv %[vPowSum], %[vAPowShif], %[vBPowShift];\
                  vsra.vi %[vPowSumShift], %[vPowSum], 0;\
                  vredsum.vs %[vSum], %[vPowSumShift], %[vAcc];"
                 :[vSum]"=vr"(vSum),[vAPowShif]"+&vr"(vAPowShif),[vBPowShift]"+&vr"(vBPowShift),[vPowSum]"+&vr"(vPowSum),[vPowSumShift]"+&vr"(vPowSumShift)
                 :[vShift]"vr"(vShift), [vAcc]"vr"(vAcc),[vAPow]"vr"(vAPow),[vBPow]"vr"(vBPow));   
                 
  int32_t  volatile result;
  asm volatile("vmv.x.s  %[result], %[vSum];"
               :[result]"=r"(result)
               :[vSum]"vr"(vSum));                                 
    return result;
}