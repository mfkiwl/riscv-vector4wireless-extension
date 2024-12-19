#include <stdio.h>
#include <riscv_vector.h>
#include "op_common.h"
#include "op_const_coef.h"

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
    volatile uint32_t addr0,addr1;
    uint32_t vcsrA0M0R0Sa = ACCSFT0 | MULSFT0 | VXRM_RNU | VXSAT1;
    uint32_t vtypeL1E32 = MA | TA | E32 | M1;
    uint32_t loop;

    asm volatile ("csrw vcsr,%[vcsrA0M0R0Sa]"::[vcsrA0M0R0Sa] "r" (vcsrA0M0R0Sa));

    for (loop = 0; loop < m6; loop++)
    {
        asm volatile ("vsetvl %v1,%[av1],%[vtype]":[v1] "=r" (vl1): [av1] "r" (m4),[vtype] "r" (vtypeL1E32));
        addr0 = pParam->m12 + loop * 2 * m4 * 4;
        addr1 = pParam->m12 + (loop * 2 + 1) * m4 * 4;
        asm volatile (
            "vle32.v v1,(%[addr0]);\
            vle32.v v2,(%[addr1]);"
            :
            : [addr0] "r" (addr0),[addr1] "r" (addr1)
            : "memory"
        );
    }
}


void model1_ut()
{
    int32_t result = 1;
    size_t count0 = 0;
    size_t count1 = 0;
    int32_t result0,result1;
    op_model1_param_t para;
    int32_t cirSel[64] =
    {
        0xffdb19c5,0x001b02d8,0xfff602fd,0xffd8fd57,0x001cfd50,0x001f0267,0xffee0265,0xffe4fdea,
        0x003ffdf5,0x004e01c1,0xffcb01aa,0xffddfec7,0x003cfed1,0x002c00ce,0xffae00d1,0x002dff9c,
        0xfffa19b5,0x001b02d1,0xfff802df,0xffc7fd50,0x0024fd50,0x001e026d,0xffd9025e,0xffa7fe17,
        0x004bfef0,0x004901a4,0xffb0019c,0xffdcfe9f,0x004dfecb,0x002f00e1,0xffd700e9,0x004bff77,
        0xffec19b6,0x001402dc,0xffdf02db,0xffcafd59,0x0026fd46,0x00290263,0xffcd0259,0xffd3fdf0,
        0x0028fde9,0x002b019e,0xffeb01a2,0xfff4feba,0x0033fec9,0x002100da,0xffbd00e5,0x0016ff8f,
        0xfffa19c9,0x000702e6,0xffdd02f7,0xffe7fd64,0x0013fd5c,0x00210246,0xffd60265,0xffcefdfb,
        0x002cfde4,0x004501b4,0xffd201c1,0xffbefec8,0x002dfec2,0x001600cc,0xffc800d9,0x0016ff9b
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
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C
    };
    uint32_t vsrc2[32] =
    {
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE
    };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C
    };
    uint32_t i;
    size_t vl;
    uint32_t len = 1;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;
    uint32_t vcsrA0M15R0Sa = ACCSFT0 | MULSFT15 | VXRM_RNU | VXSAT1;

    asm volatile("vsetvl %[vl],%[avl],%[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile(
        "vmv.s.x v0,%[vs1];\
        vmv.s.x v6,%[vs2];\
        csrw vcsr,%[vcsrA0M15R0Sa];\
        vdscmul.vs v12,v6,v0;\
        vmv.x.s %[vd],v12;"
        : [vd] "=r" (vd)
        : [vcsrA0M15R0Sa] "r" (vcsrA0M15R0Sa),[vs1] "r" (vs1),[vs2] "r" (vs2)
    );

    if (vd == vdTv)
    {
        printf("vdscmul.vs_ut passed!\n");
    }
    else
    {
        printf("dscmul.vs_ut data comparison failed vd[%d] vdTv[%d]!\n",vd,vdTv);
    }

    len = 32;
    asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    vle32.v v6,(%[vsrc2]);\
    csrw vcsr,%[vcsrA0M15R0Sa];\
    vdscmul.vv v12,v6,v0;\
    vse32.v v12,(%[vdst]);"
    :
    : [vcsrA0M15R0Sa] "r" (vcsrA0M15R0Sa),[vsrc1] "r" (vsrc1),[vsrc2] "r" (vsrc2),[vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("dscmul.vv_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst[i],vdstTv[i]);
            return;
        }
    }
    printf("vdscmul.vv_ut passed!\n");
}

void vdscmac_ut()
{
    uint32_t vs1 = 0x1C1C;
    uint32_t vs2 = 0x7FFE;
    uint32_t vd;
    uint32_t vdTv = 0x384;
    uint32_t vsrc1[32] =
    {
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,
        0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C,0x00001C1C
    };
    uint32_t vsrc2[32] =
    {
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,
        0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE,0x00007FFE
    };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384
    };
   uint32_t i;

    size_t vl;
    uint32_t len = 1;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;
    uint32_t vcsrA4M15R0Sa = ACCSFT4 | MULSFT15 | VXRM_RNU | VXSAT1;

    asm volatile("vsetvl %[vl],%[avl],%[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile(
        "vmv.s.x v0,%[vs1];\
        vmv.s.x v6,%[vs2];\
        csrw vcsr,%[vcsrA4M15R0Sa];\
        vdsmacini.i 15;\
        vdscmac.vs v6,v0;\
        vdscmaco.vs v12,v6,v0;\
        vmv.x.s %[vd],v12;"
        : [vd] "=r" (vd)
        : [vcsrA4M15R0Sa] "r" (vcsrA4M15R0Sa),[vs1] "r" (vs1),[vs2] "r" (vs2)
        );

    if (vd == vdTv)
    {
        printf("vdscmac.vs_ut passed!\n");
    }
    else
    {
        printf("dscmac.vs_ut data comparison failed vd[%d] vdTv[%d]!\n",vd,vdTv);
    }

   len = 32;
   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    vle32.v v6,(%[vsrc2]);\
    csrw vcsr,%[vcsrA4M15R0Sa];\
    vdsmacini.i 15;\
    vdscmac.vv v6,v0;\
    vdscmaco.vv v12,v6,v0;\
    vse32.v v12,(%[vdst]);"
    :
    : [vcsrA4M15R0Sa] "r" (vcsrA4M15R0Sa),[vsrc1] "r" (vsrc1),[vsrc2] "r" (vsrc2),[vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("dscmac.vv_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst[i],vdstTv[i]);
            return;
        }
    }
    printf("vdscmac.vv_ut passed!\n");
}

