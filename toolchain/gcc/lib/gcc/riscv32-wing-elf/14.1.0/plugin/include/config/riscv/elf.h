/* Target macros for riscv*-elf targets.
   Copyright (C) 1994-2024 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define LINK_SPEC "\
-melf" XLEN_SPEC DEFAULT_ENDIAN_SPEC "riscv \
%{mno-relax:--no-relax} \
%{mbig-endian:-EB} \
%{mlittle-endian:-EL} \
%{shared}"

/* Link against Newlib libraries, because the ELF backend assumes Newlib.
   Handle the circular dependence between libc and libgloss. */
/* wingsemi specific - modify the checking rules of multi_lib_check() - 202408 */
#undef  LIB_SPEC
#define LIB_SPEC \
  "--start-group -lc%{mbig-endian:_be} %{!specs=nosys.specs:-lgloss%{mbig-endian:_be}} --end-group " \
  "%{!nostdlib:%{!nodefaultlibs|!nostartfiles:%:riscv_multi_lib_check()}}"

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crt0%{mbig-endian:_be}%O%s crtbegin%{mbig-endian:_be}%O%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend%{mbig-endian:_be}%O%s"

/* wingsemi specific - support big-endian - 202407 */
#undef  LIBGCC_SPEC
#define LIBGCC_SPEC "-lgcc_nano%{mbig-endian:_be}"

#define RISCV_USE_CUSTOMISED_MULTI_LIB select_by_abi_arch_cmodel
