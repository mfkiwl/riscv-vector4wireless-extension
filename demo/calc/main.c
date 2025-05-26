/**
 * Copyright (c) 2022 - 2023, WingSemi Technology LTD.
 *
 * All rights reserved.
 */
#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"

extern int32_t volatile aAddr[32];
extern int32_t volatile bAddr[32];
extern int32_t volatile rZvmAddr[32];
extern int32_t volatile rRvvAddr[32];
extern uint16_t imageCcmMaskAddr[4];

extern int32_t volatile aCdsmAddr[64];
extern int32_t volatile bCdsmAddr[64];
extern uint16_t imageMaskAddr[8];

extern int32_t aWapAddr[2][32];
extern int32_t gainShiftAdrrZvw[32];
extern int32_t gainShiftAdrrRvv[64];

extern int32_t op_testzvwCcm();
extern int32_t op_testrvvCcm();
extern int32_t op_testzvwCdsm();
extern int32_t op_testrvvCdsm();
extern int32_t op_testzvwWap();
extern int32_t op_testrvvWap();
extern void model1_ut();
extern void vdscmul_ut();
extern void vdscmac_ut();
extern void vperm_ut();
extern void op_testMulj();
extern void vconj_ut();
extern void vdsredsum_ut();
extern void vfsl_ut();
extern void vfsr_ut();
extern void nolinear_recip8_ut();
extern void nolinear_sqrt8_ut();
extern void nolinear_recipSqrt8_ut();
extern void nolinear_arctan8_ut();
extern void nolinear_log2seg8_ut();
extern void nolinear_log10seg8_ut();
extern void vdsredsum_ut1();
extern int32_t op_intrinsicTest1();

void vdsp_test_cp(uint32_t dmemAddr,uint32_t vmemAddr,uint32_t len)
{
	uint32_t *pSrc = (uint32_t*)dmemAddr;
	uint32_t *pDsc = (uint32_t*)vmemAddr;
	uint32_t i;
	for(i = 0; i < len; i++)
	{
		*pDsc = *pSrc;
		pSrc++;
		pDsc++;
	}
	pDsc = (uint32_t *)vmemAddr;
}

int main()
{
    asm volatile("c.lui a0,0x10;"::);
    asm volatile("csrs mstatus, a0;"::);

    int i;
    // test case
    op_testMulj();
    vconj_ut();
    vdsredsum_ut();
    vfsl_ut();
    vfsr_ut();
#if 0    
    nolinear_recip8_ut();
    nolinear_sqrt8_ut();
    nolinear_recipSqrt8_ut();
    nolinear_arctan8_ut();
    nolinear_log2seg8_ut();
    nolinear_log10seg8_ut();
#endif    
    vdsredsum_ut1();
    model1_ut();
    vdscmul_ut();
    vdscmac_ut();
    vperm_ut();

    //ccm
    //op_goldenCcm();
	vdsp_test_cp((uint32_t)(&aAddr[0]),VM_SRC1_ADDR,32);
	vdsp_test_cp((uint32_t)(&bAddr[0]),VM_SRC2_ADDR,32);
	vdsp_test_cp((uint32_t)(&imageCcmMaskAddr[0]),VM_MASK_ADDR,4);
    op_testzvwCcm();
    op_testrvvCcm();
    uint32_t flag = 0;
    uint32_t *pz1 = (uint32_t *)VM_RST1_ZVW_ADDR;
    uint32_t *pz2 = (uint32_t *)VM_RST2_RVV_ADDR;
    for (i = 0; i < 32 ; i++)
    {
      if (*pz1 != *pz2)
      {
    	  flag = 1;
      }
    	pz1++;
    	pz2++;
    }
	if (flag == 0)
	{
		printf("ccm succ\n");
	}
    else{
        printf("ccm fail\n");
    }
	
	//cdsm
	vdsp_test_cp((uint32_t)(&aCdsmAddr[0]),VM_SRC1_ADDR,64);
	vdsp_test_cp((uint32_t)(&bCdsmAddr[0]),VM_SRC2_ADDR,64);
	vdsp_test_cp((uint32_t)(&imageMaskAddr[0]),VM_MASK_ADDR,8);
    op_testzvwCdsm();
    op_testrvvCdsm();
    pz1 = (uint32_t *)VM_RST1_ZVW_ADDR;
    pz2 = (uint32_t *)VM_RST2_RVV_ADDR;
    if (*pz1 != *pz2)
    {
        printf("cdsm fail %x,%x\n",*pz1,*pz2);
    }
    else
    {
    	printf("cdsm succ %x\n",*pz1);
    }	
	
	//wap
	vdsp_test_cp((uint32_t)(&aWapAddr[0][0]),VM_SRC1_ADDR,64);
	vdsp_test_cp((uint32_t)(&gainShiftAdrrZvw[0]),VM_GAIN_ZVW_ADDR,32);
	vdsp_test_cp((uint32_t)(&gainShiftAdrrRvv[0]),VM_GAIN_RVV_ADDR,64);
    op_testzvwWap();
    op_testrvvWap();
    pz1 = (uint32_t *)VM_RST1_ZVW_ADDR;
    pz2 = (uint32_t *)VM_RST2_RVV_ADDR;
    if (*pz1 != *pz2)
    {
        printf("wap fail resZvw %x ,resRvv %x\n", *pz1, *pz2);
    }
    else
    {
    	printf("wap succ %x\n",*pz1);
    }
    return 0;
}