void vperm_ut()
{
    uint32_t vsrc1[32] =
    {
        0x00001C1C,0x00001C1D,0x00001C1E,0x00001C1F,0x00001C20,0x00001C21,0x00001C22,0x00001C23,
        0x00001C24,0x00001C25,0x00001C26,0x00001C27,0x00001C28,0x00001C29,0x00001C2A,0x00001C2B,
        0x00001C2C,0x00001C2D,0x00001C2E,0x00001C2F,0x00001C30,0x00001C31,0x00001C32,0x00001C33,
        0x00001C34,0x00001C35,0x00001C36,0x00001C37,0x00001C38,0x00001C39,0x00001C3A,0x00001C3B
    };
    uint32_t vdst[32];
    uint32_t vdst8Tv[32] =
    {
        0x00001C3B,0x00001C3A,0x00001C39,0x00001C38,0x00001C37,0x00001C36,0x00001C35,0x00001C34,
        0x00001C33,0x00001C32,0x00001C31,0x00001C30,0x00001C2F,0x00001C2E,0x00001C2D,0x00001C2C,
        0x00001C2B,0x00001C2A,0x00001C29,0x00001C28,0x00001C27,0x00001C26,0x00001C25,0x00001C24,
        0x00001C23,0x00001C22,0x00001C21,0x00001C20,0x00001C1F,0x00001C1E,0x00001C1D,0x00001C1C
    };
    uint32_t i;

    size_t vl;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

    asm volatile("vsetvl %[vl],%[avl],%[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    vperm.vi v1,v0,8;\
    vse32.v v1,(%[vdst]);"
    :
    : [vsrc1] "r" (vsrc1),[vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdst8Tv[i])
        {
            printf("vperm0_ut data comparison failed at index %d vdst[%d] vdst8Tv[%d]!\n",i,vdst,vdst8Tv);
            return; 
            }
    }
    printf("vperm0_ut passed!\n");
}

int32_t gAddr[64] = {1};
int32_t gRes[64] = {0};
int32_t op_testMvZero()
{
    size_t vl,avl;
    uint32_t vtypeE;

    vint32m2_t vA;
                   
    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl],%[avl],%[vtype]": [vl] "=r" (vl) : [avl] "r" (avl),[vtype] "r" (vtypeE));

