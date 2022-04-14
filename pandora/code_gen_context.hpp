#ifndef PANDORA_CODE_GEN_CONTEXT_HPP_
#define PANDORA_CODE_GEN_CONTEXT_HPP_

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <unordered_map>

namespace pandora::ast {

class code_gen_context {
public:
  code_gen_context();

  auto &llvm_context() { return *llvm_context_; }
  auto const &llvm_context() const { return *llvm_context_; }

  auto &llvm_module() { return *llvm_module_; }
  auto const &llvm_module() const { return *llvm_module_; }

  auto &llvm_builder() { return *llvm_builder_; }
  auto const &llvm_builder() const { return llvm_builder_; }

  auto &named_values() { return *named_values_; }
  auto const &named_values() const { return *named_values_; }

private:
  std::unique_ptr<llvm::LLVMContext> llvm_context_;
  std::unique_ptr<llvm::Module> llvm_module_;
  std::unique_ptr<llvm::IRBuilder<>> llvm_builder_;
  std::unordered_map<std::string, llvm::Value *> named_values_;
};

}; // namespace pandora::ast

#endif
