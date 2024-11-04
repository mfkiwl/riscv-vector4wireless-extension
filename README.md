# riscv-vector4wireless-extension

## Introduction

Currently, RVV1.0 vector extensions are mostly applied in AI and image processing fields, but these basic vector instructions cannot fully meet the needs of wireless broadband signal processing scenarios, such as complex number processing requirements in 4G/5G communications and the need for high-precision fixed-point dynamic scaling. Therefore, it is necessary to add more effective vector extension instructions for the wireless communication field, especially in 4G,5G, wifi and some other wireless areas, which account for a considerable portion of vector signal processing demands. Vector signal processing extension instruction sets based on RVV extension can not only promote rapid development in 4G and 5G signal processing fields but also promote RISC-V applications in wireless signal processing.

This instruction set, as a supplementary instruction set to RISC-V RVV 1.0 version, mainly targets for wideband wireless communication signal processing needs. This vector wireless broadband extension instruction is denoted by 'w' (Wireless), and it's based on the basic v extension, thus can be defined as 'Zvw'. The following instruction types were added for wireless broadband applications:

* Complex fixed-point arithmetic instructions
* Complex compression format conversion instructions
* Special load and store instructions
* Unified Fast non-linear operation instructions
* Efficient inter-element operation instructions
* Dynamic scaling multiply-accumulate operation instructions

The "operation" part of instructions in this document refers to the operation of each element in the vector processor. The loop count follows the standard RVV specifications of LMUL and VL; operands are controlled by SEW; whether each element is operated is controlled by vm; the default values of non-operated elements are controlled by mask agnostic and tail agnostic as defined by RVV specification.
