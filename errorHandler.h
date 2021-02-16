#ifndef COMPILER_ERRORHANDLER_H
#define COMPILER_ERRORHANDLER_H


#include "stdafx.h"
#include "output.h"
#include "symbolTable.h"


enum ErrorType {
    INVALIDLETTER = 'a',
    IDENTIFIERDUPLICATION,
    IDENTIFIERNOTDEFINE,
    FUNCPARAMNUMNOTMATCH,
    FUNCPARAMTYPENOTMATCH,
    INVALIDQUALIFICATION,
    UNRETURNEDFUNCNOTMATCH,
    RETURNEDFUNCNOTMATCH,
    INVALIDARRAYINDEX,
    CHANGECONST,
    NOTSEMI,
    NOTPARENT,
    NOTBRACK,
    LACKWHILE,
    CONSTDEFINEERROR
};


class ErrorHandler{
public:
    ErrorHandler(SymbolTable *symbolTable, Output *output);
    ~ErrorHandler();
    void inValidLetterError();
    void identifierDupError(std::string &var, SyntaxType syntaxType);
    void identifierDupError(std::string &func, SyntaxType syntaxType, std::vector<Paramter> &params);
    void identifierNotDefError(std::string &var);
    void funcParamError(std::string &func, std::vector<SyntaxType> &paramsType);
    void inValidQualificationError();
    void unreturnFuncError();
    void returnFuncError();
    void inValidArrayIndexError();
    void changeConstError(std::string &var);
    void notSemiError();
    void notParentError();
    void notBracketError();
    void lackWhileError();
    void constDefineError();

    void addRow();
    int getCurrRow();
    void setLastRow();

private:
    SymbolTable *symbolTable;
    Output *output;
    int row = 1;
    int lastTokenRow = 1;
};


#endif //COMPILER_ERRORHANDLER_H
