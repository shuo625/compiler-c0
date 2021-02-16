#include "optimize.h"


Optimize::Optimize(TempVarTable *tempVarTable) {
    this->tempVarTable = tempVarTable;
}

Optimize::~Optimize() {}

void Optimize::initReg() {
    this->regs.clear();
    for (int i = 0; i < this->regNum; i++) {
        Reg reg;
        reg.regNumber = 5 + i;
        reg.isNew = true;
        this->regs.push_back(reg);
    }
    this->regUse.clear();
}

Reg Optimize::getReg() {
    if (!this->regs.empty()) {
        Reg reg = this->regs.back();
        this->regs.pop_back();
        return reg;
    }
    Reg reg = this->regUse.at(0);
    std::string var = reg.var;
    this->tempVarTable->updataVarRegUse(var);
    this->regUse.erase(this->regUse.begin());
    return reg;
}

void Optimize::updateReg(const Reg &reg) {
    this->regUse.push_back(reg);
}

void Optimize::updateReg(int regNumber) {
    Reg reg;
    std::vector<Reg>::iterator itor;
    for (itor = this->regUse.begin(); itor != this->regUse.end(); itor++) {
        Reg r = *itor;
        if (r.regNumber == regNumber) {
            reg = r;
            this->regUse.erase(itor);
            break;
        }
    }
    this->regUse.push_back(reg);
}

std::vector<Reg>& Optimize::getRegUse() {
    return this->regUse;
}