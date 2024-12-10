#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"

typedef struct {
    uint8_t m0;
    uint8_t m1;
    uint8_t m2;
    uint8_t m3;
    uint8_t m4;
    uint8_t m5;
    uint8_t m6;
    uint8_t pad0;
    int16_t m7[MAX_RX_NUM];
    int16_t m8[MAX_RX_NUM];
#if 0
    int16_t m9[MAX_RX_NUM];
#else
    int16_t m10;
    int16_t m11;
#endif
    uint32_t m12;
    uint32_t m13;
    int32_t m14[2];
} op_model1_param_t;


void op_model1_calc(op_model1_param_t *pParam)
{
    size_t vl1;
    uint32_t m12 = pParam->m12;
    uint32_t m4 = pParam->m4;
    uint32_t m6 = pParam->m6;
    volatile uint32_t addr0, addr1;
    uint32_t vcsrA0M0R0Sa = ACCSFT0 | MULSFT0 | VXRM_RNU | VXSAT1;
    uint32_t vtypeL1E32 = MA | TA | E32 | M1;
    uint32_t loop;

    asm volatile ("csrw vcsr, %[vcsrA0M0R0Sa]"::[vcsrA0M0R0Sa] "r" (vcsrA0M0R0Sa));

    for (loop = 0; loop < m6; loop++)
    {
        asm volatile ("vsetvl %v1, %[av1], %[vtype]":[v1] "=r" (vl1): [av1] "r" (m4), [vtype] "r" (vtypeL1E32));
        addr0 = pParam->m12 + loop * 2 * m4 * 4;
        addr1 = pParam->m12 + (loop * 2 + 1) * m4 * 4;
        asm volatile (
            "vle32.v v1, (%[addr0]);\
            vle32.v v2, (%[addr1]);"
            :
            : [addr0] "r" (addr0), [addr1] "r" (addr1)
            : "memory"
        );
    }
}


void model1_ut()
{
    int32_t result = 1;
    size_t count0 = 0;
    size_t count1 = 0;
    int32_t result0, result1;
    op_model1_param_t para;
    int32_t cirSel[64] =
    {
        0xffdb19c5, 0x001b02d8, 0xfff602fd, 0xffd8fd57, 0x001cfd50, 0x001f0267, 0xffee0265, 0xffe4fdea,
        0x003ffdf5, 0x004e01c1, 0xffcb01aa, 0xffddfec7, 0x003cfed1, 0x002c00ce, 0xffae00d1, 0x002dff9c,
        0xfffa19b5, 0x001b02d1, 0xfff802df, 0xffc7fd50, 0x0024fd50, 0x001e026d, 0xffd9025e, 0xffa7fe17,
        0x004bfef0, 0x004901a4, 0xffb0019c, 0xffdcfe9f, 0x004dfecb, 0x002f00e1, 0xffd700e9, 0x004bff77,
        0xffec19b6, 0x001402dc, 0xffdf02db, 0xffcafd59, 0x0026fd46, 0x00290263, 0xffcd0259, 0xffd3fdf0,
        0x0028fde9, 0x002b019e, 0xffeb01a2, 0xfff4feba, 0x0033fec9, 0x002100da, 0xffbd00e5, 0x0016ff8f,
        0xfffa19c9, 0x000702e6, 0xffdd02f7, 0xffe7fd64, 0x0013fd5c, 0x00210246, 0xffd60265, 0xffcefdfb,
        0x002cfde4, 0x004501b4, 0xffd201c1, 0xffbefec8, 0x002dfec2, 0x001600cc, 0xffc800d9, 0x0016ff9b
    };

    para.m10 = 0x3ffd;
    para.m11 = 0;
    para.m0 = 0;
    para.m1 = 0;
    para.m3 = 2;
    para.m4 = 0x0010;
    para.m5 = 0;
    para.m6 = 2;
    para.m7[0] = 2;
    para.m7[1] = 2;
    para.m8[0] = 0x001E;
    para.m8[1] = 0x001E;
    para.m10 = 0;
    para.m11 = 0x3ffd;
    para.m12 = (uint32_t)&cirSel[0];

    op_model1_calc(&para);
}