/*

 1000546:	5e003157          	vmv.v.i	v2,0
 100054a:	02076107          	vle32.v	v2,(a4)
 100054e:	56200057          	vdsmacini.v	v2
 1000552:	bbc18793          	addi	a5,gp,-1092 # 2000143c <gRes>
 1000556:	0207e127          	vse32.v	v2,(a5)
*/
    asm volatile("vle32.v %[vA],(%[gAddr]);\
                  vdsmacini.v %[vA];" 
                  :[vA]"+vr"(vA)
                  :[gAddr]"r"(gAddr)); 
                                      
    asm volatile("vse32.v %[vA],(%[gRes]);" 
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
    size_t vl,avl;
    uint32_t vtypeE;
    int32_t shiftBit = 0;
    
    vint32m1_t vAcc,vA;
                   
    avl = 32;
    vtypeE = TA | MA | M1 | E32;
    asm volatile("vsetvl %[vl],%[avl],%[vtype]": [vl] "=r" (vl) : [avl] "r" (avl),[vtype] "r" (vtypeE)); 
    
    asm volatile("vle32.v %[vA],(%[gMacj]);" 
                  :[vA]"+vr"(vA)
                  :[gMacj]"r"(gMacj)); 
                  
    asm volatile("vdsmacini.s %[shiftBit];" : :[shiftBit]"r"(shiftBit)); 
                                   
    asm volatile("vdscmacj.vv %[vA],%[vA];" 
                  :
                  :[vA]"vr"(vA));
                  
    asm volatile("vdscmacjor.vv %[vAcc],%[vA],%[vA];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vA]"vr"(vA));
                  

    asm volatile("vse32.v %[vAcc],(%[gRes]);" 
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
    size_t vl,avl;
    uint32_t vtypeE;
    int32_t shiftBit = 0;
    
    vint32m2_t vAcc,vA;
   
    avl = 64;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl],%[avl],%[vtype]": [vl] "=r" (vl) : [avl] "r" (avl),[vtype] "r" (vtypeE)); 
    
    asm volatile("vle32.v %[vA],(%[gMacj]);" 
                  :[vA]"+vr"(vA)
                  :[gMacj]"r"(gMacj)); 
                  
    asm volatile("vdsmacini.s %[shiftBit];" : :[shiftBit]"r"(shiftBit)); 
                                                     
    asm volatile("vdscmacjor.vv %[vAcc],%[vA],%[vA];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vA]"vr"(vA));  
                  
    asm volatile("vse32.v %[vAcc],(%[gRes]);" 
                  :[vAcc]"=vr"(vAcc)
                  :[gRes]"r"(gRes)); 
     int32_t i;                         
   for (i = 0; i < 1; i++)
    {
      printf("res[%d] = %x\n",i,gRes[i]);
    }                                                  
                  
}


int32_t gMacjMAXNumI1[32] ={
0x00007FFF,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};

int32_t gMacjMAXNumI2[32] ={
0x7FFF0000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};

int32_t gMacjMAXNumR1[32] ={
0x00007FFF,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};

int32_t gMacjMAXNumR2[32] ={
0x00007FFF,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};

void vdscmacjoi_ut()
{
    size_t vl,avl;
    uint32_t vtypeE;
    int32_t shiftBit = 0;
    
    vint32m1_t vAcc,vS1,vS2;
    vint32m1_t vZ;     
    avl = 32;
    vtypeE = TA | MA | M1 | E32;
    asm volatile("vsetvl %[vl],%[avl],%[vtype]": [vl] "=r" (vl) : [avl] "r" (avl),[vtype] "r" (vtypeE)); 
    
    uint32_t vcsrA0M0R0Sa = ACCSFT0 | MULSFT0 | VXRM_RNU | VXSAT1;    
    asm volatile("csrw vcsr,%[vcsrA0M0R0Sa];"
    :
    : [vcsrA0M0R0Sa] "r" (vcsrA0M0R0Sa)
    );   
     
    asm volatile("vmv.v.i %[vZ],0;"
                 :[vZ]"=vd"(vZ)
                 :);
                 
    asm volatile("vdscmacjo.vv %[vAcc],%[vZ],%[vZ];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vZ]"vr"(vZ)); 
    asm volatile("vdsmacini.s %[shiftBit];" : :[shiftBit]"r"(shiftBit)); 
        
    asm volatile("vle32.v %[vS1],(%[gMacjMAXNumI1]);" 
                  :[vS1]"=vr"(vS1)
                  :[gMacjMAXNumI1]"r"(gMacjMAXNumI1)); 

    asm volatile("vle32.v %[vS2],(%[gMacjMAXNumI2]);" 
                  :[vS2]"=vr"(vS2)
                  :[gMacjMAXNumI2]"r"(gMacjMAXNumI2)); 
                                    
                               
    asm volatile("vdscmacj.vv %[vS2],%[vS1];" 
                  :
                  :[vS1]"vr"(vS1),[vS2]"vr"(vS2));
                                    
    asm volatile("vdscmacjoi.vv %[vAcc],%[vZ],%[vZ];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vZ]"vr"(vZ));

    asm volatile("vse32.v %[vAcc],(%[gRes]);" 
                  :[vAcc]"=vr"(vAcc)
                  :[gRes]"r"(gRes));   

    printf("img res = %x\n",gRes[0]);
    
    asm volatile("vdscmacjo.vv %[vAcc],%[vZ],%[vZ];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vZ]"vr"(vZ)); 
    asm volatile("vdsmacini.s %[shiftBit];" : :[shiftBit]"r"(shiftBit)); 
        
    asm volatile("vle32.v %[vS1],(%[gMacjMAXNumR1]);" 
                  :[vS1]"=vr"(vS1)
                  :[gMacjMAXNumR1]"r"(gMacjMAXNumR1)); 

    asm volatile("vle32.v %[vS2],(%[gMacjMAXNumR2]);" 
                  :[vS2]"=vr"(vS2)
                  :[gMacjMAXNumR2]"r"(gMacjMAXNumR2)); 
                                    
                               
    asm volatile("vdscmacj.vv %[vS1],%[vS1];" 
                  :
                  :[vS1]"vr"(vS1),[vS2]"vr"(vS2));
                                    
    asm volatile("vdscmacjor.vv %[vAcc],%[vZ],%[vZ];" 
                  :[vAcc]"=vr"(vAcc)
                  :[vZ]"vr"(vZ));

    asm volatile("vse32.v %[vAcc],(%[gRes]);" 
                  :[vAcc]"=vr"(vAcc)
                  :[gRes]"r"(gRes));   

    printf("real res = %x\n",gRes[0]);   
    
           
}

#pragma (push)
#pragma (4)//8 16
int32_t g_lse32Test[106] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
    33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64
};
void vlse32_ut()
{
    size_t vl,avl;
    uint32_t vtypeE;

    vint32m2_t vs;

    avl = 16;
    vtypeE = TA | MA | M2 | E32;
    asm volatile("vsetvl %[vl],%[avl],%[vtype]": [vl] "=r" (vl) : [avl] "r" (avl),[vtype] "r" (vtypeE)); 
    int32_t sample = 4;
    asm volatile("vlse32.v %[vs],(%[g_lse32Test]),%[sample];":[vs]"=vr"(vs):[g_lse32Test]"r"(g_lse32Test),[sample]"r"(sample));
}
#pragma (pop)


int32_t g_unpacktestData[64] = {
0x12345671,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678,
0x12345672,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678,
0x12345673,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678,
0x12345674,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678,
0x12345675,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678,
0x12345676,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678,
0x12345677,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678,
0x12345678,0x12345672,0x12345673,0x12345674,0x12345675,0x12345676,0x12345677,0x12345678};

int32_t g_unpackResultQ[64];
int32_t g_unpackResultQT[64] = {
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234
};
int32_t g_unpackResultI[64];
int32_t g_unpackResultIT[64] = {
0x5671,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678,
0x5672,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678,
0x5673,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678,
0x5674,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678,
0x5675,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678,
0x5676,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678,
0x5677,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678,
0x5678,0x5672,0x5673,0x5674,0x5675,0x5676,0x5677,0x5678
};

void op_testUnpack()
{
    size_t vl,avl;
    uint32_t vType;
    
    vint32m2_t vX;
    vint32m2_t vQ,vI;
    vint32m2_t vQr,vIr;
    
    avl = 64;
    vType = TA | MA | M2 | E32;
    uint32_t shift = 16;
    
    asm volatile("vsetvl %[vl],%[avl],%[vtype];"
                  :[vl] "=r" (vl)
                  :[avl] "r" (avl),[vtype] "r" (vType));
                  
    asm volatile("vle32.v %[vX],(%[g_unpacktestData]);"
                  :[vX]"=vd"(vX)
                  :[g_unpacktestData]"r"(g_unpacktestData));                  
                  
    asm volatile("vcunpackr.v %[vI],%[vX];"
                  :[vI]"=vd"(vI)
                  :[vX]"vd"(vX));
                  
    asm volatile("vcunpacki.v %[vQ],%[vX];"
                  :[vQ]"=vd"(vQ)
                  :[vX]"vd"(vX));


    asm volatile("vsrl.vx %[vIr],%[vI],%[shift];"
                  :[vIr]"=vd"(vIr)
                  :[vI]"vd"(vI),[shift]"r"(shift));

    asm volatile("vsrl.vx %[vQr],%[vQ],%[shift];"
                  :[vQr]"=vd"(vQr)
                  :[vQ]"vd"(vQ),[shift]"r"(shift));
                                    
    asm volatile("vse32.v  %[vIr],(%[g_unpackResultI]);"
                  :
                  :[g_unpackResultI]"r"(g_unpackResultI),[vIr]"vr"(vIr));    

    asm volatile("vse32.v  %[vQr],(%[g_unpackResultQ]);"
                  :
                  :[g_unpackResultQ]"r"(g_unpackResultQ),[vQr]"vr"(vQr));    
    int i;
    for (i = 0; i < 64; i++)
    {
        if (g_unpackResultQT[i] != g_unpackResultQ[i])
        {
            printf("vcunpackr  fail %x\n",g_unpackResultQ[i]);
            return;
        }
    }        
    printf("vcunpackr  pass\n");                    

    for (i = 0; i < 64; i++)
    {
        if (g_unpackResultIT[i] != g_unpackResultI[i])
        {
            printf("vcunpacki  fail\n");
            return;
        }
    }        
    printf("vcunpacki  pass\n");     
    
}


int32_t g_macestData1[64] = {
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008
};
int32_t g_macestData2[64] = {
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001
};
int32_t g_macestDataRes[64] = {0};
int32_t g_macestDataResT[64] = {
0x4,0x8,0xc,0x10,0x14,0x18,0x1C,0x20,
0x4,0x8,0xc,0x10,0x14,0x18,0x1C,0x20,
0x4,0x8,0xc,0x10,0x14,0x18,0x1C,0x20,
0x4,0x8,0xc,0x10,0x14,0x18,0x1C,0x20,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
};
void op_testMac()
{
printf("vdsmac  start\n"); 
    size_t vl,avl;
    uint32_t vType;
    
    vint32m2_t vS1,vS2;
    vint32m2_t vMac;

    avl = 64;
    vType = TA | MA | M2 | E32;

    asm volatile("vsetvl %[vl],%[avl],%[vtype];"
                  :[vl] "=r" (vl)
                  :[avl] "r" (avl),[vtype] "r" (vType));

    uint32_t vcsrA0M0R0Sa = ACCSFT0 | MULSFT0 | VXRM_RNU | VXSAT1;    
    asm volatile("csrw vcsr,%[vcsrA0M0R0Sa];"
    :
    : [vcsrA0M0R0Sa] "r" (vcsrA0M0R0Sa)
    );   
    
    asm volatile("vle32.v %[vS1],(%[g_macestData1]);"
                  :[vS1]"=vd"(vS1)
                  :[g_macestData1]"r"(g_macestData1));                  

    asm volatile("vle32.v %[vS2],(%[g_macestData2]);"
                  :[vS2]"=vd"(vS2)
                  :[g_macestData2]"r"(g_macestData2)); 
                  
    asm volatile("vdsmac.vv %[vS1],%[vS2];" 
                  :
                  :[vS1]"vr"(vS1),[vS2]"vr"(vS2));                
    
    asm volatile("vdsmaco.vv %[vMac],%[vS1],%[vS2];" 
                  :[vMac]"=vr"(vMac)
                  :[vS1]"vr"(vS1),[vS2]"vr"(vS2));   

                 
    asm volatile("vse32.v  %[vMac],(%[g_macestDataRes]);"
                  :
                  :[g_macestDataRes]"r"(g_macestDataRes),[vMac]"vr"(vMac));   
    int i;
    for (i = 0; i < 32; i++)
    {
        if (g_macestDataResT[i] != g_macestDataRes[i])
        {
            printf("vdsmac fail [%d]=%x %x\n",i,g_macestDataResT[i],g_macestDataRes[i]);
            //return;
        }
    }        
    printf("vdsmac  pass\n");                  
}


int32_t g_multestData2[64] = {
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,0x7fff7fff,
};

int32_t g_multestData1[64] = {
0x00010001,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,
0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002, 
0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,
0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,
0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,
0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,
0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,
0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002,0x00020002 
};

int32_t g_muljestDataRes[64];
int32_t g_muljestDataResT[64] = {
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF};
void op_testMulj()
{
    size_t vl,avl;
    uint32_t vType;
    
    vint32m2_t vD,vS1,vS2;

    avl = 64;
    vType = TA | MA | M2 | E32;

    asm volatile("vsetvl %[vl],%[avl],%[vtype];"
                  :[vl] "=r" (vl)
                  :[avl] "r" (avl),[vtype] "r" (vType));
                  
    uint32_t vcsrA0M0R0Sa = ACCSFT0 | MULSFT1 | VXRM_RNU | VXSAT1;    
    asm volatile("csrw vcsr,%[vcsrA0M0R0Sa];"
    :
    : [vcsrA0M0R0Sa] "r" (vcsrA0M0R0Sa)
    );      
    
    asm volatile("vle32.v %[vS2],(%[g_multestData2]);"
                  :[vS2]"=vd"(vS2)
                  :[g_multestData2]"r"(g_multestData2)); 
    asm volatile("vle32.v %[vS1],(%[g_multestData1]);"
                  :[vS1]"=vd"(vS1)
                  :[g_multestData1]"r"(g_multestData1));                                 
    asm volatile("vdscmulj.vs %[vD],%[vS2],%[vS1];" 
                  :[vD]"=&vr"(vD)
                  :[vS1]"vr"(vS1),[vS2]"vr"(vS2));  
    asm volatile("vse32.v  %[vD],(%[g_muljestDataRes]);"
                  :
                  :[g_muljestDataRes]"r"(g_muljestDataRes),[vD]"vr"(vD));   
    int i;
    for (i = 0; i < 64; i++)
    {
        if (g_muljestDataResT[i] != g_muljestDataRes[i])
        {
            printf("vdscmulj  fail %x\n",g_muljestDataRes[i]);
            return;
        }
    }        
    printf("vdscmulj  pass\n");
}

void vconj_ut()
{
    uint32_t vsrc1[64] =
    {
        0x80007FFF,0x7FFF7FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
   };
    uint32_t vdst[64];
    uint32_t vdstTv[64] =
    {
        0x7FFF7FFF,0x80017FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,
        0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,
        0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,
        0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,
        0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,
        0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,
        0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,
        0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF,0x7FFF7FFF
    };
   uint32_t i;
    size_t vl;
    uint32_t len = 48;
    uint32_t vtypeL2E32 = MA | TA | M2 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL2E32));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    vconj.v v2,v0;\
    vse32.v v2,(%[vdst]);"
    :
    :  [vsrc1] "r" (vsrc1),[vdst] "r" (vdst)
    );

    for (i = 0; i < len ; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("vconj.v_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst[i],vdstTv[i]);
            return;
        }
    }
    printf("vconj.v_ut passed!\n");
}


void vdsredsum_ut()
{
    uint32_t vsrc1[64] =
    {
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
        0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,0x00007FFF,
   };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x00007FFF,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384
    };
   uint32_t i;
    size_t vl;
    uint32_t len = 64;
    uint32_t vtypeL2E32 = MA | TA | M2 | E32;
    uint32_t vcsrA6M0R0Sa = ACCSFT6 | MULSFT0 | VXRM_RNU | VXSAT1;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL2E32));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    csrw vcsr,%[vcsrA6M0R0Sa];\
    vdsredsum.v v2,v0;\
    vse32.v v2,(%[vdst]);"
    :
    : [vcsrA6M0R0Sa ] "r" (vcsrA6M0R0Sa ),[vsrc1] "r" (vsrc1),[vdst] "r" (vdst)
    );

    for (i = 0; i < 1; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("vdsredsum.v_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst[i],vdstTv[i]);
            return;
        }
    }
    printf("vdsredsum.v_ut passed!\n");
}

