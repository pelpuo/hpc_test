# hpc_test

#### Generate LLVM IR for C code

```
clang -S -emit-llvm -target riscv64-unknown-linux-gnu stack_prog.c -o stack_prog.ll
```



#### Compile for RISC-V

```
clang -static -target riscv64-unknown-linux-gnu new_stack_prog.ll -o new_stack_prog
```



#### Execute RISC-V program

```
qemu-riscv64 new_stack_prog
```

#### 