/*
 * Created on Wed Mar 15 2023
 *
 * by Léon Sautour
 */

#pragma once

#include <string>

class Lexer {
public:
    enum TokenTypes {
        TOK_EOF,
        TOK_DEF,
        TOK_EXT,
        TOK_ID,
        TOK_NUM,
        ERROR
    };

    std::string _identifierStr;   // Filled if TOK_ID
    double _numVal;               // Filled if TOK_NUM

    TokenTypes get_token();
};