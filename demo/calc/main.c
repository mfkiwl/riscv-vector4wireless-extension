/**
 * Copyright (c) 2022 - 2023, WingSemi Technology LTD.
 *
 * All rights reserved.
 */
#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"

extern int32_t volatile rZvmAddr[32];
extern int32_t volatile rRvvAddr[32];

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
int main()
{
    asm volatile("c.lui a0,0x10;"::);
    asm volatile("csrs mstatus, a0;"::);

    int i;
    op_intrinsicTest1();
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
    op_testzvwCcm();
    op_testrvvCcm();
    for (i = 0; i < 32 ; i++)
    {
      if (rZvmAddr[i] != rRvvAddr[i])
      {
        printf("ccm fail\n");
        break;
      }
    }
	if (i >= 32)
	{
        printf("ccm succ\n");
	}
	
	//cdsm
    int32_t resZvw = op_testzvwCdsm();
    int32_t resRvv = op_testrvvCdsm();
    if (resZvw == resRvv)
    {
      printf("cdsm succ\n");
    }
    else
    {
      printf("cdsm fail\n");
    }	
	
	//wap
   resZvw = op_testzvwWap();
   resRvv = op_testrvvWap();

    if (resZvw == resRvv)
    {
      printf("wap succ %x \n",resRvv);
    }
    else
    {
      printf("wap fail resZvw %x ,resRvv %x\n", resZvw, resRvv);
    }
    return 0;
}
