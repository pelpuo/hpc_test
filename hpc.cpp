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

    outs() << "\n----------------------------------------------------------------\n";
    errs() << *Mod->getFunction("main");
    outs() << "\n----------------------------------------------------------------\n";
    // Module *newMod = Mod.release();
    // newMod->print(outs(), nullptr);

    return 0;
}