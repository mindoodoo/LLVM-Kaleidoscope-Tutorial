/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#include "Driver.hpp"

void Driver::mainloop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (this->_parser._currentToken) {
            case Lexer::TOK_EOF:
                return;
            case ';': // ignore top-level semicolons.
                this->_parser.getNextToken();
                break;
            case Lexer::TOK_DEF:
                this->handleDefinition();
                break;
            case Lexer::TOK_EXT:
                this->handleExtern();
                break;
            default:
                this->handleTopLevelExpression();
                break;
        }
    }
}

void Driver::handleDefinition() {
    if (this->_parser.parseDefinition())
        fprintf(stderr, "Parsed a function definition.\n");
    else
        this->_parser.getNextToken();
}

void Driver::handleExtern() {
    if (this->_parser.parseExtern())
        fprintf(stderr, "Parsed an extern\n");
    else
        this->_parser.getNextToken();
}

void Driver::handleTopLevelExpression() {
  if (this->_parser.parseTopLevelExpr())
    fprintf(stderr, "Parsed a top-level expr\n");
  else
    this->_parser.getNextToken();
}