void vdscredsum_ut()
{
    uint32_t vsrc1[64] =
    {
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
        0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,0x80007FFF,
   };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x80007FFF,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,
        0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384,0x00000384
    };
   uint32_t i;
    size_t vl;
    uint32_t len = 64;
    uint32_t vtypeL2E32 = MA | TA | M2 | E32;
    uint32_t vcsrA6M0R0Sa = ACCSFT6 | MULSFT0 | VXRM_RNU | VXSAT1;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL2E32));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    csrw vcsr,%[vcsrA6M0R0Sa];\
    vdscredsum.v v2,v0;\
    vse32.v v2,(%[vdst]);"
    :
    : [vcsrA6M0R0Sa ] "r" (vcsrA6M0R0Sa ),[vsrc1] "r" (vsrc1),[vdst] "r" (vdst)
    );

    for (i = 0; i < 1; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("vdscredsum.v_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst[i],vdstTv[i]);
            return;
        }
    }
    printf("vdscredsum.v_ut passed!\n");
}


void vfsl_ut()
{
    uint32_t vsrc1[32] =
    {
        0x00001C1C,0x00001C1D,0x00001C1E,0x00001C1F,0x00001C20,0x00001C21,0x00001C22,0x00001C23,
        0x00001C24,0x00001C25,0x00001C26,0x00001C27,0x00001C28,0x00001C29,0x00001C2A,0x00001C2B,
        0x00001C2C,0x00001C2D,0x00001C2E,0x00001C2F,0x00001C30,0x00001C31,0x00001C32,0x00001C33,
        0x00001C34,0x00001C35,0x00001C36,0x00001C37,0x00001C38,0x00001C39,0x00001C3A,0x00001C3B
    };
    uint32_t vsrc2[32] =
    {
        0x00001C1C,0x00001C1D,0x00001C1E,0x00001C1F,0x00001C20,0x00001C21,0x00001C22,0x00001C23,
        0x00001C24,0x00001C25,0x00001C26,0x00001C27,0x00001C28,0x00001C29,0x00001C2A,0x00001C2B,
        0x00001C2C,0x00001C2D,0x00001C2E,0x00001C2F,0x00001C30,0x00001C31,0x00001C32,0x00001C33,
        0x00001C34,0x00001C35,0x00001C36,0x00001C37,0x00001C38,0x00001C39,0x00001C3A,0x00001C3B
    };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x00001C2C,0x00001C2D,0x00001C2E,0x00001C2F,0x00001C30,0x00001C31,0x00001C32,0x00001C33,
        0x00001C34,0x00001C35,0x00001C36,0x00001C37,0x00001C38,0x00001C39,0x00001C3A,0x00001C3B,
        0x00001C1C,0x00001C1D,0x00001C1E,0x00001C1F,0x00001C20,0x00001C21,0x00001C22,0x00001C23,
        0x00001C24,0x00001C25,0x00001C26,0x00001C27,0x00001C28,0x00001C29,0x00001C2A,0x00001C2B,
    };
    uint32_t i;

    size_t vl;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;
    uint32_t vcsrA0F16M0R0Sa = ACCSFT0 | FSFT16| MULSFT0 | VXRM_RNU | VXSAT1;

    asm volatile("vsetvl %[vl],%[avl],%[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len),[vtype] "r" (vtypeL1E32));
    asm volatile ("csrw vcsr,%[vcsrA0F16M0R0Sa]"::[vcsrA0F16M0R0Sa] "r" (vcsrA0F16M0R0Sa));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    vle32.v v1,(%[vsrc2]);\
    vfsl.vv v2,v1,v0;\
    vse32.v v2,(%[vdst]);"
    :
    : [vsrc1] "r" (vsrc1),[vsrc2] "r" (vsrc2),[vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("vfsl_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst,vdstTv);
            return; 

            }
    }
    printf("vfsl_ut passed!\n");
}

