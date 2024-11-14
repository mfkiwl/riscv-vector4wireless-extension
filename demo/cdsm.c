/**
 * Complex Dynamic Scaling MAC
 *
 * 
 */
 /*		 
SC16 VSA[64],VSB[64];

////vdscmacjo 
for (i = 0 ; i < 32; i ++)
{
	Tmp[i].real  = VSA[i].real  * VSB[i].real  + VSA[i].image * VSB[i].image
	Tmp[i].image = VSA[i].image * VSB[i].real - VSA[i].real  * VSB[i].image
}

for (i = 32 ; i < 64; i ++)
{
	Tmp[i].real  = VSA[i].real  * VSB[i].real  + VSA[i].image * VSB[i].image
	Tmp[i].image = VSA[i].image * VSB[i].real - VSA[i].real  * VSB[i].image
}
for (i = 0 ; i < 32; i ++)
{
	VSR[i].real  = Tmp[i].real  + Tmp[i + 32].real
	VSR[i].image = Tmp[i].image + Tmp[i + 32].image
}

//vdscredsum
SUM[i].real  = 0;
SUM[i].image = 0;
for (i = 0 ; i < 32; i ++)
{
	SUM[i].real  += VSR[i].real
	SUM[i].image += VSR[i].image
}
 */
#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"
/*int32_t volatile aAddr[64] = {0x00030005,0x00090009,0x00070002,0x00020001,0x00040005,0x00030006,0x00050006,0x00030004,
                              0x00040005,0x00050002,0x00060007,0x00030001,0x00090004,0x00090007,0x00020006,0x00050007,
                              0x00090007,0x00060008,0x00020008,0x00020001,0x00070008,0x00090004,0x00010007,0x00050004,
                              0x00080004,0x00030007,0x00090008,0x00090006,0x00010004,0x00050002,0x00010009,0x00090001,
                              0x00030006,0x00090009,0x00070002,0x00020001,0x00040005,0x00030006,0x00050006,0x00030004,
                              0x00040005,0x00050002,0x00060007,0x00030001,0x00090004,0x00090007,0x00020006,0x00050007,
                              0x00090007,0x00060008,0x00020008,0x00020001,0x00070008,0x00090004,0x00010007,0x00050004,
                              0x00080004,0x00030007,0x00090008,0x00090006,0x00010004,0x00050002,0x00010009,0x00090001};
                                                                
int32_t volatile bAddr[64] = {0x00090002,0x00060006,0x00040006,0x00080004,0x00040003,0x00050007,0x00080003,0x00050007,
                              0x00050003,0x00040001,0x00050006,0x00020004,0x00020009,0x00040009,0x00060007,0x00090008,
                              0x00060009,0x00060002,0x00060008,0x00080003,0x00090005,0x00020008,0x00080005,0x00060006,
                              0x00050003,0x00020002,0x00050002,0x00040009,0x00070001,0x00020008,0x00070009,0x00060001,
                              0x00090003,0x00060006,0x00040006,0x00080004,0x00040003,0x00050007,0x00080003,0x00050007,
                              0x00050003,0x00040001,0x00050006,0x00020004,0x00020009,0x00040009,0x00060007,0x00090008,
                              0x00060009,0x00060002,0x00060008,0x00080003,0x00090005,0x00020008,0x00080005,0x00060006,
                              0x00050003,0x00020002,0x00050002,0x00040009,0x00070001,0x00020008,0x00070009,0x00060001}; */
int32_t volatile aCdsmAddr[64] = {0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001};
                                                                
int32_t volatile bCdsmAddr[64] = {0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
                              0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001};                              

uint16_t imageMaskAddr[8] = {0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA};

int32_t op_testzvwCdsm()
{
    size_t vl, avl;
    uint32_t vtypeE;
    
    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    
    uint32_t gainShift = 0;       
    vint32m2_t volatile vA0;
    vint32m2_t volatile vA1;
    vint32m2_t volatile vSum;
    vint32m2_t volatile vR;
    vint32m2_t vZero;
    int32_t  volatile result;
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vdsmacini.s %[gainShift];\
                  vle32.v %[vA0], (%[aCdsmAddr]);\
                  vle32.v %[vA1], (%[bCdsmAddr]);" 
                  :[vl] "=r" (vl),[vA0]"=vr"(vA0),[vA1]"=vr"(vA1)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE),[gainShift]"r"(gainShift),[aCdsmAddr]"r"(aCdsmAddr),[bCdsmAddr]"r"(bCdsmAddr));                   
#if 1
    //累加寄存器初始值不为0，此段为规避代码
    asm volatile("vmv.v.i %[vZero], 0;"
                 :[vZero]"=vr"(vZero)
                 :);
    asm volatile("vdscmacjo.vv v18, %[vZero], %[vZero];" 
                  :
                  :[vZero]"vr"(vZero));
#endif                  
    asm volatile("vdscmacjo.vv v10, %[vA0], %[vA1];\
                  vdscredsum.v v12,v10;\
                  vmv.x.s  %[result], v12;" 
                  :[result]"=r"(result)
                  :[vA0]"vr"(vA0),[vA1]"vr"(vA1));
                                                                        
  return result; 
}

