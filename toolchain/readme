########################0.目录说明#####################################
## gcc目录为编译器目录，其二进制文件在./gcc/bin下
## spike目录为仿真器目录
## openocd目录为硬件调试器目录
## debug目录为调试目录
## benchmark目录为用例目录
  ## env和common目录是编译依赖的一些文件以及链接脚本等
  ## coremark、dhrystone、helloworld是三个benchmark的源码

########################1.依赖工具安装#################################
  sudo apt install device-tree-compiler
  sudo apt install libboost-all-dev
  sudo apt install -y libusb-1.0-0-dev
  sudo apt install -y libjaylink-dev

########################2.WingGCC介绍及说明############################
## 2.0 WingGCC介绍
## WingGcc以riscv32-wing-elf-作为前缀，主要组件包括：
## riscv32-wing-elf-gcc 编译器
## riscv32-wing-elf-as  汇编器
## riscv32-wing-elf-ld  链接器
## binutils除汇编器及链接器外其他与二进制文件处理相关的工具：
## riscv32-wing-elf-nm
## riscv32-wing-elf-objcopy
## riscv32-wing-elf-objdump
## riscv32-wing-elf-readelf
## ...

## 2.1 WingGCC目录结构：
## .
## ├── bin
## │   ├── riscv32-wing-elf-addr2line
## │   ├── riscv32-wing-elf-ar
## │   ├── riscv32-wing-elf-as
## │   ├── riscv32-wing-elf-c++
## │   ├── riscv32-wing-elf-c++filt
## │   ├── riscv32-wing-elf-cpp
## │   ├── riscv32-wing-elf-elfedit
## │   ├── riscv32-wing-elf-g++
## │   ├── riscv32-wing-elf-gcc
## │   ├── riscv32-wing-elf-gcc-14.1.0
## │   ├── riscv32-wing-elf-gcc-ar
## │   ├── riscv32-wing-elf-gcc-nm
## │   ├── riscv32-wing-elf-gcc-ranlib
## │   ├── riscv32-wing-elf-gcov
## │   ├── riscv32-wing-elf-gcov-dump
## │   ├── riscv32-wing-elf-gcov-tool
## │   ├── riscv32-wing-elf-gdb
## │   ├── riscv32-wing-elf-gdb-add-index
## │   ├── riscv32-wing-elf-gprof
## │   ├── riscv32-wing-elf-ld
## │   ├── riscv32-wing-elf-ld.bfd
## │   ├── riscv32-wing-elf-lto-dump
## │   ├── riscv32-wing-elf-nm
## │   ├── riscv32-wing-elf-objcopy
## │   ├── riscv32-wing-elf-objdump
## │   ├── riscv32-wing-elf-ranlib
## │   ├── riscv32-wing-elf-readelf
## │   ├── riscv32-wing-elf-run
## │   ├── riscv32-wing-elf-size
## │   ├── riscv32-wing-elf-strings
## │   └── riscv32-wing-elf-strip
## ├── include
## │   ├── gdb
## │   └── sim
## ├── lib
## │   ├── bfd-plugins
## │   ├── gcc
## │   ├── libcc1.la
## │   ├── libcc1.so -> libcc1.so.0.0.0
## │   ├── libcc1.so.0 -> libcc1.so.0.0.0
## │   ├── libcc1.so.0.0.0
## │   └── libriscv32-wing-elf-sim.a
## ├── libexec
## │   └── gcc
## ├── riscv32-wing-elf
## │   ├── bin
## │   ├── include
## │   └── lib
## └── share
##     ├── gcc-14.1.0
##     ├── gdb
##     ├── info
##     ├── locale
##     └── man

## 2.2 RISCV架构相关选项
## -march 指定RISC-V的模块化指令集组合, 目前支持的指令集组合为
##     rv32imcv_zicsr_zicntr_zvw
## -mabi 指定指令集组合的ABI, 目前支持的ABI
##     ilp32

########################2.zvw intrinsic说明##############################
## 所有指令均已支持嵌入式汇编。
## 目前已实现的指令intrinsic均按照riscv官方intrinsic文档说明v-intrinsic-spec标准实现。
## 标准rvv intrinsic函数会用不同的后缀来声明其EEW和EMUL等属性。
## 编译器会在intrinsic前根据后缀插入一条vsetvl指令，用户不需要自己添加vsetvl指令。

## 3.0 intrinsic支持.vv和.vs指令
## 以vdsmul指令举例，vv后缀为.vv指令，_vs后缀为.vs指令，有
vint8mf8_t __riscv_vdsmul_vv_i8mf8(vint8mf8_t vs2, vint8mf8_t vs1, size_t vl);
vint8mf8_t __riscv_vdsmul_vs_i8mf8(vint8mf8_t vs2, vint8mf8_t vs1, size_t vl);
 
