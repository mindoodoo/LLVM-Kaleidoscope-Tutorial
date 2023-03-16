/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#pragma once

#include <memory>
#include <stdio.h>
#include <map>
#include "Ast.hpp"
#include "Lexer.hpp"

class AstParser {
public:
    AstParser();

    // Utilities
    int _currentToken;
    int getNextToken();

    // Error handling
    std::unique_ptr<ExprAst> logError(const char *str);
    std::unique_ptr<PrototypeAst> logErrorP(const char *str);

    std::unique_ptr<NumberExprAst> parseNumberExprAst();

    // parenexpr ::= '(' expression ')'
    std::unique_ptr<ExprAst> parseParenExpr();

    std::unique_ptr<ExprAst> parseIdentifierExpr();

    // primary
    //   ::= identifierexpr
    //   ::= numberexpr
    //   ::= parenexpr
    std::unique_ptr<ExprAst> parsePrimary();

    // ─── Binary Operators ────────────────────────────────────────────────────────────────────

    std::map<char, int> _binopPrecedence;

    // Get precedence of pending binary operator
    int getTokenPrecedence();
    // Expression parsing
    std::unique_ptr<ExprAst> parseExpression();
    std::unique_ptr<ExprAst> parseBinOpRHS(int exprPrec, std::unique_ptr<ExprAst> lhs);
    std::unique_ptr<FunctionAst> parseTopLevelExpr();

    // ─── Functions ───────────────────────────────────────────────────────────────────────────

    std::unique_ptr<PrototypeAst> parsePrototype();
    std::unique_ptr<FunctionAst> parseDefinition();
    std::unique_ptr<PrototypeAst> AstParser::parseExtern();


private:
    Lexer _lexer;
};