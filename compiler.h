#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H


#include "stdafx.h"
#include "symbolTable.h"
#include "output.h"
#include "syntax.h"
#include "token.h"
#include "errorHandler.h"
#include "optMips.h"
#include "mips.h"


class Compiler {
public:
    Compiler(std::string &compile_file);
    ~Compiler();
    void compile();

private:
    SyntaxParse *syntaxParse;
    OptMips *optMips;
    Mips *mips;

    bool isOpt = true;
};


#endif //COMPILER_COMPILER_H
