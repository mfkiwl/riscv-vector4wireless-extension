/*======================================================================*/
/* Proxy kernel linker script                                           */
/*======================================================================*/
/* This is the linker script used when building the proxy kernel. */

/*----------------------------------------------------------------------*/
/* Setup                                                                */
/*----------------------------------------------------------------------*/

/* The OUTPUT_ARCH command specifies the machine architecture where the
   argument is one of the names used in the BFD library. More
   specifically one of the entires in bfd/cpu-mips.c */

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY {
  ITCM  : ORIGIN = 0x01000000,  LENGTH = 64K      /* text segment start address, don't delete this comment */
  DTCM  : ORIGIN = 0x20000000,  LENGTH = 64K      /* data segment start address, don't delete this comment */
  SRAM  : ORIGIN = 0x21000000,  LENGTH = 64K
  FLASH : ORIGIN = 0x08000000,  LENGTH = 64K
}

/*----------------------------------------------------------------------*/
/* Sections                                                             */
/*----------------------------------------------------------------------*/
STACK_SIZE = 4096;
HEAP_SIZE = 1024;

SECTIONS
{
  .text.interrupt : {
    *(.text.interrupt)
  } >ITCM

  /* text: test code section */
  .text.init : {
    *(.text.init)
  } >ITCM

  .text : {
     *(.text*) 
  } >ITCM

  /* data segment */
  .rodata : { 
    *(.rodata*) 
  } >DTCM
  
  .data : { 
    *(.data*) 
  } >DTCM

  .sdata : {
    __global_pointer$ = . + 0x700;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }>DTCM

  /* bss segment */
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }>DTCM

  .bss : { 
    *(.bss.*) 
  }>DTCM

  /* End of uninitalized data segement */
  _end = .;

  .heap :{
    . = ALIGN(4);
    __HEAP_START = .;
    . += HEAP_SIZE;
    __HEAP_MAX = .;
  } >DTCM

  .stack ORIGIN(DTCM) + LENGTH(DTCM) - STACK_SIZE -0x100 :{
    PROVIDE( __HEAP_END = . );
    FILL(0);
    . += STACK_SIZE;
    PROVIDE( __STACK_END__ = . );
  } >DTCM
}