## 3.1 intrinsic支持不同的EEW和EMUL：
## 请参考riscv官方intrinsic文档说明v-intrinsic-spec 7.1节
vint8mf8_t __riscv_vdsmul_vs_i8mf8(vint8mf8_t vs2, vint8mf8_t vs1, size_t vl);
vint8mf4_t __riscv_vdsmul_vs_i8mf4(vint8mf4_t vs2, vint8mf4_t vs1, size_t vl);
vint8mf2_t __riscv_vdsmul_vs_i8mf2(vint8mf2_t vs2, vint8mf2_t vs1, size_t vl);
vint8m1_t __riscv_vdsmul_vs_i8m1(vint8m1_t vs2, vint8m1_t vs1, size_t vl);
vint8m2_t __riscv_vdsmul_vs_i8m2(vint8m2_t vs2, vint8m2_t vs1, size_t vl);
vint8m4_t __riscv_vdsmul_vs_i8m4(vint8m4_t vs2, vint8m4_t vs1, size_t vl);
vint8m8_t __riscv_vdsmul_vs_i8m8(vint8m8_t vs2, vint8m8_t vs1, size_t vl);
## 请注意没有vint16mf8
vint16mf4_t __riscv_vdsmul_vs_i16mf4(vint16mf4_t vs2, vint16mf4_t vs1, size_t vl);
vint16mf2_t __riscv_vdsmul_vs_i16mf2(vint16mf2_t vs2, vint16mf2_t vs1, size_t vl);
vint16m1_t __riscv_vdsmul_vs_i16m1(vint16m1_t vs2, vint16m1_t vs1, size_t vl);
vint16m2_t __riscv_vdsmul_vs_i16m2(vint16m2_t vs2, vint16m2_t vs1, size_t vl);
vint16m4_t __riscv_vdsmul_vs_i16m4(vint16m4_t vs2, vint16m4_t vs1, size_t vl);
vint16m8_t __riscv_vdsmul_vs_i16m8(vint16m8_t vs2, vint16m8_t vs1, size_t vl);
 
vint32mf2_t __riscv_vdsmul_vs_i32mf2(vint32mf2_t vs2, vint32mf2_t vs1, size_t vl);
vint32m1_t __riscv_vdsmul_vs_i32m1(vint32m1_t vs2, vint32m1_t vs1, size_t vl);
vint32m2_t __riscv_vdsmul_vs_i32m2(vint32m2_t vs2, vint32m2_t vs1, size_t vl);
vint32m4_t __riscv_vdsmul_vs_i32m4(vint32m4_t vs2, vint32m4_t vs1, size_t vl);
vint32m8_t __riscv_vdsmul_vs_i32m8(vint32m8_t vs2, vint32m8_t vs1, size_t vl);
 
vint64m1_t __riscv_vdsmul_vs_i64m1(vint64m1_t vs2, vint64m1_t vs1, size_t vl);
vint64m2_t __riscv_vdsmul_vs_i64m2(vint64m2_t vs2, vint64m2_t vs1, size_t vl);
vint64m4_t __riscv_vdsmul_vs_i64m4(vint64m4_t vs2, vint64m4_t vs1, size_t vl);
vint64m8_t __riscv_vdsmul_vs_i64m8(vint64m8_t vs2, vint64m8_t vs1, size_t vl);
 
## 3.2 intrinsic支持int和uint类型，如
vuint8mf8_t __riscv_vdsmul_vs_u8mf8(vuint8mf8_t vs2, vuint8mf8_t vs1, size_t vl);
vuint16mf4_t __riscv_vdsmul_vs_u16mf4(vuint16mf4_t vs2, vuint16mf4_t vs1, size_t vl);
vuint32mf2_t __riscv_vdsmul_vs_u32mf2(vuint32mf2_t vs2, vuint32mf2_t vs1, size_t vl);
vuint64m1_t __riscv_vdsmul_vs_u64m1(vuint64m1_t vs2, vuint64m1_t vs1, size_t vl);
 
## 3.3 intrinsic支持不同的predication，即不同的vm, vta和vma组合：
## 请参考riscv官方intrinsic文档说明v-intrinsic-spec 6.1节
## 以下以EEW=8和EMUL=1的vdsmul指令intrinsic为例，其他EEW和EMUL的组合省略。
## 无后缀，即vm=1, vta=1
vint8m1_t __riscv_vdsmul_vs_i8m1(vint8m1_t vs2, vint8m1_t vs1, size_t vl);
 
## tu后缀，即vm=1, vta=0
vint8m1_t __riscv_vdsmul_vv_i8m1_tu(vint8m1_t vd, vint8m1_t vs2, vint8m1_t vs1, size_t vl);
 
## m后缀，即vm=0, vta=1, vma=1
vint8m1_t __riscv_vdsmul_vv_i8m1_m(vbool8_t vm, vint8m1_t vs2, vint8m1_t vs1, size_t vl);
 
## tum后缀，即vm=0, vta=0, vma=1
## 当掩码有tu或者mu时，intrinsic会添加一个vd参数来描述指令需要在计算时使用vd寄存器
## 请参考riscv官方intrinsic文档说明v-intrinsic-spec 9.3节
vint8m1_t __riscv_vdsmul_vv_i8m1_tum(vbool8_t vm, vint8m1_t vd, vint8m1_t vs2, vint8m1_t vs1, size_t vl);
 
