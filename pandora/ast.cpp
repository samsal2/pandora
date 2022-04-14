#include <pandora/ast.hpp>
#include <pandora/code_gen_context.hpp>

namespace pandora::ast {

llvm::Value *node::code_gen([[maybe_unused]] code_gen_context &context) {
  throw std::runtime_error("ayo??? make sure this isn't just an empty node");
}

llvm::Value *integer::code_gen(code_gen_context &context) {
  return llvm::ConstantFP::get(context.llvm_context(),
                               llvm::APFloat((float)value_));
}

llvm::Value *number::code_gen(code_gen_context &context) {
  return llvm::ConstantFP::get(context.llvm_context(), llvm::APFloat(value_));
}

llvm::Value *identifier::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *call::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *
binary_operator::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *assignment::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *block::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *
expression_statement::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *
return_statement::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *
variable_declaration::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *
extern_declaration::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

llvm::Value *
function_declaration::code_gen([[maybe_unused]] code_gen_context &context) {
  return nullptr;
}

}; // namespace pandora::ast