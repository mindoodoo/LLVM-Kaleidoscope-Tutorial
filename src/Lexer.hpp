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

    static std::string IdentifierStr;   // Filled if TOK_ID
    static double NumVal;               // Filled if TOK_NUM

    static TokenTypes get_token();
};