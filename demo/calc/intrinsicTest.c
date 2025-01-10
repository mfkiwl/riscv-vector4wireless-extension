#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"
#define ZVW_INTRINSIC


int32_t op_intrinsicTest1()
{
    size_t vl;
    int32_t avl;
    avl = 128;

    uint32_t i,j;
    vint32m4_t vZero,vSum;    
    vint32m4_t volatile vA0;
    vint32m4_t volatile vA1;
    vint32m4_t volatile vR;
    int32_t aAddr[128], bAddr[128], rZvmAddr[128];
    vZero = __riscv_vmv_v_x_i32m4(0,avl);
    

    for (i = 0; i < 16; i++)
    {
    	for (j = 0; j < 2; j++)
    	{
    	    
	    vA0 = __riscv_vle32_v_i32m4((int32_t*)aAddr, avl);
	    vA1 = __riscv_vle32_v_i32m4((int32_t*)bAddr, avl);
	    __riscv_vdscmacj_vv_i32m4(vA0,vA1,0, avl);
	    // asm volatile("vdscmacj.vv %[vA0], %[vA1];" 
        //          :
        //          :[vA0]"vr"(vA0),[vA1]"vr"(vA1));
           vR = __riscv_vdscmacjor_vv_i32m4(vZero,vZero,0,avl);
           __riscv_vse32_v_i32m4((int32_t*)rZvmAddr,vR,avl);    	    
    	}
    }
    return 0;
}