int32_t op_testrvvCdsm()
{
    size_t vl, avl;
    uint32_t vtypeE;
       
    avl = 64 * 2;
    vtypeE = TA | MA | M2 | E16;

    uint32_t shift = 0;
    vint16m2_t vZero,vMinusOne;
    vint16m2_t vA,vB,vR;
    vint16m2_t vaccReal,vaccImage,vaccImageUp;
    vint16m2_t vAimage,vAreal,vBimage,vBreal,vResult;
#if 1    
    // 多个 asm没法合并一条 asm，合并后运行异常
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));
    
    asm volatile("vlm.v v0, (%[imageMaskAddr]);"
                 :
                 :[imageMaskAddr]"r"(imageMaskAddr));
                 
    asm volatile("vmv.v.i %[vZero], 0;\
                  vmv.v.v v16,%[vZero];\
                  vmv.v.v v20,%[vZero];"
                 :[vZero]"+vr"(vZero)
                 :);  
                 
    asm volatile("vle16.v %[vA], (%[aCdsmAddr]);\
                  vle16.v %[vB], (%[bCdsmAddr]);"
                 :[vA]"=vr"(vA),[vB]"=vr"(vB)
                 :[aCdsmAddr]"r"(aCdsmAddr),[bCdsmAddr]"r"(bCdsmAddr));
                                 
    asm volatile("vmerge.vvm %[vAimage], %[vZero], %[vA], v0;\
                  vmerge.vvm %[vAreal], %[vA], %[vZero], v0;\
                  vslidedown.vi %[vAimage], %[vAimage], 1;"
                 :[vAimage]"+vr"(vAimage),[vAreal]"+vr"(vAreal)
                 :[vZero]"vr"(vZero),[vA]"vr"(vA));
                 
    asm volatile("vmerge.vvm %[vBimage], %[vZero], %[vB], v0;\
                  vmerge.vvm %[vBreal], %[vB], %[vZero], v0;\
                  vslidedown.vi %[vBimage], %[vBimage], 1;"
                 :[vBimage]"+vr"(vBimage),[vBreal]"+vr"(vBreal)
                 :[vZero]"vr"(vZero),[vB]"vr"(vB));
 
     asm volatile("vmv.v.i %[vMinusOne], -1;"
                 :[vMinusOne]"=vr"(vMinusOne)
                 :);
#else
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vlm.v v0, (%[imageMaskAddr]);\
                  vmv.v.i %[vZero], 0;\
                  vmv.v.v v16,%[vZero];\
                  vmv.v.v v20,%[vZero];\
                  vle16.v %[vA], (%[aCdsmAddr]);\
                  vle16.v %[vB], (%[bCdsmAddr]);\
                  vmerge.vvm %[vAimage], %[vZero], %[vA], v0;\
                  vmerge.vvm %[vAreal], %[vA], %[vZero], v0;\
                  vslidedown.vi %[vAimage], %[vAimage], 1;\
                  vmerge.vvm %[vBimage], %[vZero], %[vB], v0;\
                  vmerge.vvm %[vBreal], %[vB], %[vZero], v0;\
                  vslidedown.vi %[vBimage], %[vBimage], 1;\
                  vmv.v.i %[vMinusOne], -1;"
                 :[vl] "=r" (vl),[vZero]"+vr"(vZero),[vA]"+vr"(vA),[vB]"+vr"(vB),[vAimage]"+vr"(vAimage),[vAreal]"+vr"(vAreal),[vBimage]"+vr"(vBimage),[vBreal]"+vr"(vBreal),[vMinusOne]"=vr"(vMinusOne)
                 :[imageMaskAddr]"r"(imageMaskAddr),[avl] "r" (avl), [vtype] "r" (vtypeE),[aCdsmAddr]"r"(aCdsmAddr),[bCdsmAddr]"r"(bCdsmAddr));
               
#endif                 
#if 0     
    //此段代码进入后会异常，怀疑寄存器分配不合理，按照else分支手动分配运行正常            
    asm ("vwmacc.vv %[vaccReal], %[vAreal], %[vBreal];\
                  vwmacc.vv %[vaccReal], %[vAimage], %[vBimage];\
                  vmul.vv %[vBimage], %[vBimage], %[vMinusOne];\
                  vwmacc.vv %[vaccImage], %[vAreal], %[vBimage];\
                  vwmacc.vv %[vaccImage], %[vAimage], %[vBreal];"
                 :[vaccReal]"=vr"(vaccReal),[vaccImage]"=vr"(vaccImage)
                 :[vAimage]"vr"(vAimage),[vAreal]"vr"(vAreal),[vBimage]"vr"(vBimage),[vBreal]"vr"(vBreal),[vMinusOne]"vr"(vMinusOne));
#else
   asm volatile("vwmacc.vv v16, %[vAreal], %[vBreal];\
                  vwmacc.vv v16, %[vAimage], %[vBimage];\
                  vmul.vv %[vBimage], %[vBimage], %[vMinusOne];\
                  vwmacc.vv v20, %[vAreal], %[vBimage];\
                  vwmacc.vv v20, %[vAimage], %[vBreal];"
                 :
                 :[vAimage]"vr"(vAimage),[vAreal]"vr"(vAreal),[vBimage]"vr"(vBimage),[vBreal]"vr"(vBreal),[vMinusOne]"vr"(vMinusOne));                 
#endif

    vtypeE = TA | MA | M4 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));
    
    asm volatile("vsra.vx v24, v16, %[shift];\
                  vsra.vx v28, v20, %[shift];"
                 :
                 :[shift]"r"(shift));

    vtypeE = TA | MA | M4 | E16;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));                 
    asm volatile("vslideup.vi v12, v28, 1;\
                  vmerge.vvm v4,v24, v12, v0;"
                 :
                 :);
                 
    vtypeE = TA | MA | M4 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));
    asm volatile("vredsum.vs v2,v4, %[vZero];"
                 :
                 :[vZero]"vr"(vZero));
              
    int32_t  volatile result;
    asm volatile("vmv.x.s  %[result], v2;"
                 :[result]"=r"(result)
                 :);    
    return result;
}

