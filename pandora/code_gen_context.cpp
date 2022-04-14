#include <memory>
#include <pandora/code_gen_context.hpp>

namespace pandora::ast {

code_gen_context::code_gen_context() {
  llvm_context_ = std::make_unique<llvm::LLVMContext>();
  llvm_module_ = std::make_unique<llvm::Module>("pandora", *llvm_context_);
  llvm_builder_ = std::make_unique<llvm::IRBuilder<>>(*llvm_context_);
}

} // namespace pandora::ast