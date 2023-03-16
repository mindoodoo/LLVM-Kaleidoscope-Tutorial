/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#pragma once

#include <memory>
#include <stdio.h>
#include "Ast.hpp"
#include "Lexer.hpp"

class AstParser {
public:
    // Utilities
    int _currentToken;
    int getNextToken();

    // Error handling
    std::unique_ptr<ExprAst> logError(const char *str);
    std::unique_ptr<PrototypeAst> logErrorP(const char *str);

    // Expression parsing
    std::unique_ptr<ExprAst> parseExpression();

    std::unique_ptr<NumberExprAst> parseNumberExprAst();

    // parenexpr ::= '(' expression ')'
    std::unique_ptr<ExprAst> parseParenExpr();

private:
    Lexer _lexer;
};