void vfsr_ut()
{
    uint32_t vsrc1[32] =
    {
        0x00001C1C,0x00001C1D,0x00001C1E,0x00001C1F,0x00001C20,0x00001C21,0x00001C22,0x00001C23,
        0x00001C24,0x00001C25,0x00001C26,0x00001C27,0x00001C28,0x00001C29,0x00001C2A,0x00001C2B,
        0x00001C2C,0x00001C2D,0x00001C2E,0x00001C2F,0x00001C30,0x00001C31,0x00001C32,0x00001C33,
        0x00001C34,0x00001C35,0x00001C36,0x00001C37,0x00001C38,0x00001C39,0x00001C3A,0x00001C3B
    };
    uint32_t vsrc2[32] =
    {
        0x00001C1C,0x00001C1D,0x00001C1E,0x00001C1F,0x00001C20,0x00001C21,0x00001C22,0x00001C23,
        0x00001C24,0x00001C25,0x00001C26,0x00001C27,0x00001C28,0x00001C29,0x00001C2A,0x00001C2B,
        0x00001C2C,0x00001C2D,0x00001C2E,0x00001C2F,0x00001C30,0x00001C31,0x00001C32,0x00001C33,
        0x00001C34,0x00001C35,0x00001C36,0x00001C37,0x00001C38,0x00001C39,0x00001C3A,0x00001C3B
    };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x00001C2C,0x00001C2D,0x00001C2E,0x00001C2F,0x00001C30,0x00001C31,0x00001C32,0x00001C33,
        0x00001C34,0x00001C35,0x00001C36,0x00001C37,0x00001C38,0x00001C39,0x00001C3A,0x00001C3B,
        0x00001C3B,0x00001C3A,0x00001C39,0x00001C38,0x00001C37,0x00001C36,0x00001C35,0x00001C34,
        0x00001C33,0x00001C32,0x00001C31,0x00001C30,0x00001C2F,0x00001C2E,0x00001C2D,0x00001C2C
    };
    uint32_t i;

    size_t vl;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;
    uint32_t vcsrA0F16M0R0Sa = ACCSFT0 | FSFT16| MULSFT0 | VXRM_RNU | VXSAT1;

    asm volatile("vsetvl %[vl],%[avl],%[vtype]"
                 : [vl] "=r" (vl)
                 : [avl] "r" (len),[vtype] "r" (vtypeL1E32));
    asm volatile ("csrw vcsr,%[vcsrA0F16M0R0Sa]"::[vcsrA0F16M0R0Sa] "r" (vcsrA0F16M0R0Sa));

    asm volatile(
    "vle32.v v0,(%[vsrc1]);\
    vle32.v v1,(%[vsrc2]);\
    vfsr.vv v2,v1,v0;\
    vse32.v v2,(%[vdst]);"
    :
    : [vsrc1] "r" (vsrc1),[vsrc2] "r" (vsrc2),[vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("vfsr_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst,vdstTv);
            return; 
            }
    }
    printf("vfsr_ut passed!\n");
}

