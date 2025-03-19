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
    uint32_t vcsrA0M0R0Sa = ACCSFT0 | MULSFT0 | VXRM_RNU | VXSAT1;    
    asm volatile("csrw vcsr,%[vcsrA0M0R0Sa];"
    :
    : [vcsrA0M0R0Sa] "r" (vcsrA0M0R0Sa)
    );   
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vdsmacini.s %[gainShift];\
                  vle32.v %[vA0], (%[aCdsmAddr]);\
                  vle32.v %[vA1], (%[bCdsmAddr]);" 
                  :[vl] "=r" (vl),[vA0]"=vr"(vA0),[vA1]"=vr"(vA1)
                  :[avl] "r" (avl), [vtype] "r" (vtypeE),[gainShift]"r"(gainShift),[aCdsmAddr]"r"(aCdsmAddr),[bCdsmAddr]"r"(bCdsmAddr));                   
                 
    asm volatile("vdscmacjo.vv v10, %[vA0], %[vA1];\
                  vdscredsum.v v12,v10;\
                  vmv.x.s  %[result], v12;" 
                  :[result]"=r"(result)
                  :[vA0]"vr"(vA0),[vA1]"vr"(vA1));
                                                                        
  return result; 
}

int32_t op_testrvvCdsm()
{
    uint32_t accShfit = 0, mulShfit = 0;     
    uint32_t num16 = 16;
    
    size_t vl, avl;
    uint32_t vtypeE;
       
    avl = 64;
    
    vint32m2_t vA,vB;
    vint32m2_t vZero,vZ;
    vint32m2_t vResult,vResultReal,vResultImag;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vsub.vv %[vZero], %[vZero],%[vZero];\
                  vsub.vv %[vZ], %[vZ],%[vZ];\
                  vsub.vv %[vResult], %[vResult],%[vResult];\
                  vsub.vv %[vResultReal], %[vResultReal],%[vResultReal];\
                  vsub.vv %[vResultImag], %[vResultImag],%[vResultImag];\
                  vle32.v %[vA], (%[aCdsmAddr]);\
                  vle32.v %[vB], (%[bCdsmAddr]);"
                  :[vl]"=&r"(vl),[vZero]"+vd"(vZero),[vZ]"+vd"(vZ),[vResult]"+vd"(vResult),[vResultReal]"+vd"(vResultReal),[vResultImag]"+vd"(vResultImag),[vA]"=vd"(vA),[vB]"=vd"(vB)
                  :[avl]"r"(avl),[vtype]"r"(vtypeE),[aCdsmAddr]"r"(aCdsmAddr),[bCdsmAddr]"r"(bCdsmAddr));    
                  
    vint32m2_t vAimage,vAreal,vBimage,vBreal,vTempA,vTempB,vRealShfit;
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vsrl.vx %[vAimage], %[vA], %[num16];\
                  vsll.vx %[vRealShfit], %[vA], %[num16];\
                  vsrl.vx %[vAreal], %[vRealShfit], %[num16];\
                  vsrl.vx %[vBimage], %[vB], %[num16];\
                  vsll.vx %[vRealShfit], %[vB], %[num16];\
                  vsrl.vx %[vBreal], %[vRealShfit], %[num16];"
                 :[vl]"=&r"(vl),[vAimage]"=vd"(vAimage),[vAreal]"=vd"(vAreal),[vRealShfit]"+vd"(vRealShfit),[vBimage]"=vd"(vBimage),[vBreal]"=vd"(vBreal)
                 :[avl]"r"(avl),[vtype]"r"(vtypeE),[num16]"r"(num16),[vA]"vd"(vA),[vB]"vd"(vB));  

    int32_t One = 1;
    vint32m2_t vMinusOne;
    vint32m2_t vaccReal,vaccImage;
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vsub.vx %[vMinusOne], %[vZero],%[One];"
                  :[vl]"=&r"(vl),[vMinusOne]"=vd"(vMinusOne)
                  :[avl]"r"(avl),[vtype]"r"(vtypeE),[vZero]"vd"(vZero),[One] "r" (One)); 
                  
    asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                  vmacc.vv %[vaccReal], %[vAreal], %[vBreal];\
                  vmacc.vv %[vaccReal], %[vAimage], %[vBimage];\
                  vmul.vv %[vBimage], %[vBimage], %[vMinusOne];\
                  vmacc.vv %[vaccImage], %[vAreal], %[vBimage];\
                  vmacc.vv %[vaccImage], %[vAimage], %[vBreal];"
                 :[vl]"=&r"(vl),[vaccReal]"+vd"(vaccReal),[vaccImage]"+vd"(vaccImage)
                 :[avl]"r"(avl),[vtype]"r"(vtypeE),[vAimage]"vd"(vAimage),[vAreal]"vd"(vAreal),[vBimage]"vd"(vBimage),[vBreal]"vd"(vBreal),[vMinusOne]"vd"(vMinusOne)); 

    asm volatile("vredsum.vs %[vResultImag],%[vaccImage], %[vZ];"
                 :[vResultImag]"=vd"(vResultImag)
                 :[vZ]"vd"(vZ),[vaccImage]"vd"(vaccImage));
    asm volatile("vredsum.vs %[vResultReal],%[vaccReal], %[vZ];"
                 :[vResultReal]"=vd"(vResultReal)
                 :[vZ]"vd"(vZ),[vaccReal]"vd"(vaccReal));
                 
     asm volatile("vsetvl %[vl], %[avl], %[vtype];\
                   vsll.vx %[vTempB], %[vResultImag], %[num16];\
                   vxor.vv %[vResult], %[vTempB], %[vResultReal]"
                 :[vl]"=&r"(vl),[vTempB]"+vd"(vTempB),[vResult]"+vd"(vResult)
                 :[avl]"r"(avl),[vtype]"r"(vtypeE),[num16]"r"(num16),[vResultImag]"vd"(vResultImag),[vResultReal]"vd"(vResultReal));  

    int32_t  volatile result;
    asm volatile("vmv.x.s  %[result], %[vResult];"
                 :[result]"=r"(result)
                 :[vResult]"vd"(vResult));                 
              
    return result;
}

