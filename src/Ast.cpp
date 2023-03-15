/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#include "Ast.hpp"

// ─── Constructors ────────────────────────────────────────────────────────────────────────────────

NumberExprAst::NumberExprAst(double val) : _val(val) {}

VariableExprAst::VariableExprAst(const std::string &name) : _name(name) {}

BinaryExprAst::BinaryExprAst(char op, std::unique_ptr<ExprAst> lhs, std::unique_ptr<ExprAst> rhs)
    : _op(op), _lhs(std::move(lhs)), _rhs(std::move(rhs)) {}

CallExprAst::CallExprAst(const std::string &callee, std::vector<std::unique_ptr<ExprAst>> args)
    : _callee(callee), _args(std::move(args)) {}

PrototypeAst::PrototypeAst(const std::string &name, std::vector<std::string> args)
    : _name(name), _args(std::move(args)) {}

FunctionAst::FunctionAst(std::unique_ptr<PrototypeAst> proto, std::unique_ptr<ExprAst> body)
    : _proto(std::move(proto)), _body(std::move(body)) {}

// ─── Getters And Setters ─────────────────────────────────────────────────────────────────────────

const std::string &PrototypeAst::getName() const {
    return _name;
}