void vluxei32_ut()
{
    uint32_t vsrc1[16] =
    {
        32,30,33,29,34,35,36,37,
        28,27,26,40,41,24,25,23,
   };
    uint32_t addr[64] =
    {
        0x80007FFF,0x80007FFE,0x80007FFD,0x80007FFC,0x80007FFB,0x80007FFA,0x80007FF9,0x80007FF8,
        0x80007FF7,0x80007FF6,0x80007FF5,0x80007FF4,0x80007FF3,0x80007FF2,0x80007FF1,0x80007FF0,
        0x80007FEF,0x80007FEE,0x80007FED,0x80007FEC,0x80007FEB,0x80007FEA,0x80007FE9,0x80007FE8,
        0x80007FE7,0x80007FE6,0x80007FE5,0x80007FE4,0x80007FE3,0x80007FE2,0x80007FE1,0x80007FE0,
        0x80007FDF,0x80007FDE,0x80007FDD,0x80007FDC,0x80007FDB,0x80007FDA,0x80007FD9,0x80007FD8,
        0x80007FD7,0x80007FD6,0x80007FD5,0x80007FD4,0x80007FD3,0x80007FD2,0x80007FD1,0x80007FD0,
        0x80007FCF,0x80007FCE,0x80007FCD,0x80007FCC,0x80007FCB,0x80007FCA,0x80007FC9,0x80007FC8,
        0x80007FC7,0x80007FC6,0x80007FC5,0x80007FC4,0x80007FC3,0x80007FC2,0x80007FC1,0x80007FC0,
   };
    uint32_t vdst[32];
    uint32_t vdstTv[32] =
    {
        0x80007FDF,0x80007FE1,0x80007FDE,0x80007FE2,0x80007FDD,0x80007FDC,0x80007FDB,0x80007FDA,
        0x80007FE3,0x80007FE4,0x80007FE5,0x80007FD7,0x80007FD6,0x80007FE7,0x80007FE6,0x80007FE8
    };
    uint32_t i;
    size_t vl;
    uint32_t len = 16;
    uint32_t vtypeL2E32 = MA | TA | M2 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL2E32));

    asm volatile("vle32.v v0,(%[vsrc1]);": : [vsrc1] "r" (vsrc1));

    asm volatile(
    "vluxei32.v v2,(%[addr]),v0;\
    vse32.v v2,(%[vdst]);"
    :
    : [addr ] "r" (addr ),[vdst] "r" (vdst)
    );

    for (i = 0; i < len; i++)
    {
        if (vdst[i] != vdstTv[i])
        {
            printf("vluxei32.v_ut data comparison failed at index %d vdst[%d] vdstTv[%d]!\n",i,vdst[i],vdstTv[i]);
            return;
        }
    }
    printf("vluxei32.v_ut passed!\n");
}


