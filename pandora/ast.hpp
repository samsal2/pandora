#ifndef PANDORA_AST_HPP_
#define PANDORA_AST_HPP_

#include <llvm/IR/Value.h>
#include <span>
#include <string>
#include <vector>

namespace pandora::ast {

class code_gen_context;

class node {
public:
  node() = default;

  node(node const &other) = default;
  node(node &&other) = default;

  node &operator=(node const &other) = default;
  node &operator=(node &&other) = default;

  virtual llvm::Value *code_gen(code_gen_context &context);

  virtual ~node() = default;
};

class expression : public node {};

class statement : public node {};

class integer : public expression {
public:
  using type = std::uint64_t;

  explicit integer(type value) noexcept : value_(value) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  type value_;
};

class number : public expression {
public:
  using type = double;

  explicit number(type value) noexcept : value_(value) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  type value_;
};

class identifier : public expression {
public:
  using type = std::string;

  explicit identifier(type value) noexcept : value_(std::move(value)) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  type value_;
};

class call : public expression {
public:
  call(identifier *identifier, std::vector<expression *> arguments) noexcept
      : identifier_(identifier), arguments_(std::move(arguments)) {}

  explicit call(identifier *identifier) noexcept : identifier_(identifier) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  identifier *identifier_ = nullptr;
  std::vector<expression *> arguments_ = {};
};

class binary_operator : public expression {
public:
  using type = std::uint32_t;

  binary_operator(type op, expression *lhs, expression *rhs) noexcept
      : op_(op), lhs_(lhs), rhs_(rhs) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  type op_;
  expression *lhs_ = nullptr;
  expression *rhs_ = nullptr;
};

class assignment : public expression {
public:
  assignment(expression *lhs, expression *rhs) noexcept
      : lhs_(lhs), rhs_(rhs) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  expression *lhs_ = nullptr;
  expression *rhs_ = nullptr;
};

class block : public expression {
public:
  block() = default;

  virtual llvm::Value *code_gen(code_gen_context &context);

  void add_statement(statement *statement) { statements_.push_back(statement); }

  auto view() noexcept { return std::span(statements_); }
  auto view() const noexcept { return std::span(statements_); }

private:
  std::vector<statement *> statements_;
};

class expression_statement : public statement {
public:
  explicit expression_statement(expression *expression) noexcept
      : expression_(expression) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  expression *expression_;
};

class return_statement : public statement {
public:
  explicit return_statement(expression *expression) noexcept
      : expression_(expression) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  expression *expression_;
};

class variable_declaration : public statement {
public:
  variable_declaration(identifier *type, identifier *id) noexcept
      : type_(type), id_(id) {}

  variable_declaration(identifier *type, identifier *id,
                       expression *assignment) noexcept
      : type_(type), id_(id), assignment_(assignment) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  identifier *type_ = nullptr;
  identifier *id_ = nullptr;
  expression *assignment_ = nullptr;
};

class extern_declaration : public statement {
public:
  extern_declaration(identifier *type, identifier *id,
                     std::vector<variable_declaration *> variables) noexcept
      : type_(type), id_(id), variables_(std::move(variables)) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  identifier *type_;
  identifier *id_;
  std::vector<variable_declaration *> variables_;
};

class function_declaration : public statement {
public:
  function_declaration(identifier *type, identifier *id, block *block,
                       std::vector<variable_declaration *> variables) noexcept
      : type_(type), id_(id), block_(block), variables_(variables) {}

  virtual llvm::Value *code_gen(code_gen_context &context);

private:
  identifier *type_;
  identifier *id_;
  block *block_;
  std::vector<variable_declaration *> variables_;
};

} // namespace pandora::ast

#endif
