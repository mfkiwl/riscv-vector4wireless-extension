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
    uint32_t rs2 = 0xFF,syncRd;
    uint32_t pa = (uint32_t)VM_SRC1_ADDR;
    uint32_t pb = (uint32_t)VM_SRC2_ADDR;
    uint32_t *pz = (uint32_t*)VM_RST1_ZVW_ADDR;
    uint32_t gainShift = 0;       
    uint32_t vcsrA0M0R0Sa = ACCSFT0 | MULSFT0 | VXRM_RNU | VXSAT1;
    asm volatile("csrw vcsr,%[vcsrA0M0R0Sa];":: [vcsrA0M0R0Sa] "r" (vcsrA0M0R0Sa));

    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvli %[vl], %[avl],209;":[vl] "=&r" (vl):[avl] "r" (avl));
    asm volatile("vdsmacini.s %[gainShift];" ::[gainShift]"r"(gainShift));
    asm volatile("vle32.v v0, (%[aCdsmAddr]);" ::[aCdsmAddr]"r"(pa));
    asm volatile("vle32.v v2, (%[bCdsmAddr]);" ::[bCdsmAddr]"r"(pb));
    asm volatile("vdscmacjo.vv v10,v0, v2;"::);
    avl = 32;
    vtypeE = TA | MA | M1 | E32;
    asm volatile("vsetvli %[vl], %[avl],208;":[vl] "=&r" (vl):[avl] "r" (avl));
    asm volatile("vdscredsum.v v12,v10;"::);
    asm volatile("vmv.x.s  %[result], v12;" :[result]"=&r"(*pz):);
    asm volatile("vsync %[syncRd],%[rs2];":[syncRd]"=&r"(syncRd):[rs2]"r"(rs2));
    syncRd = syncRd * 2;
  return syncRd;
}

uint32_t op_testrvvCdsm()
{
    uint32_t accShfit = 0, mulShfit = 0;     
    uint32_t num16 = 16;
    uint32_t pa = (uint32_t)VM_SRC1_ADDR;
    uint32_t pb = (uint32_t)VM_SRC2_ADDR;
    uint32_t *pz = (uint32_t*)VM_RST2_RVV_ADDR;
    uint32_t pm = (uint32_t)VM_MASK_ADDR;
    size_t vl, avl;
    uint32_t vtypeE;
    int32_t One = 1;
    uint32_t rs2 = 0xFF,syncRd;
    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvli %[vl], %[avl],209;":[vl] "=&r" (vl):[avl] "r" (avl));
    asm volatile("vle32.v v2, (%[aCdsmAddr]);"::[aCdsmAddr]"r"(pa));
    asm volatile("vle32.v v4, (%[bCdsmAddr]);"::[bCdsmAddr]"r"(pb));
    asm volatile("vsrl.vx v6, v2, %[num16];"::[num16]"r"(num16));
    asm volatile("vsll.vx v8, v2, %[num16];"::[num16]"r"(num16));
    asm volatile("vsrl.vx v10, v8, %[num16];"::[num16]"r"(num16));
    asm volatile("vsrl.vx v12, v4, %[num16];"::[num16]"r"(num16));
    asm volatile("vsll.vx v8, v4, %[num16];"::[num16]"r"(num16));
    asm volatile("vsrl.vx v14, v8, %[num16];"::[num16]"r"(num16));
    asm volatile("vsub.vv v16, v16,v16;"::);
    asm volatile("vsub.vv v18, v18,v18;"::);
    asm volatile("vsub.vx v24, v18,%[One];"::[One] "r" (One));
    asm volatile("vmul.vv v0, v10, v14;"::);
    asm volatile("vadd.vv v16, v16, v0;"::);
    asm volatile("vmul.vv v0, v6, v12;"::);
    asm volatile("vadd.vv v16, v16, v0;"::);
    asm volatile("vmul.vv v12, v12, v24;"::);
    asm volatile("vmul.vv v0, v10, v12;"::);
    asm volatile("vadd.vv v18, v18, v0;"::);
    asm volatile("vmul.vv v0, v6, v14;"::);
    asm volatile("vadd.vv v18, v18, v0;"::);
    asm volatile("vsub.vv v0, v0,v0;"::);
    asm volatile("vredsum.vs v20,v18, v0;"::);
    asm volatile("vredsum.vs v22,v16, v0;"::);
    asm volatile("vsll.vx v0, v20, %[num16];"::[num16]"r"(num16));
    asm volatile("vxor.vv v2, v0, v22;"::);
    asm volatile("vmv.x.s  %[result], v2;":[result]"=&r"(*pz):);
    asm volatile("vsync %[syncRd],%[rs2];":[syncRd]"=&r"(syncRd):[rs2]"r"(rs2));
    syncRd = syncRd * 2;
    return syncRd;
}

