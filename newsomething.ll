; ModuleID = 'something.ll'
source_filename = "something.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

@0 = private unnamed_addr constant [37 x i8] c"Before :: Value read from CSR: %lld\0A\00", align 1
@1 = private unnamed_addr constant [36 x i8] c"After :: Value read from CSR: %lld\0A\00", align 1
@2 = private unnamed_addr constant [33 x i8] c"Time taken in clock ticks: %lld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @hello() #0 {
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %csr_value = alloca i64, align 8
  %1 = call i64 asm sideeffect "csrr $0, time", "=r"()
  store i64 %1, ptr %csr_value, align 8
  %2 = call i32 (ptr, ...) @printf(ptr @0, i64 %1)
  %3 = alloca i32, align 4
  store i32 0, ptr %3, align 4
  %4 = call i64 asm sideeffect "csrr $0, time", "=r"()
  %5 = call i32 (ptr, ...) @printf(ptr @1, i64 %4)
  %6 = sub i64 %4, %1
  %7 = call i32 (ptr, ...) @printf(ptr @2, i64 %6)
  ret i32 0
}

declare i32 @printf(ptr, ...)

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
