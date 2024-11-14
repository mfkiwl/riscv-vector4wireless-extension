.
├── demo
│   └── ccm.c：Conjugate Complex Multiply comparison operator; Op_testzvwCcm function is implemented by the zvw instruction; Op_testrvvCCM function is implemented by the rvv instruction
│   ├── cdsm.c：Complex Dynamic Scaling MAC Comparison Operator; Op_testzvwCdsm function is implemented by the zvw instruction; Op_testrvvCdsm function is implemented by the rvv instruction
│   ├── wap.c: Wideband Average Power comparison operator; Op_testzvwWap function is implemented by the zvw instruction; Op_testrvvWap function is implemented by the rvv instruction
│   ├── main.c: Compare the running results and output them;

Compile：run “make” in the current directory
Execute：spike --isa=rv32imcv_zicsr_zicntr_zvw -l --log-commits --varch=vlen:1024,elen:32 --log="test_trace.log" ./demo.riscv



