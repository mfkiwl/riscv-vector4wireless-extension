# Directory Structure
```
.
├── demo
│   └── ccm.c：Conjugate Complex Multiply comparison operator.
│              op_testzvwCcm() is implemented by the zvw instruction;
│              op_testrvvCCM() is implemented by the rvv instruction
│   ├── cdsm.c：Complex Dynamic Scaling MAC Comparison Operator.
│   │          op_testzvwCdsm() is implemented by the zvw instruction; 
│   │          op_testrvvCdsm()is implemented by the rvv instruction
│   ├── wap.c: Wideband Average Power comparison operator.
│   │          op_testzvwWap() is implemented by the zvw instruction; 
│   │          op_testrvvWap()is implemented by the rvv instruction
│   ├── main.c: Compare the running results and output them
```

# Compile：
To build the demo run:
```shell
make 
```
# Execute
```shell
spike --isa=rv32imcv_zicsr_zicntr_zvw -l --log-commits --varch=vlen:1024,elen:32 --log="test_trace.log" ./demo.riscv
```



