/**
 * Conjugate Complex Multiply
 *
 * 
 */
 /*		 
SC16 VSA[32],VSB[32];
 
for (i = 0 ; i < 32; i ++)
{
	VSR[i].real  = VSA[i].real  * VSB[i].real  + VSA[i].image * VSB[i].image
	VSR[i].image = VSA[i].image * VSB[i].real - VSA[i].real  * VSB[i].image
}
 */
#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"
int32_t volatile aAddr[32] = {0x00030005,0x00090009,0x00070002,0x00020001,0x00040005,0x00030006,0x00050006,0x00030004,
			      0x00040005,0x00050002,0x00060007,0x00030001,0x00090004,0x00090007,0x00020006,0x00050007,
			      0x00090007,0x00060008,0x00020008,0x00020001,0x00070008,0x00090004,0x00010007,0x00050004,
			      0x00080004,0x00030007,0x00090008,0x00090006,0x00010004,0x00050002,0x00010009,0x00090001};
                                                                
int32_t volatile bAddr[32] = {0x00090002,0x00060006,0x00040006,0x00080004,0x00040003,0x00050007,0x00080003,0x00050007,
			      0x00050003,0x00040001,0x00050006,0x00020004,0x00020009,0x00040009,0x00060007,0x00090008,
			      0x00060009,0x00060002,0x00060008,0x00080003,0x00090005,0x00020008,0x00080005,0x00060006,
			      0x00050003,0x00020002,0x00050002,0x00040009,0x00070001,0x00020008,0x00070009,0x00060001}; 

int32_t volatile rZvmAddr[32] = {0};
int32_t volatile rRvvAddr[32] = {0};

uint16_t imageCcmMaskAddr[4] = {0xAAAA,0xAAAA,0xAAAA,0xAAAA};

int32_t op_testzvwCcm()
{
    size_t vl, avl;
    uint32_t vtypeE32;
    avl = VLEN / SEW32;
    vtypeE32 = TA | MA | M1 | E32;
    uint32_t gainShift = 0;       
    vint32m1_t volatile vA0;
    vint32m1_t volatile vA1;
    vint32m1_t volatile vR;

    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vle32.v %[vA0], (%[aAddr]);\
                  vle32.v %[vA1], (%[bAddr]);\
                  vdscmulj.vv v4, %[vA0], %[vA1];\
                  vse32.v  v4, (%[rZvmAddr]);" 
                  :[vl] "=r" (vl),[vA0]"+vr"(vA0),[vA1]"+vr"(vA1)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE32),[gainShift]"r"(gainShift),[aAddr]"r"(aAddr),[bAddr]"r"(bAddr),[rZvmAddr]"r"(rZvmAddr));    
    return 0;
}

int32_t op_testrvvCcm()
{
    size_t vl, avl;
    uint32_t vtypeE;
       
    avl = 64;

    
    vbool8_t   vMask;
    vint16m1_t vZero;
    vint16m1_t vA,vB,vR;
    vint16m1_t vAimage,vAreal,vBimage,vBreal,vReal1,vReal2,vImage1,vImage2,vReal,vImage,vImageUp,vResult;
    
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vlm.v %[vMask], (%[imageCcmMaskAddr]);"
                  :[vl] "=r" (vl),[vMask]"=vm"(vMask)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE),[imageCcmMaskAddr]"r"(imageCcmMaskAddr));
    
    vtypeE = TA | MA | M1 | E16;              
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vmv.v.i %[vZero], 0;\
                  vle16.v %[vA], (%[aAddr]);\
                  vle16.v %[vB], (%[bAddr]);"
                  :[vl] "=&r" (vl),[vA]"=vd"(vA),[vB]"=vd"(vB),[vZero]"=vd"(vZero)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE),[aAddr]"r"(aAddr),[bAddr]"r"(bAddr));               
                  
    
    vtypeE = TA | MA | M1 | E16;
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vmerge.vvm %[vAimage], %[vZero], %[vA], %[vMask];\
                  vmerge.vvm %[vAreal], %[vA], %[vZero], %[vMask];"
                  :[vl] "=&r" (vl),[vAimage]"=&vd"(vAimage),[vAreal]"=vd"(vAreal)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE),[vMask]"vm"(vMask),[vZero]"vr"(vZero),[vA]"vr"(vA));
    
    vtypeE = TA | MA | M1 | E16;              
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vslidedown.vi %[vAimage], %[vAimage], 1;\
                  vmerge.vvm %[vBimage], %[vZero], %[vB], %[vMask];\
                  vmerge.vvm %[vBreal], %[vB], %[vZero], %[vMask];\
                  vslidedown.vi %[vBimage], %[vBimage], 1;"              
                  :[vl] "=&r" (vl),[vAimage]"+&vd"(vAimage),[vBimage]"+&vd"(vBimage),[vBreal]"=vd"(vBreal)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE),[vMask]"vm"(vMask),[vZero]"vd"(vZero),[vB]"vd"(vB));
                                  
    asm volatile("vmul.vv %[vReal1], %[vAreal], %[vBreal];\
                  vmul.vv %[vReal2], %[vAimage], %[vBimage];\
                  vmul.vv %[vImage1], %[vAreal], %[vBimage];\
                  vmul.vv %[vImage2], %[vAimage], %[vBreal];"
                  :[vReal1]"=&vd"(vReal1),[vReal2]"=&vd"(vReal2),[vImage1]"=&vd"(vImage1),[vImage2]"=&vd"(vImage2)
                  :[vAimage]"vd"(vAimage),[vAreal]"vd"(vAreal),[vBimage]"vd"(vBimage),[vBreal]"vd"(vBreal));                     
                  
    asm volatile("vadd.vv %[vReal], %[vReal1], %[vReal2];\
                  vsub.vv %[vImage],%[vImage2], %[vImage1];\
                  vslideup.vi %[vImageUp], %[vImage], 1;"
                  :[vReal]"+&vd"(vReal),[vImage]"+&vd"(vImage),[vImageUp]"=&vd"(vImageUp)
                  :[vReal1]"vd"(vReal1),[vReal2]"vd"(vReal2),[vImage1]"vd"(vImage1),[vImage2]"vd"(vImage2));
                  
    
    vtypeE = TA | MA | M1 | E16;              
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vmerge.vvm %[vResult], %[vReal], %[vImageUp], %[vMask];\
                  vse16.v  %[vResult], (%[rRvvAddr]);"
                  :[vl] "=&r" (vl),[vResult]"=vr"(vResult)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE),[vReal]"vd"(vReal),[vImageUp]"vd"(vImageUp),[vMask]"vm"(vMask),[rRvvAddr]"r"(rRvvAddr));
 
                                                
    return 0;
}
