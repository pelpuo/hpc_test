#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Type.h" 

using namespace llvm;

namespace{
    struct SkeletonPass: public FunctionPass{
        static char ID;

        SkeletonPass() : FunctionPass(ID){}

        virtual bool runOnFunction(Function &F){
            errs() << "I saw a function called " << F.getName() << "\n";
            if(F.getName() == "main"){

                auto FirstBB = F.begin();
                auto LastBB = std::prev(F.end());
                // errs() << "I am in main" << "\n";
                IRBuilder<> builder(&(*FirstBB), FirstBB->getFirstInsertionPt());
                // Value *NewInst = builder.CreateMul(builder.getInt32(31), builder.getInt32(53));
                // builder.CreateRet(ConstantInt::get(Type::getInt32Ty(F.getContext()), 42));



                FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getPtrTy()}, true);
                Function *printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", F.getParent());

                // ASSEMBLY

                // Create a vector of operands
                // Create the inline assembly instruction
                // InlineAsm *loadHPCVal = InlineAsm::get(
                //     FunctionType::get(Type::getVoidTy(F.getContext()), false),
                //     "li a5, 420",
                //     "",
                //     true // Can throw
                // );
                // builder.CreateCall(loadHPCVal, {});

                // InlineAsm *startHPC = InlineAsm::get(
                //     FunctionType::get(Type::getVoidTy(F.getContext()), false),
                //     "csrr a5, time",
                //     "",
                //     true // Can throw
                // );
                // builder.CreateCall(startHPC, {});

                // Allocate memory for the value read from CSR
                AllocaInst *csrValuePtr = builder.CreateAlloca(builder.getInt64Ty(), nullptr, "csr_value");

                // Inline assembly to read value from CSR directly into the variable
                InlineAsm *readCSR = InlineAsm::get(
                                FunctionType::get(Type::getInt64Ty(F.getContext()), false),
                                "csrr $0, time",
                                "=r",
                                true // Can throw
                            );
                Value* csrValueBefore = builder.CreateCall(readCSR, {});

                builder.CreateStore(csrValueBefore, csrValuePtr);

                    // Load the value read from CSR
                // Value *val = builder.CreateLoad(builder.getInt64Ty(), csrValueBefore);

                // Call printf
                Value *str = builder.CreateGlobalStringPtr("Before :: Value read from CSR: %lld\n");
                builder.CreateCall(printfFunc, {str, csrValueBefore});


                // Create the inline assembly
                // FunctionCallee InlineAsmPrototype = F.getParent()->getOrInsertFunction(
                //     "llvm.riscv.csrr",
                //     Type::getInt32Ty(F.getContext()),
                //     Type::getInt32Ty(F.getContext())
                // );

                // // Create the inline assembly function call
                // Value *AsmCall = builder.CreateCall(
                //     InlineAsmPrototype,
                //     builder.getInt32(0x300), // mstatus CSR number
                //     "csrr"
                // );

                // // Store the result in register a0
                // Value* reg = builder.CreateAlloca(Type::getInt32Ty(F.getContext()), nullptr, "a0");
                // builder.CreateStore(AsmCall, reg);
                // ASSEMBLY

                // Iterate over instructions in the last basic block
                Instruction* returnInst = LastBB->getTerminator();
                builder.SetInsertPoint(returnInst);


                Value* csrValueAfter = builder.CreateCall(readCSR, {});

                // Load the value read from CSR
                // val = builder.CreateLoad(builder.getInt64Ty(), csrValueAfter);

                // Call printf
                Value *str2 = builder.CreateGlobalStringPtr("After :: Value read from CSR: %lld\n");
                builder.CreateCall(printfFunc, {str2, csrValueAfter});

                Value *str3 = builder.CreateGlobalStringPtr("Time taken in clock ticks: %lld\n");
                Value *timeTaken = builder.CreateSub(csrValueAfter, csrValueBefore);
                builder.CreateCall(printfFunc, {str3, timeTaken});

                // builder.CreateRet(ConstantInt::get(Type::getInt32Ty(F.getContext()), 42));

            }
            return true;
        }
    };

}

namespace RAIL{

    std::unique_ptr<Module> loadModule(StringRef Filename, LLVMContext &Ctx, const char *ProgName) {
        SMDiagnostic Err;
        std::unique_ptr<Module> Mod = parseIRFile(Filename, Err, Ctx);
        if (!Mod.get()) {
            Err.print(ProgName, errs());
            exit(-1);
        }
        return std::move(Mod);
    } //loadModule

} //namespace RAIL


char SkeletonPass::ID = 0;

int main(){
    InitializeNativeTarget();
    InitializeNativeTargetAsmParser();
    InitializeNativeTargetAsmPrinter();

    LLVMContext context;

    std::unique_ptr<Module> Mod = RAIL::loadModule("something.ll", context, "File");

    legacy::PassManager PM;
    PM.add(new SkeletonPass());
    PM.run(*Mod);

    // outs() << "\n----------------------------------------------------------------\n";
    // errs() << *Mod->getFunction("main");
    // outs() << "\n----------------------------------------------------------------\n";

    std::error_code EC;
    llvm::raw_fd_ostream outputFile("newsomething.ll", EC, llvm::sys::fs::OpenFlags::OF_None);

    Module *newMod = Mod.release();
    newMod->print(outputFile, nullptr);

    return 0;
}