/**
 * Copyright (c) 2022 - 2023, WingSemi Technology LTD.
 *
 * All rights reserved.
 */
#include <stdio.h>
#include <riscv_vector.h>

vint8m1_t test_vdsmac_vs_i8m1(vint8m1_t vs2, vint8m1_t vs1, size_t vl) {
    return __riscv_vdsmac_vs_i8m1(vs2, vs1, 0, vl);
}

size_t vl = 4;

char dest[] = "Hello!";

int main()
{
    vint8m1_t vs2, vs1;
    vs2 = __riscv_vle8_v_i8m1(dest, vl);
    vint8m1_t vd = test_vdsmac_vs_i8m1(vs2, vs1, vl);
    __riscv_vse8_v_i8m1(dest, vd, vl);

    printf("%s\r\n", dest);
    return 0;
}