## mu后缀，即vm=0, vta=1, vma=0
vint8m1_t __riscv_vdsmul_vv_i8m1_mu(vbool8_t vm, vint8m1_t vd, vint8m1_t vs2, vint8m1_t vs1, size_t vl);
 
## tumu后缀，即vm=0, vta=0, vma=0
vint8m1_t __riscv_vdsmul_vv_i8m1_tumu(vbool8_t vm, vint8m1_t vd, vint8m1_t vs2, vint8m1_t vs1, size_t vl);
 
########################4.已实现指令intrinsic说明#########################
## 每条指令根据不同的后缀可能有上百个相关intrinsic函数
## 如无特殊说明，默认指令intrinsic支持所有predication，支持int和uint，支持不同的EEW和EMUL
## load指令以vlfcb2w.v为例子，支持所有predication，支持int和uint，支持不同的EEW和EMUL，即默认
vuint32m4_t __riscv_vlfcb2w_v_u32m4(const uint8_t *rs1, size_t vl);
vuint32mf2_t __riscv_vlfcb2w_v_u32mf2(const uint8_t *rs1, size_t vl);
vuint32mf2_t __riscv_vlfcb2w_v_u32mf2_m(vbool64_t vm, const uint8_t *rs1, size_t vl);
vuint32mf2_t __riscv_vlfcb2w_v_u32mf2_tu(vuint32mf2_t vd, const uint8_t *rs1, size_t vl);
vuint32mf2_t __riscv_vlfcb2w_v_u32mf2_tum(vbool64_t vm, vuint32mf2_t vd, const uint8_t *rs1, size_t vl)
vuint32mf2_t __riscv_vlfcb2w_v_u32mf2_mu(vbool64_t vm, vuint32mf2_t vd, const uint8_t *rs1, size_t vl)
vuint32mf2_t __riscv_vlfcb2w_v_u32mf2_tumu(vbool64_t vm, vuint32mf2_t vd, const uint8_t *rs1, size_t vl);
 
## store指令以vsfcw2b.v，store指令只有无后缀和m后缀，且无返回值
void __riscv_vsfcw2b_v_u32m4(uint8_t *rs1, vuint32m4_t vs3, size_t vl);
void __riscv_vsfcw2b_v_u32mf2(uint8_t *rs1, vuint32mf2_t vs3, size_t vl);
void __riscv_vsfcw2b_v_u32mf2_m(vbool64_t vm, uint8_t *rs1, vuint32mf2_t vs3, size_t vl);

## 对于同时有.i和.x的指令，intrinsic只实现_x后缀来表示其是标量
## 可以传入立即数或变量，当立即数超范围时，编译器会自动选择.x指令
## __riscv_vdsmacini_x_i8m1(10, vl); // 选择为vdsmacini.i 10
## __riscv_vdsmacini_x_i8m1(32, vl); // 选择为li a5, 32; vdsmacini.x a5

 
## vcpack 默认
## vdsmul 默认
## vdsmacini 无vd参数，只支持无后缀和m后缀
## vdsmac intrinsic函数添加一个返回值, 无vd参数，只支持无后缀和m后缀
## 即原本为void __riscv_vdsmac_i8m1(vint8m1_t vs2, vint8m1_t vs1, size_t vl); 
## 变为vint8m1_t __riscv_vdsmac_i8m1(vint8m1_t vs2, vint8m1_t vs1, size_t vl);
## vdsmaco 默认
## vlsb 默认
## vconj 默认
## vcunpackr 默认
## vcunpacki 默认
## vdscmul 默认
## vdscmulj 默认
## vdscredsum 默认
## vdscmac  intrinsic函数添加一个返回值, 无vd参数，只支持无后缀和m后缀
## vdscmacj intrinsic函数添加一个返回值, 无vd参数，只支持无后缀和m后缀
## vdscmacjo 默认
## vdscmaco 默认
## vdscmacor 默认
## vdscmacoi 默认
## vdscmacjor 默认
## vdscmacjoi 默认
## vdscmulr 默认
## vdscmuli 默认
## vdscmuljr 默认
## vdscmulji 默认
## vdsredsum 默认
## vdsredsumn 默认
## vlfcb2h 默认
## vlfcb2w 默认
## vlfch2w 默认
## vlfcpa2c 默认
## vlfcpb2c 默认
## vsfch2b store指令只持无后缀和m后缀
## vsfcw2b store指令只持无后缀和m后缀
## vsfcw2h store指令只持无后缀和m后缀
## vsfcc2pa store指令只持无后缀和m后缀
## vsfcc2pb store指令只持无后缀和m后缀
## vredmaxi 默认
## vredmini 默认
## vpharot 无vm位，只支持无后缀和tu后缀
## vperm  无vm位，只支持无后缀和tu后缀
## vfsl  无vm位，只支持无后缀和tu后缀
## vfsr  无vm位，只支持无后缀和tu后缀
## vlnlp 无vm位，无vd参数，只支持无后缀
## vnle 默认
## vnlm 默认

