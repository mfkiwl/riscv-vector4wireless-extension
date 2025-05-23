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
    size_t vl;
    uint32_t vtype1 = TA | MA | M1 | E32;
    uint32_t *pz = (uint32_t *)VM_RST2_RVV_ADDR;
    uint32_t syncRd;

    asm volatile("vsetvl %[vl], %[avl], %[vtype]" : [vl] "=r" (vl) : [avl] "r" (1), [vtype] "r" (vtype1));
    asm volatile("vmv.v.i v26, 0;");//VALU2
    asm volatile("vsetvli %[vl], %[avl],209;":[vl] "=&r" (vl):[avl] "r" (64));
    asm volatile("vle32.v v2, (%[aCdsmAddr]);"::[aCdsmAddr]"r"(VM_SRC1_ADDR));//LSD
    asm volatile("vsrl.vx v6, v2, %[num16];"::[num16]"r"(16));//VALU2 IM0(L)
    asm volatile("vle32.v v4, (%[bCdsmAddr]);"::[bCdsmAddr]"r"(VM_SRC2_ADDR));//LSD
    asm volatile("vsrl.vx v12, v4, %[num16];"::[num16]"r"(16));//VALU2 IM1(L)
    asm volatile("vsll.vx v8, v2, %[num16];"::[num16]"r"(16));//VALU2 RE0(H)
    asm volatile("vsll.vx v30, v4, %[num16];"::[num16]"r"(16));//VALU2 RE1(H)
    asm volatile("vmul.vv v26, v6, v12;"::);//VALU1 IM0(L)*IM1(L)
    asm volatile("vsrl.vx v10, v8, %[num16];"::[num16]"r"(16));//VALU2 RE0(L)
    asm volatile("vsrl.vx v14, v30, %[num16];"::[num16]"r"(16));//VALU2 RE1(L)
    asm volatile("vmul.vv v16, v10, v14;"::);//VALU1  RE0(L)*RE1(L)
    asm volatile("vadd.vv v16, v16, v26;"::);//VALU1   RE0(L)*RE1(L)+IM0(L)*IM1(L)
    asm volatile("vmul.vv v28, v6, v14;"::);//VALU1  IM0(L)*RE1(L)
    asm volatile("vsetvli %[vl], %[avl],209;":[vl] "=&r" (vl):[avl] "r" (32));
    asm volatile("vredsum.vs v22, v16, v26;"::);//VALU2
    asm volatile("vsetvli %[vl], %[avl],209;":[vl] "=&r" (vl):[avl] "r" (64));
    asm volatile("vmul.vv v18, v10, v12;"::);//VALU1  IM1(L)*RE0(L)
    asm volatile("vsub.vv v18, v28, v18;"::);//VALU1  IM0(L)*RE1(L)-IM1(L)*RE0(L)
    asm volatile("vsetvli %[vl], %[avl],209;":[vl] "=&r" (vl):[avl] "r" (32));
    asm volatile("vredsum.vs v20, v18, v26;"::);//VALU2
    asm volatile("vsetvl %[vl], %[avl], %[vtype]" : [vl] "=r" (vl) : [avl] "r" (1), [vtype] "r" (vtype1));
    asm volatile("vsll.vx v0, v20, %[num16];"::[num16]"r"(16));//VALU2
    asm volatile("vxor.vv v2, v0, v22;"::);//VALU1
    asm volatile("vmv.x.s %[result], v2;"::[result]"r"(*pz));//VALU2
    asm volatile("vsync %[syncRd],%[rs2];":[syncRd]"=&r"(syncRd):[rs2]"r"(0xFF));
              
    return *pz;
}

