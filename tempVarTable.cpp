#include "tempVarTable.h"


TempVarTable::TempVarTable() {
}

TempVarTable::~TempVarTable() {}

void TempVarTable::addScope() {
    this->scope += 1;
}

void TempVarTable::decreaseScope() {
    while (true) {
        TempVar &tempVar = this->tempVarTable.back();
        if (tempVar.scope == this->scope)
            this->tempVarTable.pop_back();
        else
            break;
    }
    this->scope -= 1;
}

void TempVarTable::pushConVar(std::string &conVar, std::string &val, SyntaxType syntaxType) {
    TempVar tempVar;
    tempVar.var = conVar;
    tempVar.val = val;
    tempVar.scope = this->scope;
    tempVar.isGlobal = this->scope == 0;
    tempVar.syntaxType = syntaxType;

    this->tempVarTable.push_back(tempVar);
}

void TempVarTable::pushVar(std::string &var, SyntaxType syntaxType) {
    TempVar tempVar;
    this->curAddr -= 4;
    tempVar.var = var;
    tempVar.scope = this->scope;
    tempVar.syntaxType = syntaxType;
    tempVar.addr = std::to_string(this->curAddr);
    tempVar.isGlobal = this->scope == 0;

    this->tempVarTable.push_back(tempVar);
}

void TempVarTable::pushArray(std::string &array, SyntaxType syntaxType, int arrLen) {
    this->curAddr -= 4 * arrLen;
    TempVar tempVar;
    tempVar.var = array;
    tempVar.scope = this->scope;
    tempVar.syntaxType = syntaxType;
    tempVar.addr = std::to_string(this->curAddr);
    tempVar.isGlobal = this->scope == 0;

    this->tempVarTable.push_back(tempVar);
}

void TempVarTable::pushFunc(std::string &func, std::vector<std::string> &params, std::vector<SyntaxType> paramsType) {
    addScope();
    this->curAddr = 0;
    std::vector<std::string> paramsAddr;
    for (int i = 0; i < params.size(); i++) {
        pushVar(params.at(i), paramsType.at(i));
        paramsAddr.push_back(std::to_string(this->curAddr));
    }
    this->funcParamAddr.insert(
            std::pair<std::string, std::vector<std::string>>(func, paramsAddr));
}

std::vector<std::string>& TempVarTable::getFuncParamAddr(std::string &func) {
    return this->funcParamAddr.at(func);
}

TempVar& TempVarTable::getVarAddr(std::string &var, SyntaxType syntaxType) {
    for (int i = this->tempVarTable.size() - 1; i >= 0; i--) {
        TempVar &tempVar = this->tempVarTable.at(i);
        if (tempVar.var == var) {
            return tempVar;
        }
    }
    pushVar(var, syntaxType);

    return this->tempVarTable.back();
}

int TempVarTable::getCurAddr() {
    return this->curAddr;
}

void TempVarTable::updataVarRegUse(const std::string &var) {
    for (int i = this->tempVarTable.size() - 1; i >= 0; i--) {
        TempVar &tempVar = this->tempVarTable.at(i);
        if (tempVar.var == var) {
            tempVar.regNumber = -1;
            break;
        }
    }
}