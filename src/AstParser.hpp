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
    static int CurrentToken;
    static int getNextToken();

    // Error handling
    static std::unique_ptr<ExprAst> logError(const char *str);
    static std::unique_ptr<PrototypeAst> logErrorP(const char *str);

    // Expression parsing
    static std::unique_ptr<ExprAst> parseExpression();

    static std::unique_ptr<NumberExprAst> parseNumberExprAst();

    // parenexpr ::= '(' expression ')'
    static std::unique_ptr<ExprAst> parseParenExpr();

private:

};