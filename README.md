# riscv-vector4wireless-extension

## Introduction

Currently, RVV1.0 vector extensions are mostly applied in AI and image processing fields, but these basic vector instructions cannot fully meet the needs of wireless broadband signal processing scenarios, such as complex number processing requirements in 4G/5G communications and the need for high-precision fixed-point dynamic scaling. Therefore, it is necessary to add more effective vector extension instructions for the wireless communication field, especially in 4G,5G, wifi and some other wireless areas, which account for a considerable portion of vector signal processing demands. Vector signal processing extension instruction sets based on RVV extension can not only promote rapid development in 4G and 5G signal processing fields but also promote RISC-V applications in wireless signal processing.

This instruction set, as a supplementary instruction set to RISC-V RVV 1.0 version, mainly targets for wideband wireless communication signal processing needs. This vector wireless broadband extension instruction is denoted by 'w' (Wireless), and it's based on the basic v extension, thus can be defined as 'Zvw'. 

The following instruction types were added for wireless applications:
* Complex fixed-point arithmetic instructions
* Complex compression format conversion instructions
* Special load and store instructions
* Unified Fast non-linear operation instructions
* Efficient inter-element operation instructions
* Dynamic scaling multiply-accumulate operation instructions

The "operation" part of instructions in this document refers to the operation of each element in the vector processor. The loop count follows the standard RVV specifications of LMUL and VL; operands are controlled by SEW; whether each element is operated is controlled by vm; the default values of non-operated elements are controlled by mask agnostic and tail agnostic as defined by RVV specification.

## License

This work is licensed under a Creative Commons Attribution 4.0 International License (CC-BY-4.0). For details, see the link:LICENSE[LICENSE] file.

## Maintainers

The list of maintainers of this specification is maintained in the link:MAINTAINERS.md[MAINTAINERS] file.

## Contributors

The list of contributors to this specification is maintained in the link:src/contributors.adoc[contributors] file.

For guidelines on how to contribute, refer to the link:CONTRIBUTING.md[CONTRIBUTING] file.

## Governance

The governance for this project is defined in the link:GOVERNANCE.md[GOVERNANCE] file.

Community information, including meeting (if held) and mailing lists are detailed in this file.

## Building the Document

### Directory Structure

The following directories are used to organize the contents of this repo:

* `dependencies/`: software dependencies needed to build the specification
* `docs-resources/`: resources for all specifications sourced from link:.gitmodules[git submodule]
* `src/`: source files for the specification
* `build/`: default directory where the build artifacts are generated
* `demo/`: Example of complex operations for zvw instruction and rvv1.0 instruction

### Prerequisites

To build the document, you'll need the following tools installed on your system:

* Make
* asciiDoctor-pdf, asciidoctor-bibtex, asciidoctor-diagram, and asciidoctor-mathematical
* Docker

### Cloning the Repository

```shell
git clone --recurse-submodules https://github.com/gaoshanlee193/riscv-vector4wireless-extension.git
```

All in one single line:

```shell
git clone --recurse-submodules https://github.com/gaoshanlee193/riscv-vector4wireless-extension.git && cd riscv-vector4wireless-extension && git submodule update --init --recursive

```

### Building the Documentation

To start the build process, run:

```shell
cd ./riscv-vector4wireless-extension && make build
```

The link:Makefile[] script will check the availability of Docker on your system:

* If Docker is available, the documentation will be built inside a Docker container using the image riscvintl/riscv-docs-base-container-image:latest. This ensures a consistent build environment across different systems.
* If Docker is not available, the documentation will be built directly on your system using the installed tools.

The documentation is generated from the AsciiDoctor source files in your project. The primary source file is specified by the `HEADER_SOURCE` variable in the Makefile.

The build process utilizes several options, including theming and font settings, and generates a PDF document as output.

### Cleaning up

To clean up the generated files, run:

```shell
make clean
```

## Enabling pre-commit checks locally

The repository has some basic commit checks set up with https://pre-commit.com/ that will be enforced by the GitHub CI.
To ensure these checks are also run in the local repository while making changes the following can be done:

* Installing pre-commit tool
```shell
pip3 install pre-commit
```

* Installing pre-commit git hook in repo
```shell
pre-commit install
```

Rather than doing the above `pre-commit install` in every repo that uses it, you can do it https://pre-commit.com/#automatically-enabling-pre-commit-on-repositories

When enabling additional checks https://pre-commit.com/#plugins, it is recommended running the newly added check on all files in the repository. This can be done with the following command:

* Running all pre-commit hooks on all files
```shell
pre-commit run --all-files
```
