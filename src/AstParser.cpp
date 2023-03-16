/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#include "AstParser.hpp"

// ─── Utilities ───────────────────────────────────────────────────────────────────────────────────

int AstParser::CurrentToken = 0;

int AstParser::getNextToken() {
    return AstParser::CurrentToken = Lexer::get_token();
}

std::unique_ptr<ExprAst> AstParser::logError(const char *str) {
    fprintf(stderr, "Error: %s", str);
    return nullptr;
}

std::unique_ptr<PrototypeAst> AstParser::logErrorP(const char *str) {
    AstParser::logError(str);
    return nullptr;
}

// ─── Expression Parsing ──────────────────────────────────────────────────────────────────────────

std::unique_ptr<ExprAst> parseExpression() {
    return nullptr;
}

std::unique_ptr<NumberExprAst> AstParser::parseNumberExprAst() {
    auto output = std::make_unique<NumberExprAst>(Lexer::NumVal);
    // Consume token
    AstParser::getNextToken();
    return std::move(output);
}

// This function expects current token to be '('
std::unique_ptr<ExprAst> parseParenExpr() {
    // Consume opening parenthesis
    AstParser::getNextToken();

    auto output = AstParser::parseExpression();
    if (!output)
        return nullptr;

    if (AstParser::CurrentToken != ')')
        return AstParser::logError("Expected ')'");
    // Consume closing
    AstParser::getNextToken();
    return output;
}