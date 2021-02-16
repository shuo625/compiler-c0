#ifndef COMPILER_TEMPVARTABLE_H
#define COMPILER_TEMPVARTABLE_H


#include "stdafx.h"
#include "symbolTable.h"


typedef struct {
    std::string var;
    std::string addr;
    std::string val;
    SyntaxType syntaxType;
    std::string reg;
    int regNumber = -1;
    int scope;
    bool isGlobal;
}TempVar;


class TempVarTable {
public:
    TempVarTable();
    ~TempVarTable();

    void addScope();
    void decreaseScope();


    void pushConVar(std::string &conVar, std::string &val, SyntaxType syntaxType);
    void pushVar(std::string &var, SyntaxType syntaxType);
    void pushArray(std::string &array, SyntaxType syntaxType, int arrLen);
    void pushFunc(std::string &func, std::vector<std::string> &params, std::vector<SyntaxType> paramsType);
    TempVar& getVarAddr(std::string &var, SyntaxType syntaxType=INT);

    std::vector<std::string>& getFuncParamAddr(std::string &func);

    int getCurAddr();

    void updataVarRegUse(const std::string &var);

private:
    std::vector<TempVar> tempVarTable;
    std::map<std::string, std::vector<std::string>> funcParamAddr;

    int scope = 0;
    int curAddr = 0;
};


#endif //COMPILER_TEMPVARTABLE_H
