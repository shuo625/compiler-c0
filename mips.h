#ifndef COMPILER_MIPS_H
#define COMPILER_MIPS_H


#include "stdafx.h"
#include "midCode.h"
#include "tempVarTable.h"
#include "output.h"


class Mips {
public:
    Mips(Output *output, MidCode *midCode);
    ~Mips();

    void geneMips();

    void geneLoad(const std::string &reg, const std::string &addr, bool isGlobal, SyntaxType syntaxType);
    void geneLoadByReg(const std::string &reg, const std::string &addrReg, bool isGlobal, SyntaxType syntaxType);
    void geneStore(const std::string &reg, const std::string &addr, bool isGlobal, SyntaxType syntaxType);
    void geneStoreByReg(const std::string &reg, const std::string &addrReg, bool isGlobal, SyntaxType syntaxType);
    void geneStoreFunc(const std::string &reg, const std::string &addr, const std::string &addrReg, SyntaxType syntaxType);
    void geneAddSub(const std::string &resReg, const std::string &reg1, const std::string &reg2, const std::string &op);
    void geneMulDiv(const std::string &reg1, const std::string &reg2, std::string op);
    void geneLi(const std::string &reg, const std::string &val);
    void geneJ(const std::string &label);
    void geneJar(std::string &func);
    void geneJr();
    void geneJump(std::string &label, const std::string &reg1, const std::string &reg2, std::string &op);
    void geneLabel(std::string &label);
    void geneMflo(const std::string &reg);
    void geneRead(SyntaxType syntaxType);
    void printString(std::string &s);
    void printVar(SyntaxType syntaxType);
    void geneMove(const std::string &reg1, const std::string &reg2);
    void printEnter();

    void geneSignalVarAssignMips(MdCode &mdCode);
    void geneConAssignMips(MdCode &mdCode);
    void geneArrayRefMips(MdCode &mdCode);
    void geneArrayAssignMips(MdCode &mdCode);
    void geneArrayDefMips(MdCode &mdCode);
    void geneDirJumpMips(MdCode &mdCode);
    void geneExprMips(MdCode &mdCode);
    void geneJumpMips(MdCode &mdCode);
    void geneConDefMips(MdCode &mdCode);
    void geneVarDefMips(MdCode &mdCode);
    void geneCallFuncMips(MdCode &mdCode);
    void geneFuncDefMips(MdCode &mdCode);
    void geneLabelMips(MdCode &mdCode);
    void geneReturnMips(MdCode &mdCode);
    void geneScanfMips(MdCode &mdCode);
    void genePrintfMips(MdCode &mdCode);
    void geneEndFuncMips();
    void geneExitMips();

private:
    MidCode *midCode;
    Output *output;
    TempVarTable *tempVarTable;
    bool firstFunc = true;

};


#endif //COMPILER_MIPS_H