void nolinear_recip8_ut()
{
    uint32_t vsrc1[32];
    uint32_t mantissa[32];
    uint32_t exponent[32];
    uint32_t mantissaTv[32];
    uint32_t exponentTv[32];
    uint32_t i;
    size_t vl;
    uint32_t point = 1;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile("vle32.v v0,(%[vsrc1]);": : [vsrc1] "r" (vsrc1));

    asm volatile(
    "vlnlp.s %[recip_seg08_cfg_table];\
    vnlm.vs v1, v0, %[point];\
    vnle.vs v2, v0, %[point];\
    vse32.v v1,(%[mantissa]);\
    vse32.v v2,(%[exponent]);"
    :
    : [recip_seg08_cfg_table ] "r" (recip_seg08_cfg_table ),[point] "r" (point),[mantissa] "r" (mantissa),[exponent] "r" (exponent));

    for (i = 0; i < len; i++)
    {
        if (mantissa[i] != mantissaTv[i])
        {
            printf("recip_seg08_ut mantissa data comparison failed at index %d mantissa[%d] mantissaTv[%d]!\n",i,mantissa[i],mantissaTv[i]);
            return;
        }
        if (exponent[i] != exponentTv[i])
        {
            printf("recip_seg08_ut exponent data comparison failed at index %d exponent[%d] exponentTv[%d]!\n",i,exponent[i],exponentTv[i]);
            return;
        }
    }
    printf("recip_seg08_ut passed!\n");
}

void nolinear_sqrt8_ut()
{
    uint32_t vsrc1[32];
    uint32_t mantissa[32];
    uint32_t exponent[32];
    uint32_t mantissaTv[32];
    uint32_t exponentTv[32];
    uint32_t i;
    size_t vl;
    uint32_t point = 1;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile("vle32.v v0,(%[vsrc1]);": : [vsrc1] "r" (vsrc1));

    asm volatile(
    "vlnlp.s %[sqrt_seg08_cfg_table];\
    vnlm.vs v1, v0, %[point];\
    vnle.vs v2, v0, %[point];\
    vse32.v v1,(%[mantissa]);\
    vse32.v v2,(%[exponent]);"
    :
    : [sqrt_seg08_cfg_table ] "r" (sqrt_seg08_cfg_table ),[point] "r" (point),[mantissa] "r" (mantissa),[exponent] "r" (exponent));

    for (i = 0; i < len; i++)
    {
        if (mantissa[i] != mantissaTv[i])
        {
            printf("sqrt_seg08_ut mantissa data comparison failed at index %d mantissa[%d] mantissaTv[%d]!\n",i,mantissa[i],mantissaTv[i]);
            return;
        }
        if (exponent[i] != exponentTv[i])
        {
            printf("sqrt_seg08_ut exponent data comparison failed at index %d exponent[%d] exponentTv[%d]!\n",i,exponent[i],exponentTv[i]);
            return;
        }
    }
    printf("sqrt_seg08_ut passed!\n");
}