void vdscmul_ut()
{
    uint32_t vs1 = 0x1C1C;
    uint32_t vs2 = 0x7FFE;
    uint32_t vd;
    uint32_t vdTv = 0x1C1C;
    uint32_t vsrc1[32] =
    {
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C
    };
    uint32_t vsrc2[32] =
    {
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE,
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE,
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE,
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE
    };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C
    };
    uint32_t i;
    size_t vl;
    uint32_t len = 1;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;
    uint32_t vcsrA0M15R0Sa = ACCSFT0 | MULSFT15 | VXRM_RNU | VXSAT1;

    asm volatile("vsetvl %[vl], %[avl], %[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len), [vtype] "r" (vtypeL1E32));

    asm volatile(
        "vmv.s.x v0, %[vs1];\
        vmv.s.x v6, %[vs2];\
        csrw vcsr, %[vcsrA0M15R0Sa];\
        vdscmul.vs v12, v6, v0;\
        vmv.x.s %[vd], v12;"
        : [vd] "=r" (vd)
        : [vcsrA0M15R0Sa] "r" (vcsrA0M15R0Sa), [vs1] "r" (vs1), [vs2] "r" (vs2)
    );

    if (vd == vdTv)
    {
        printf("vdsmul.vs_ut passed!\n");
    }
    else
    {
        printf("dsmul.vs_ut data comparison failed vd[%d] vdTv[%d]!\n", vd, vdTv);
    }

    len = 32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len), [vtype] "r" (vtypeL1E32));

    asm volatile(
    "vle32.v v0, (%[vsrc1]);\
    vle32.v v6, (%[vsrc2]);\
    csrw vcsr, %[vcsrA0M15R0Sa];\
    vdscmul.vv v12, v6, v0;\
    vse32.v v12, (%[vdst]);"
    :
    : [vcsrA0M15R0Sa] "r" (vcsrA0M15R0Sa), [vsrc1] "r" (vsrc1), [vsrc2] "r" (vsrc2), [vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("dsmul.vv_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n", i, vdst[i], vdstTv[i]);
            return;
        }
    }
    printf("vdsmul.vv_ut passed!\n");
}

void vdscmac_ut()
{
    uint32_t vs1 = 0x1C1C;
    uint32_t vs2 = 0x7FFE;
    uint32_t vd;
    uint32_t vdTv = 0x384;
    uint32_t vsrc1[32] =
    {
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C,
        0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C, 0x00001C1C
    };
    uint32_t vsrc2[32] =
    {
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE,
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE,
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE,
        0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE, 0x00007FFE
    };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384,
        0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384,
        0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384,
        0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384, 0x00000384
    };
   uint32_t i;

    size_t vl;
    uint32_t len = 1;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;
    uint32_t vcsrA4M15R0Sa = ACCSFT4 | MULSFT15 | VXRM_RNU | VXSAT1;

    asm volatile("vsetvl %[vl], %[avl], %[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len), [vtype] "r" (vtypeL1E32));

    asm volatile(
        "vmv.s.x v0, %[vs1];\
        vmv.s.x v6, %[vs2];\
        csrw vcsr, %[vcsrA4M15R0Sa];\
        vdsmacini.i 15;\
        vdscmac.vs v6, v0;\
        vdscmaco.vs v12, v6, v0;\
        vmv.x.s %[vd], v12;"
        : [vd] "=r" (vd)
        : [vcsrA4M15R0Sa] "r" (vcsrA4M15R0Sa), [vs1] "r" (vs1), [vs2] "r" (vs2)
        );

    if (vd == vdTv)
    {
        printf("vdscmac.vs_ut passed!\n");
    }
    else
    {
        printf("dscmac.vs_ut data comparison failed vd[%d] vdTv[%d]!\n", vd, vdTv);
    }

   len = 32;
   asm volatile("vsetvl %[vl], %[avl], %[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len), [vtype] "r" (vtypeL1E32));

    asm volatile(
    "vle32.v v0, (%[vsrc1]);\
    vle32.v v6, (%[vsrc2]);\
    csrw vcsr, %[vcsrA4M15R0Sa];\
    vdsmacini.i 15;\
    vdscmac.vv v6, v0;\
    vdscmaco.vv v12, v6, v0;\
    vse32.v v12, (%[vdst]);"
    :
    : [vcsrA4M15R0Sa] "r" (vcsrA4M15R0Sa), [vsrc1] "r" (vsrc1), [vsrc2] "r" (vsrc2), [vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("dscmac.vv_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n", i, vdst[i], vdstTv[i]);
            return;
        }
    }
    printf("vdscmac.vv_ut passed!\n");
}





void vperm_ut()
{
    uint32_t vsrc1[32] =
    {
        0x00001C1C, 0x00001C1D, 0x00001C1E, 0x00001C1F, 0x00001C20, 0x00001C21, 0x00001C22, 0x00001C23,
        0x00001C24, 0x00001C25, 0x00001C26, 0x00001C27, 0x00001C28, 0x00001C29, 0x00001C2A, 0x00001C2B,
        0x00001C2C, 0x00001C2D, 0x00001C2E, 0x00001C2F, 0x00001C30, 0x00001C31, 0x00001C32, 0x00001C33,
        0x00001C34, 0x00001C35, 0x00001C36, 0x00001C37, 0x00001C38, 0x00001C39, 0x00001C3A, 0x00001C3B
    };
    uint32_t vdst[32];
    uint32_t vdst8Tv[32] =
    {
        0x00001C3B, 0x00001C3A, 0x00001C39, 0x00001C38, 0x00001C37, 0x00001C36, 0x00001C35, 0x00001C34,
        0x00001C33, 0x00001C32, 0x00001C31, 0x00001C30, 0x00001C2F, 0x00001C2E, 0x00001C2D, 0x00001C2C,
        0x00001C2B, 0x00001C2A, 0x00001C29, 0x00001C28, 0x00001C27, 0x00001C26, 0x00001C25, 0x00001C24,
        0x00001C23, 0x00001C22, 0x00001C21, 0x00001C20, 0x00001C1F, 0x00001C1E, 0x00001C1D, 0x00001C1C
    };
    uint32_t i;

    size_t vl;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

    asm volatile("vsetvl %[vl], %[avl], %[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len), [vtype] "r" (vtypeL1E32));

    asm volatile(
    "vle32.v v0, (%[vsrc1]);\
    vperm.vi v1, v0, 8;\
    vse32.v v1, (%[vdst]);"
    :
    : [vsrc1] "r" (vsrc1), [vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdst8Tv[i])
        {
            printf("vperm0_ut data comparison failed at index %d vdst[%d] vdst8Tv[%d]!\n", i, vdst, vdst8Tv);
            return; 
            }
    }
    printf("vperm0_ut passed!\n");
}

int32_t gAddr[64] = {1};
int32_t gRes[64] = {0};
int32_t op_testMvZero()
{
    size_t vl, avl;
    uint32_t vtypeE;

    vint32m2_t vA;
                   
    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE));

/*

 1000546:	5e003157          	vmv.v.i	v2,0
 100054a:	02076107          	vle32.v	v2,(a4)
 100054e:	56200057          	vdsmacini.v	v2
 1000552:	bbc18793          	addi	a5,gp,-1092 # 2000143c <gRes>
 1000556:	0207e127          	vse32.v	v2,(a5)
*/
    asm volatile("vle32.v %[vA], (%[gAddr]);\
                  vdsmacini.v %[vA];" 
                  :[vA]"+vr"(vA)
                  :[gAddr]"r"(gAddr)); 
                                      
    asm volatile("vse32.v %[vA], (%[gRes]);" 
                  :[vA]"=vr"(vA)
                  :[gRes]"r"(gRes));                              
    return 0;
}

