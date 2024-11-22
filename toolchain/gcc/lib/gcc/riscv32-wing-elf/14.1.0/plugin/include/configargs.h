/* Generated automatically. */
static const char configuration_arguments[] = "/toolchains/build/../gcc/configure --target=riscv32-wing-elf --prefix=/releases/gcc-ubuntu --disable-shared --disable-threads --enable-languages=c,c++ --with-pkgversion=14.1.0 --with-system-zlib --enable-tls --with-newlib --with-sysroot=/releases/gcc-ubuntu/riscv32-wing-elf --with-native-system-header-dir=/include --disable-libmudflap --disable-libssp --disable-libquadmath --disable-libgomp --disable-nls --disable-tm-clone-registry --src=../../gcc --disable-multilib --with-abi=ilp32 --with-arch=rv32imc_zve32x_zicsr_zicntr_zvw --with-tune=wing-m130 --with-isa-spec=20191213 'CFLAGS_FOR_TARGET=-Os    -mcmodel=medlow ' 'CXXFLAGS_FOR_TARGET=-O0    -mcmodel=medlow '";
static const char thread_model[] = "single";

static const struct {
  const char *name, *value;
} configure_default_options[] = { { "abi", "ilp32" }, { "arch", "rv32imc_zicntr_zicsr_zve32x_zvl32b_zvw" }, { "tune", "wing-m130" }, { "isa_spec", "20191213" } };
