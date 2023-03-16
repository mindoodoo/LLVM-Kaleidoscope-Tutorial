/*
 * Created on Thu Mar 16 2023
 *
 * by Léon Sautour
 */

#pragma once

#include <stdio.h>
#include "AstParser.hpp"
#include "Lexer.hpp"

class Driver {
public:
    void mainloop();

    void handleDefinition();
    void handleExtern();
    void handleTopLevelExpression();

private:
    AstParser _parser;
};
