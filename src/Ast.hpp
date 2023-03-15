/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#pragma once

#include <string>
#include <memory>
#include <vector>

class ExprAst {
public:
    virtual ~ExprAst() = default;
};

class NumberExprAst: public ExprAst {
public:
    NumberExprAst(double val);

private:
    double _val;
};

class VariableExprAst : public ExprAst {

public:
    VariableExprAst(const std::string &_name);

private:
    std::string _name;
};

class BinaryExprAst : public ExprAst {
public:
    BinaryExprAst(char op, std::unique_ptr<ExprAst> lhs, std::unique_ptr<ExprAst> rhs);

private:
    char _op;
    std::unique_ptr<ExprAst> _lhs, _rhs;
};

// ─── Functions ───────────────────────────────────────────────────────────────────────────────────

class CallExprAst : public ExprAst {
public:
    CallExprAst(const std::string &callee, std::vector<std::unique_ptr<ExprAst>> args);

private:
    std::string _callee;
    std::vector<std::unique_ptr<ExprAst>> _args;
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAst {
public:
    PrototypeAst(const std::string &name, std::vector<std::string> args);

    const std::string &getName() const;
private:
    std::string _name;
    std::vector<std::string> _args;
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAst {
public:
    FunctionAst(std::unique_ptr<PrototypeAst> proto, std::unique_ptr<ExprAst> body);

private:
    std::unique_ptr<PrototypeAst> _proto;
    std::unique_ptr<ExprAst> _body;
};