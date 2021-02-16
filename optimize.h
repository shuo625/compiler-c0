#ifndef COMPILER_OPTIMIZE_H
#define COMPILER_OPTIMIZE_H


#include "stdafx.h"
#include "midCode.h"
#include "tempVarTable.h"
#include "symbolTable.h"


typedef struct {
    int regNumber;
    std::string var;
    std::string varAddr;
    SyntaxType syntaxType;
    bool isGlobal;
    bool isNew;
}Reg;


class Optimize {
public:
    Optimize(TempVarTable *tempVarTable);
    ~Optimize();

    void initReg();
    Reg getReg();
    void updateReg(const Reg &reg);
    void updateReg(int regNumber);

    std::vector<Reg>& getRegUse();

private:
    TempVarTable *tempVarTable;
    std::vector<Reg> regs;
    std::vector<Reg> regUse;
    int regNum = 23;
};


#endif //COMPILER_OPTIMIZE_H
