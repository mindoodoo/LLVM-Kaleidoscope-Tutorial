/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#include "AstParser.hpp"

// ─── Utilities ───────────────────────────────────────────────────────────────────────────────────

int AstParser::getNextToken() {
    return this->_currentToken = this->_lexer.get_token();
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
    auto output = std::make_unique<NumberExprAst>(this->_lexer._numVal);
    // Consume token
    AstParser::getNextToken();
    return std::move(output);
}

// This function expects current token to be '('
std::unique_ptr<ExprAst> AstParser::parseParenExpr() {
    // Consume opening parenthesis
    this->getNextToken();

    auto output = this->parseExpression();
    if (!output)
        return nullptr;

    if (this->_currentToken != ')')
        return AstParser::logError("Expected ')'");
    // Consume closing
    this->getNextToken();
    return output;
}

std::unique_ptr<ExprAst> AstParser::parseIdentifierExpr() {
    std::string idName = this->_lexer._identifierStr;

    // consume identifier
    this->getNextToken();

    // Simple var ref
    if (this->_currentToken != '(')
        return std::make_unique<VariableExprAst>(idName);

    // Fuction call
    getNextToken();  // consume (
    std::vector<std::unique_ptr<ExprAst>> args;
    if (this->_currentToken != ')') {
        while (true) {
        if (auto arg = this->parseExpression())
            args.push_back(std::move(arg));
        else
            return nullptr;

        if (this->_currentToken == ')')
            break;

        if (this->_currentToken != ',')
            return this->logError("Expected ')' or ',' in argument list");
        this->getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return std::make_unique<CallExprAst>(idName, std::move(args));
}

std::unique_ptr<ExprAst> AstParser::ParsePrimary() {
    switch (this->_currentToken) {
    default:
        return this->logError("unknown token when expecting an expression");
    case Lexer::TOK_ID:
        return this->parseIdentifierExpr();
    case Lexer::TOK_NUM:
        return this->parseNumberExprAst();
    case '(':
        return this->parseParenExpr();
    }
}
