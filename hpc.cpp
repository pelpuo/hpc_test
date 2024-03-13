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
                builder.CreateRet(ConstantInt::get(Type::getInt32Ty(F.getContext()), 42));


                // ASSEMBLY
                // Define the assembly instruction as a string
                StringRef asmInstruction = "$0 = add $1, $2";

                // Define the constraints for the operands
                StringRef constraints = "=r,r,r";

                // Define the operands (registers or values)
                Value *op1 = builder.getInt32(10);
                Value *op2 = builder.getInt32(20);

                // Create a vector of operands
                std::vector<Value *> operands = {nullptr, op1, op2};

                // Create the inline assembly instruction
                InlineAsm *inlineAsm = InlineAsm::get(
                    FunctionType::get(op1->getType(), {op1->getType(), op2->getType()}, false),
                    asmInstruction,
                    constraints,
                    true, // Can throw
                    false, // Can't side-effect
                    InlineAsm::AD_Intel
                );
                Value *result = builder.CreateCall(inlineAsm, operands, "asm_result");
                // ASSEMBLY

                // Iterate over instructions in the last basic block
                Instruction* returnInst = LastBB->getTerminator();
                builder.SetInsertPoint(returnInst);
                builder.CreateRet(ConstantInt::get(Type::getInt32Ty(F.getContext()), 42));

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