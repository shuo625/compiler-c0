#include "symbolTable.h"


SymbolTable::SymbolTable() {

}

SymbolTable::~SymbolTable() {

}

void SymbolTable::addScope() {
    this->scope += 1;
}

void SymbolTable::decreaseScope() {
    clearScope();
    this->scope -= 1;
}

void SymbolTable::push(std::string &identifier, SyntaxType syntaxType, std::vector<Paramter> &params) {
    Record record;
    record.scope = this->scope;
    record.identifier = identifier;
    record.syntaxType = syntaxType;
    record.paramNum = params.size();
    record.params = params;
    this->symbolTable.push_back(record);
}

void SymbolTable::push(std::string &identifier, SyntaxType syntaxType) {
    Record record;
    record.scope = this->scope;
    record.identifier = identifier;
    record.syntaxType = syntaxType;
    this->symbolTable.push_back(record);
}

void SymbolTable::clearScope() {
    while (true) {
        if (this->symbolTable.size() == 0) {
            break;
        }
        Record record = this->symbolTable.back();
        if (record.scope == this->scope) {
            this->symbolTable.pop_back();
        } else {
            break;
        }
    }
}

bool SymbolTable::varIsDefine(const std::string &var) {
    for (Record &record: this->symbolTable) {
        if (record.identifier == var)
            return true;
    }
    return false;
}

bool SymbolTable::varIsDuplication(const std::string &var) {
    for (int i = this->symbolTable.size() - 1; i >= 0; i--) {
        Record record = this->symbolTable.at(i);

        if (record.scope != this->scope)
            break;

        if (record.identifier == var)
            return true;
    }
    return false;
}

bool SymbolTable::funcParamNumIsMatch(const std::string &func, int paramNum) {
    for (Record &record: this->symbolTable) {
        if (record.identifier == func) {
            return record.paramNum == paramNum;
        }
    }
    return false;
}

bool SymbolTable::funcParamTypeIsMatch(const std::string &func, std::vector<SyntaxType> &paramsType) {
    for (Record &record: this->symbolTable) {
        if (record.identifier == func) {
            for (int i = 0; i < record.paramNum; i++) {
                if (record.params.at(i).syntaxType != paramsType.at(i))
                    return false;
            }
        }
    }
    return true;
}

bool SymbolTable::varIsConst(std::string &var) {
    for (int i = this->symbolTable.size() - 1; i >= 0; i--) {
        Record record = this->symbolTable.at(i);
        if (record.identifier == var) {
            return record.syntaxType == CONSTCHAR || record.syntaxType == CONSTINT;
        }
    }
    return false;
}

bool SymbolTable::varIsInt(std::string &var) {
    for (int i = this->symbolTable.size() - 1; i >= 0; i--) {
        Record record = this->symbolTable.at(i);
        if (record.identifier == var) {
            return record.syntaxType == INT ||
                   record.syntaxType == CONSTINT ||
                   record.syntaxType == INTARRAY ||
                   record.syntaxType == INTRETURNEDFUNC;
        }
    }
    return false;
}

bool SymbolTable::funcIsReturn(std::string &func) {
    for (Record &record: this->symbolTable) {
        if (record.identifier == func) {
            return record.syntaxType == INTRETURNEDFUNC ||
                   record.syntaxType == CHARRETURNFUNC;
        }
    }
    return false;
}

std::vector<std::string> SymbolTable::getFuncParams(std::string &func) {
    std::vector<std::string> funcParams;
    for (Record &record: this->symbolTable) {
        if (record.identifier == func) {
            for (Paramter &param: record.params) {
                funcParams.push_back(param.identifier);
            }
            break;
        }
    }
    return funcParams;
}