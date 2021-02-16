#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


#include "stdafx.h"


const int MAXPARAMNUM = 100;


enum SyntaxType {
    CONSTINT,
    CONSTCHAR,
    INT,
    CHAR,
    INTARRAY,
    CHARARRAY,
    INTRETURNEDFUNC,
    CHARRETURNFUNC,
    UNRETURNEDFUNC
};


typedef struct {
    SyntaxType syntaxType;
    std::string identifier;
}Paramter;


typedef struct {
    std::string identifier;
    int scope;
    SyntaxType syntaxType;
    int paramNum;
    std::vector<Paramter> params;
}Record;


class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();
    void addScope();
    void decreaseScope();
    void push(std::string &identifier, SyntaxType syntaxType);
    void push(std::string &identifier, SyntaxType syntaxType, std::vector<Paramter> &params);

    bool varIsDuplication(const std::string &var);
    bool varIsDefine(const std::string &var);
    bool funcParamNumIsMatch(const std::string &func, int paramNum);
    bool funcParamTypeIsMatch(const std::string &func, std::vector<SyntaxType> &paramsType);
    bool varIsConst(std::string &var);
    bool varIsInt(std::string &var);
    bool funcIsReturn(std::string &func);

    std::vector<std::string> getFuncParams(std::string &func);

private:
    int scope = 0;
    std::vector<Record> symbolTable;

    void clearScope();
};


#endif //COMPILER_SYMBOLTABLE_H
