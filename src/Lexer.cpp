/*
 * Created on Wed Mar 15 2023
 *
 * by Léon Sautour
 */

#include "Lexer.hpp"

Lexer::TokenTypes Lexer::get_token() {
    static int last_char = ' ';

    while (isspace(last_char))
        last_char = getchar();

    if (isalpha(last_char)) {
        this->_identifierStr = last_char;

        while (isalnum((last_char = getchar())))
            this->_identifierStr += last_char;

        if (this->_identifierStr == "def")
            return TOK_DEF;
        if (this->_identifierStr == "extern")
            return TOK_EXT;
        return TOK_ID;
    } else if (isdigit(last_char)) {
        std::string num_str;

        do {
            num_str += last_char;
            last_char = getchar();
        } while (isdigit(last_char) || last_char == '.');

        this->_numVal = strtod(num_str.c_str(), 0);
        return TOK_NUM;
    } else if (last_char == '#') {
        // Handles comments
        do
            last_char = getchar();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return Lexer::get_token();
    }
    if (last_char == EOF)
        return TOK_EOF;

    // Otherwise return error
    return ERROR;
}