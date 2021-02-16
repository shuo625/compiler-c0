#include "errorHandler.h"


ErrorHandler::ErrorHandler(SymbolTable *symbolTable, Output *output) {
    this->symbolTable = symbolTable;
    this->output = output;
}

ErrorHandler::~ErrorHandler() {}

void ErrorHandler::addRow() {
    this->row += 1;
}

int ErrorHandler::getCurrRow() {
    return this->row;
}

void ErrorHandler::setLastRow() {
    this->lastTokenRow = this->row;
}

void ErrorHandler::inValidLetterError() {
    this->output->outError(this->row, INVALIDLETTER);
}

void ErrorHandler::identifierDupError(std::string &var, SyntaxType syntaxType) {
    if (this->symbolTable->varIsDuplication(var)) {
        this->output->outError(this->row, IDENTIFIERDUPLICATION);
        return;
    }
    this->symbolTable->push(var, syntaxType);
}

void ErrorHandler::identifierDupError(std::string &func, SyntaxType syntaxType, std::vector<Paramter> &params) {
    if (this->symbolTable->varIsDuplication(func)) {
        this->output->outError(this->row, IDENTIFIERDUPLICATION);
        return;
    }
    this->symbolTable->push(func, syntaxType, params);
}

void ErrorHandler::identifierNotDefError(std::string &var) {
    if (!this->symbolTable->varIsDefine(var)) {
        this->output->outError(this->row, IDENTIFIERNOTDEFINE);
    }
}

void ErrorHandler::funcParamError(std::string &func, std::vector<SyntaxType> &paramsType) {
    if (!this->symbolTable->funcParamNumIsMatch(func, paramsType.size())) {
        this->output->outError(this->row, FUNCPARAMNUMNOTMATCH);
        return;
    }
    if (!this->symbolTable->funcParamTypeIsMatch(func, paramsType)) {
        this->output->outError(this->row, FUNCPARAMTYPENOTMATCH);
    }
}

void ErrorHandler::inValidQualificationError() {
    this->output->outError(this->row, INVALIDQUALIFICATION);
}

void ErrorHandler::unreturnFuncError() {
    this->output->outError(this->row, UNRETURNEDFUNCNOTMATCH);
}

void ErrorHandler::returnFuncError() {
    this->output->outError(this->row, RETURNEDFUNCNOTMATCH);
}

void ErrorHandler::inValidArrayIndexError() {
    this->output->outError(this->row, INVALIDARRAYINDEX);
}

void ErrorHandler::changeConstError(std::string &var) {
    if (this->symbolTable->varIsConst(var)) {
        this->output->outError(this->row, CHANGECONST);
    }
}

void ErrorHandler::notSemiError() {
    this->output->outError(this->lastTokenRow, NOTSEMI);
}

void ErrorHandler::notParentError() {
    this->output->outError(this->lastTokenRow, NOTPARENT);
}

void ErrorHandler::notBracketError() {
    this->output->outError(this->lastTokenRow, NOTBRACK);
}

void ErrorHandler::lackWhileError() {
    this->output->outError(this->lastTokenRow, LACKWHILE);
}

void ErrorHandler::constDefineError() {
    this->output->outError(this->row, CONSTDEFINEERROR);
}