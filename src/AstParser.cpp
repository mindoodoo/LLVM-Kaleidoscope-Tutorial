/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#include "AstParser.hpp"

AstParser::AstParser() {
    this->_binopPrecedence['<'] = 10;
    this->_binopPrecedence['+'] = 20;
    this->_binopPrecedence['-'] = 20;
    this->_binopPrecedence['*'] = 40;
}

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

// ─── Binary Expressions ──────────────────────────────────────────────────────────────────────────

std::unique_ptr<ExprAst> AstParser::parsePrimary() {
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

int AstParser::getTokenPrecedence() {
    if (isascii(this->_currentToken))
        return -1;

    // Make sure it's declared binop
    int tokPrec = this->_binopPrecedence[this->_currentToken];
    if (tokPrec <= 0)
        return -1;
    return tokPrec;
}

std::unique_ptr<ExprAst> AstParser::parseExpression() {
    auto lhs = this->parsePrimary();
    if (!lhs)
        return nullptr;

    return this->parseBinOpRHS(0, std::move(lhs));
}

std::unique_ptr<ExprAst> AstParser::parseBinOpRHS(int exprPrec, std::unique_ptr<ExprAst> lhs) {
    while (true) {
        int tokPrec = this->getTokenPrecedence();

        if (tokPrec < exprPrec)
            return lhs;

        int binOp = this->_currentToken;
        this->getNextToken();

        auto rhs = this->parsePrimary();
        if (!rhs)
            return nullptr;

        int nextPrec = this->getTokenPrecedence();
        if (tokPrec < nextPrec) {
            rhs = this->parseBinOpRHS(tokPrec + 1, std::move(rhs));
            if (!rhs)
                return nullptr;
        }

        lhs = std::make_unique<BinaryExprAst>(binOp, std::move(lhs), std::move(rhs));
    }
}

// ─── Functions ───────────────────────────────────────────────────────────────────────────────────

std::unique_ptr<PrototypeAst> AstParser::parsePrototype() {
    if (this->_currentToken != Lexer::TOK_ID)
        return this->logErrorP("Expected function name in prototype");

    std::string FnName = this->_lexer._identifierStr;
    getNextToken();

    if (this->_currentToken != '(')
        return this->logErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string> ArgNames;
    while (getNextToken() == Lexer::TOK_ID)
        ArgNames.push_back(this->_lexer._identifierStr);
    if (this->_currentToken != ')')
        return this->logErrorP("Expected ')' in prototype");

    // success.
    getNextToken();  // eat ')'.

    return std::make_unique<PrototypeAst>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAst> AstParser::parseDefinition() {
  this->getNextToken();  // eat def.
  auto proto = this->parsePrototype();
  if (!proto) return nullptr;

  if (auto e = this->parseExpression())
    return std::make_unique<FunctionAst>(std::move(proto), std::move(e));
  return nullptr;
}

std::unique_ptr<PrototypeAst> AstParser::parseExtern() {
    this->getNextToken();  // eat extern.
    return this->parsePrototype();
}

std::unique_ptr<FunctionAst> AstParser::parseTopLevelExpr() {
  if (auto e = this->parseExpression()) {
    // Make an anonymous proto.
    auto proto = std::make_unique<PrototypeAst>("", std::vector<std::string>());
    return std::make_unique<FunctionAst>(std::move(proto), std::move(e));
  }
  return nullptr;
}