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
    struct HPCenablePass: public FunctionPass{
        static char ID;

        HPCenablePass() : FunctionPass(ID){}

        virtual bool runOnModule(Module &M){
            
        }

        virtual bool runOnFunction(Function &F){
            // errs() << "I saw a function called " << F.getName() << "\n";

            // auto FirstBB = F.begin();
            // IRBuilder<> builder(&(*FirstBB));
            // FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getPtrTy()}, true);
            // Function *printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", F.getParent());

            if(F.getName() == "main"){

                auto FirstBB = F.begin();
                IRBuilder<> builder(&(*FirstBB));
                FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getPtrTy()}, true);
                Function *printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", F.getParent());

                for(auto &BB: F){
                    // for(auto &Inst: BB){
                    for (auto Inst = BB.begin(), EndInst = BB.end(); Inst != EndInst; ) {
                        Instruction &I = *Inst++; 
                        // for (Value *Operand : Inst.operands()) {
                        //     std::string OperandName = Operand->getName().str();
                        //     // Process the operand name
                        //     // errs() << "  Used SSA Variable: " << OperandName << "\n";
                        //     Operand->setName("HACKED");
                        // }

                        if (auto *allocaInst = dyn_cast<AllocaInst>(&I)) {
                            Type *allocatedType = allocaInst->getAllocatedType();
                                // allocaInst->getOperand(0)->setName("ALSOHACKED");

                            if (allocatedType->isArrayTy()) {
                                unsigned arraySize = cast<ArrayType>(allocatedType)->getNumElements();

                                // builder.SetInsertPoint(BB.getFirstInsertionPt());
                                builder.SetInsertPoint(&I);
                                Value *newAlloca = builder.CreateAlloca(ArrayType::get(Type::getInt32Ty(BB.getContext()), arraySize), nullptr, "RAILALLOCA");

                                // Value *operand = allocaInst->getOperand(0);
                                allocaInst->replaceAllUsesWith(newAlloca);
                                // Inst.removeFromParent();
                                // outs() << allocaInst->getNumOperands()  << " " << "AAAAAAAAAAA " << operand->replaceAllUsesWith() << "\n";

                                // builder.SetInsertPoint(allocaInst->getParent(), ++(allocaInst->getIterator()));

                                Value *str3 = builder.CreateGlobalStringPtr("Executing ZENO_PROTECT on Array with size: %d\n");
                                // Value* a = builder.CreateAlloca(Type::getInt32Ty(BB.getContext()), nullptr, "VAL");
                                // builder.CreateStore(builder.getInt32(arraySize), a);
                                // Value* c = builder.CreateLoad(Type::getInt32Ty(BB.getContext()), a, "");

                                builder.CreateCall(printfFunc, {str3, builder.getInt32(arraySize)});

                                allocaInst->removeFromParent();
                            }
                        }
                    }
                }


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


char HPCenablePass::ID = 0;

int main(){
    InitializeNativeTarget();
    InitializeNativeTargetAsmParser();
    InitializeNativeTargetAsmPrinter();

    LLVMContext context;

    std::unique_ptr<Module> Mod = RAIL::loadModule("stack_prog.ll", context, "File");

    legacy::PassManager PM;
    PM.add(new HPCenablePass());
    PM.run(*Mod);

    std::error_code EC;
    llvm::raw_fd_ostream outputFile("new_stack_prog.ll", EC, llvm::sys::fs::OpenFlags::OF_None);

    Module *newMod = Mod.release();
    newMod->print(outputFile, nullptr);

    return 0;
}