int32_t gMacj[64] ={
0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,
0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001,0x00010001
};

void vvdscmacjor_utm1()
{
    size_t vl, avl;
    uint32_t vtypeE;
    int32_t shiftBit = 0;
    
    vint32m1_t vAcc,vA;
                   
    avl = 32;
    vtypeE = TA | MA | M1 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE)); 
    
    asm volatile("vle32.v %[vA], (%[gMacj]);" 
                  :[vA]"+vr"(vA)
                  :[gMacj]"r"(gMacj)); 
                  
    asm volatile("vdsmacini.s %[shiftBit];" : :[shiftBit]"r"(shiftBit)); 
                                   
    asm volatile("vdscmacj.vv %[vA], %[vA];" 
                  :
                  :[vA]"vr"(vA));
                  
    asm volatile("vdscmacjor.vv %[vAcc],%[vA], %[vA];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vA]"vr"(vA));
                  

    asm volatile("vse32.v %[vAcc], (%[gRes]);" 
                  :[vAcc]"=vr"(vAcc)
                  :[gRes]"r"(gRes));   
    int32_t i;
    for (i = 0; i < 1; i++)
    {
      printf("res[%d] = %x\n",i,gRes[i]);
    }                 
                                             
                  
}

void vvdscmacjor_utm2()
{
    size_t vl, avl;
    uint32_t vtypeE;
    int32_t shiftBit = 0;
    
    vint32m2_t vAcc,vA;
   
    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl], %[avl], %[vtype]": [vl] "=r" (vl) : [avl] "r" (avl), [vtype] "r" (vtypeE)); 
    
    asm volatile("vle32.v %[vA], (%[gMacj]);" 
                  :[vA]"+vr"(vA)
                  :[gMacj]"r"(gMacj)); 
                  
    asm volatile("vdsmacini.s %[shiftBit];" : :[shiftBit]"r"(shiftBit)); 
                                                     
    asm volatile("vdscmacjor.vv %[vAcc],%[vA], %[vA];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vA]"vr"(vA));  
                  
    asm volatile("vse32.v %[vAcc], (%[gRes]);" 
                  :[vAcc]"=vr"(vAcc)
                  :[gRes]"r"(gRes)); 
     int32_t i;                         
   for (i = 0; i < 1; i++)
    {
      printf("res[%d] = %x\n",i,gRes[i]);
    }                                                                
}