void nolinear_recipSqrt8_ut()
{
    uint32_t vsrc1[32];
    uint32_t mantissa[32];
    uint32_t exponent[32];
    uint32_t mantissaTv[32];
    uint32_t exponentTv[32];
    uint32_t i;
    size_t vl;
    uint32_t point = 1;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile("vle32.v v0,(%[vsrc1]);": : [vsrc1] "r" (vsrc1));

    asm volatile(
    "vlnlp.s %[recipSqrt_seg08_cfg_table];\
    vnlm.vs v1, v0, %[point];\
    vnle.vs v2, v0, %[point];\
    vse32.v v1,(%[mantissa]);\
    vse32.v v2,(%[exponent]);"
    :
    : [recipSqrt_seg08_cfg_table ] "r" (recipSqrt_seg08_cfg_table ),[point] "r" (point),[mantissa] "r" (mantissa),[exponent] "r" (exponent));

    for (i = 0; i < len; i++)
    {
        if (mantissa[i] != mantissaTv[i])
        {
            printf("recipSqrt_seg08_ut mantissa data comparison failed at index %d mantissa[%d] mantissaTv[%d]!\n",i,mantissa[i],mantissaTv[i]);
            return;
        }
        if (exponent[i] != exponentTv[i])
        {
            printf("recipSqrt_seg08_ut exponent data comparison failed at index %d exponent[%d] exponentTv[%d]!\n",i,exponent[i],exponentTv[i]);
            return;
        }
    }
    printf("recipSqrt_seg08_ut passed!\n");
}

void nolinear_arctan8_ut()
{
    uint32_t vsrc1[32];
    uint32_t mantissa[32];
    uint32_t exponent[32];
    uint32_t mantissaTv[32];
    uint32_t exponentTv[32];
    uint32_t i;
    size_t vl;
    uint32_t point = 1;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile("vle32.v v0,(%[vsrc1]);": : [vsrc1] "r" (vsrc1));

    asm volatile(
    "vlnlp.s %[arctan_seg08_cfg_table];\
    vnlm.vs v1, v0, %[point];\
    vnle.vs v2, v0, %[point];\
    vse32.v v1,(%[mantissa]);\
    vse32.v v2,(%[exponent]);"
    :
    : [arctan_seg08_cfg_table ] "r" (arctan_seg08_cfg_table ),[point] "r" (point),[mantissa] "r" (mantissa),[exponent] "r" (exponent));

    for (i = 0; i < len; i++)
    {
        if (mantissa[i] != mantissaTv[i])
        {
            printf("arctan_seg08_ut mantissa data comparison failed at index %d mantissa[%d] mantissaTv[%d]!\n",i,mantissa[i],mantissaTv[i]);
            return;
        }
        if (exponent[i] != exponentTv[i])
        {
            printf("arctan_seg08_ut exponent data comparison failed at index %d exponent[%d] exponentTv[%d]!\n",i,exponent[i],exponentTv[i]);
            return;
        }
    }
    printf("arctan_seg08_ut passed!\n");
}

void nolinear_log2seg8_ut()
{
    uint32_t vsrc1[32];
    uint32_t mantissa[32];
    uint32_t exponent[32];
    uint32_t mantissaTv[32];
    uint32_t exponentTv[32];
    uint32_t i;
    size_t vl;
    uint32_t point = 1;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile("vle32.v v0,(%[vsrc1]);": : [vsrc1] "r" (vsrc1));

    asm volatile(
    "vlnlp.s %[log2_seg08_cfg_table];\
    vnlm.vs v1, v0, %[point];\
    vnle.vs v2, v0, %[point];\
    vse32.v v1,(%[mantissa]);\
    vse32.v v2,(%[exponent]);"
    :
    : [log2_seg08_cfg_table ] "r" (log2_seg08_cfg_table ),[point] "r" (point),[mantissa] "r" (mantissa),[exponent] "r" (exponent));

    for (i = 0; i < len; i++)
    {
        if (mantissa[i] != mantissaTv[i])
        {
            printf("log2_seg08_ut mantissa data comparison failed at index %d mantissa[%d] mantissaTv[%d]!\n",i,mantissa[i],mantissaTv[i]);
            return;
        }
        if (exponent[i] != exponentTv[i])
        {
            printf("log2_seg08_ut exponent data comparison failed at index %d exponent[%d] exponentTv[%d]!\n",i,exponent[i],exponentTv[i]);
            return;
        }
    }
    printf("log2_seg08_ut passed!\n");
}

void nolinear_log10seg8_ut()
{
    uint32_t vsrc1[32];
    uint32_t mantissa[32];
    uint32_t exponent[32];
    uint32_t mantissaTv[32];
    uint32_t exponentTv[32];
    uint32_t i;
    size_t vl;
    uint32_t point = 1;
    uint32_t len = 32;
    uint32_t vtypeL1E32 = MA | TA | M1 | E32;

   asm volatile("vsetvl %[vl],%[avl],%[vtype]"
             : [vl] "=r" (vl)
             : [avl] "r" (len),[vtype] "r" (vtypeL1E32));

    asm volatile("vle32.v v0,(%[vsrc1]);": : [vsrc1] "r" (vsrc1));

    asm volatile(
    "vlnlp.s %[log10_seg08_cfg_table];\
    vnlm.vs v1, v0, %[point];\
    vnle.vs v2, v0, %[point];\
    vse32.v v1,(%[mantissa]);\
    vse32.v v2,(%[exponent]);"
    :
    : [log10_seg08_cfg_table ] "r" (log10_seg08_cfg_table ),[point] "r" (point),[mantissa] "r" (mantissa),[exponent] "r" (exponent));

    for (i = 0; i < len; i++)
    {
        if (mantissa[i] != mantissaTv[i])
        {
            printf("log10_seg08_ut mantissa data comparison failed at index %d mantissa[%d] mantissaTv[%d]!\n",i,mantissa[i],mantissaTv[i]);
            return;
        }
        if (exponent[i] != exponentTv[i])
        {
            printf("log10_seg08_ut exponent data comparison failed at index %d exponent[%d] exponentTv[%d]!\n",i,exponent[i],exponentTv[i]);
            return;
        }
    }
    printf("log10_seg08_ut passed!\n");
}


