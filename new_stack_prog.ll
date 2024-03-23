; ModuleID = 'stack_prog.ll'
source_filename = "stack_prog.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

@0 = private unnamed_addr constant [47 x i8] c"Executing ZENO_PROTECT on Array with size: %d\0A\00", align 1
@1 = private unnamed_addr constant [47 x i8] c"Executing ZENO_PROTECT on Array with size: %d\0A\00", align 1
@2 = private unnamed_addr constant [47 x i8] c"Executing ZENO_PROTECT on Array with size: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local signext i32 @x(i32 noundef signext %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  %5 = add nsw i32 %4, 2
  store i32 %5, ptr %3, align 4
  %6 = load i32, ptr %3, align 4
  ret i32 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local signext i32 @y(i32 noundef signext %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  %5 = add nsw i32 %4, 2
  store i32 %5, ptr %3, align 4
  %6 = load i32, ptr %3, align 4
  ret i32 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local signext i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %RAILALLOCA = alloca [5 x i32], align 4
  %4 = call i32 (ptr, ...) @printf(ptr @0, i32 5)
  %RAILALLOCA1 = alloca [10 x i32], align 4
  %5 = call i32 (ptr, ...) @printf(ptr @1, i32 10)
  %RAILALLOCA2 = alloca [15 x i32], align 4
  %6 = call i32 (ptr, ...) @printf(ptr @2, i32 15)
  store i32 2, ptr %1, align 4
  store i32 5, ptr %2, align 4
  %7 = load i32, ptr %1, align 4
  %8 = load i32, ptr %2, align 4
  %9 = add nsw i32 %7, %8
  store i32 %9, ptr %3, align 4
  call void @llvm.memset.p0.i64(ptr align 4 %RAILALLOCA, i8 0, i64 20, i1 false)
  %10 = getelementptr inbounds [5 x i32], ptr %RAILALLOCA, i64 0, i64 0
  store i32 10, ptr %10, align 4
  %11 = getelementptr inbounds [5 x i32], ptr %RAILALLOCA, i64 0, i64 1
  store i32 20, ptr %11, align 4
  %12 = getelementptr inbounds [5 x i32], ptr %RAILALLOCA, i64 0, i64 2
  store i32 30, ptr %12, align 4
  %13 = getelementptr inbounds [5 x i32], ptr %RAILALLOCA, i64 0, i64 3
  store i32 40, ptr %13, align 4
  %14 = getelementptr inbounds [5 x i32], ptr %RAILALLOCA, i64 0, i64 4
  store i32 50, ptr %14, align 4
  call void @llvm.memset.p0.i64(ptr align 4 %RAILALLOCA1, i8 0, i64 40, i1 false)
  call void @llvm.memset.p0.i64(ptr align 4 %RAILALLOCA2, i8 0, i64 60, i1 false)
  ret i32 0
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

declare i32 @printf(ptr, ...)

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,+zicsr,-e,-experimental-zaamo,-experimental-zabha,-experimental-zacas,-experimental-zalasr,-experimental-zalrsc,-experimental-zcmop,-experimental-zfbfmin,-experimental-zicfilp,-experimental-zicfiss,-experimental-zimop,-experimental-ztso,-experimental-zvfbfmin,-experimental-zvfbfwma,-h,-smaia,-smepmp,-ssaia,-svinval,-svnapot,-svpbmt,-v,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-za128rs,-za64rs,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmp,-zcmt,-zdinx,-zfa,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zifencei,-zihintntl,-zihintpause,-zihpm,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-zmmul,-zvbb,-zvbc,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: write) }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64d"}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"PIE Level", i32 2}
!4 = !{i32 7, !"uwtable", i32 2}
!5 = !{i32 7, !"frame-pointer", i32 2}
!6 = !{i32 8, !"SmallDataLimit", i32 8}
!7 = !{!"clang version 19.0.0git (https://github.com/llvm/llvm-project.git e296cedcd686e24fee75756185669f1bb3b47fdd)"}
