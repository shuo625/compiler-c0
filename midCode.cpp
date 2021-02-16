#include "midCode.h"


MidCode::MidCode() {

}

MidCode::~MidCode() {

}

std::string MidCode::getIfLabel() {
    this->curIfLabelNum += 1;
    return this->ifLabelPre + "_" + std::to_string(this->curIfLabelNum);
}

std::string MidCode::getElseLabel() {
    this->curElseLabelNum += 1;
    return this->elseLabelPre + "_" + std::to_string(this->curElseLabelNum);
}

std::string MidCode::getEndIfLabel() {
    this->curEndIfLabelNum += 1;
    return this->endIfLabelPre + "_" + std::to_string(this->curElseLabelNum);
}

std::string MidCode::getWhileLabel() {
    this->curWhileLabelNum += 1;
    return this->whileLabelPre + "_" + std::to_string(this->curWhileLabelNum);
}

std::string MidCode::getEndWhileLabel() {
    this->curEndWhileLabelNum += 1;
    return this->endWhileLabelPre + "_" + std::to_string(this->curEndWhileLabelNum);
}

std::string MidCode::getForLabel() {
    this->curForLabelNum += 1;
    return this->forLabelPre + "_" + std::to_string(this->curForLabelNum);
}

std::string MidCode::getEndForLabel() {
    this->curEndForLabelNum += 1;
    return this->endForLabelPre + "_" + std::to_string(this->curEndForLabelNum);
}

std::string MidCode::getDoLabel() {
    this->curDoLabelNum += 1;
    return this->doLabelPre + "_" + std::to_string(this->curDoLabelNum);
}

std::string MidCode::getEndDoLabel() {
    this->curEndDoLabelNum += 1;
    return this->endDoLabelPre + "_" + std::to_string(this->curEndDoLabelNum);
}

std::string MidCode::getVar() {
    this->curVarNum += 1;
    return this->varPre + std::to_string(this->curVarNum);
}

std::string MidCode::getConVar() {
    this->curConVarNum += 1;
    return this->conPre + std::to_string(this->curConVarNum);
}

void MidCode::resetVar() {
    this->curVarNum = 0;
}

void MidCode::geneArrayAssignMidCode(std::string &var, std::string &arrayVar, std::string &indexVar) {
    MdCode mdCode;
    ArrayAssignMidCode *code = new ArrayAssignMidCode();
    code->arrayVar = arrayVar;
    code->var = var;
    code->indexVar = indexVar;
    mdCode.midCodeType = ARRAYASSIGN;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneArrayDefMidCode(SyntaxType arrType, std::string &arrayVar, int arrayLen) {
    MdCode mdCode;
    ArrayDefMidCode *code = new ArrayDefMidCode();
    code->arrayVar = arrayVar;
    code->syntaxType = arrType;
    code->arrayLen = arrayLen;
    mdCode.midCodeType = ARRAYDEF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneArrayRefMidCode(std::string &tarVar, const std::string &arrayVar,
        std::string &indexVar, SyntaxType syntaxType) {
    MdCode mdCode;
    ArrayRefMidCode *code = new ArrayRefMidCode();
    code->arrayVar = arrayVar;
    code->tarVar = tarVar;
    code->indexVar = indexVar;
    code->syntaxType = syntaxType;
    mdCode.midCodeType = ARRAYREF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneCallFuncMidCode(std::string &func, std::vector<std::string> &vars, std::vector<std::string> &params,
        std::string &var, SyntaxType syntaxType) {
    MdCode mdCode;
    CallFuncMidCode *code = new CallFuncMidCode();
    code->params = params;
    code->vars = vars;
    code->var = var;
    code->func = func;
    code->syntaxType = syntaxType;
    mdCode.midCodeType = CALLFUNC;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneConDefMidCode(SyntaxType syntaxType, std::string &var, const std::string &val) {
    MdCode mdCode;
    ConDefMidCode *code = new ConDefMidCode();
    code->syntaxType = syntaxType;
    code->var = var;
    code->val = val;
    mdCode.midCodeType = CONDEF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneConAssignMidCode(std::string &tarVar, SyntaxType syntaxType, const std::string &val) {
    MdCode mdCode;
    ConAssignMidCode *code = new ConAssignMidCode();
    code->syntaxType = syntaxType;
    code->tarVar = tarVar;
    code->val = val;
    mdCode.midCodeType = CONASSIGN;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneExprMidCode(std::string &tarVar, SyntaxType tarType, std::string &var1, std::string &var2,
                              const std::string &op) {
    MdCode mdCode;
    ExprMidCode *code = new ExprMidCode();
    code->tarVar = tarVar;
    code->tarType = tarType;
    code->var1 = var1;
    code->var2 = var2;
    code->op = op;
    mdCode.midCodeType = EXPR;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneFuncDefMidCode(std::string &func, std::vector<std::string> &params, std::vector<SyntaxType> &paramType) {
    MdCode mdCode;
    FuncDefMidCode *code = new FuncDefMidCode();
    code->params = params;
    code->paramType = paramType;
    code->func = func;
    mdCode.midCodeType = FUNCDEF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneJumpMidCode(std::string &var1, std::string &var2, const std::string &op,
        std::string &label, bool isInitReg) {
    MdCode mdCode;
    JumpMidCode *code = new JumpMidCode();
    code->var1 = var1;
    code->var2 = var2;
    code->op = op;
    code->label = label;
    code->isInitReg = isInitReg;
    mdCode.midCodeType = JUMP;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneLabelMidCode(std::string &label) {
    MdCode mdCode;
    LabelMidCode *code = new LabelMidCode();
    code->label = label;
    mdCode.midCodeType = LABEL;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneVarDefMidCode(SyntaxType syntaxType, std::string &var) {
    MdCode mdCode;
    VarDefMidCode *code = new VarDefMidCode();
    code->syntaxType = syntaxType;
    code->var = var;
    mdCode.midCodeType = VARDEF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::genePrintfMidCode(std::string &v, bool isVar) {
    MdCode mdCode;
    PrintfMidCode *code = new PrintfMidCode();
    code->isDouble = false;
    code->isVar = isVar;
    if (isVar)
        code->var = v;
    else
        code->s = v;
    mdCode.midCodeType = PRINTF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::genePrintfMidCode(std::string &s, std::string &var) {
    MdCode mdCode;
    PrintfMidCode *code = new PrintfMidCode();
    code->var = var;
    code->s = s;
    code->isDouble = true;
    mdCode.midCodeType = PRINTF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneReturnMidCode(std::string &var) {
    MdCode mdCode;
    ReturnMidCode *code = new ReturnMidCode();
    code->var = var;
    code->haveVar = true;
    mdCode.midCodeType = RETURN;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneReturnMidCode() {
    MdCode mdCode;
    ReturnMidCode *code = new ReturnMidCode();
    code->haveVar = false;
    mdCode.p = (int*)code;
    mdCode.midCodeType = RETURN;
    this->midCode.push_back(mdCode);
}

void MidCode::geneScanfMidCode(std::vector<std::string> &var) {
    MdCode mdCode;
    ScanfMidCode *code = new ScanfMidCode();
    code->var = var;
    mdCode.midCodeType = SCANF;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneSignalVarAssignMidCode(std::string &tarVar, std::string &var, SyntaxType syntaxType) {
    MdCode mdCode;
    SignalVarAssignMidCode *code = new SignalVarAssignMidCode();
    code->syntaxType = syntaxType;
    code->tarVar = tarVar;
    code->var = var;
    mdCode.midCodeType = SIGNALVARASSIGN;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneDirJumpMidCode(std::string &label) {
    MdCode mdCode;
    DirJumpMidCode *code = new DirJumpMidCode();
    code->label = label;
    mdCode.midCodeType = DIRJUMP;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneSignalVarAssignMidCode(std::string &tarVar, std::string &var) {
    MdCode mdCode;
    SignalVarAssignMidCode *code = new SignalVarAssignMidCode();
    code->var = var;
    code->tarVar = tarVar;
    mdCode.midCodeType = SIGNALVARASSIGN;
    mdCode.p = (int*)code;
    this->midCode.push_back(mdCode);
}

void MidCode::geneEndFunc() {
    MdCode mdCode;
    mdCode.midCodeType = ENDFUNC;
    this->midCode.push_back(mdCode);
}

void MidCode::geneInitRegs() {
    MdCode mdCode;
    mdCode.midCodeType = INITREGS;
    this->midCode.push_back(mdCode);
}

std::vector<MdCode>& MidCode::getMidCode() {
    return this->midCode;
}

std::string MidCode::pushStringVar(const std::string &s) {
    this->curStringVarNum += 1;
    std::string var = this->stringPre + std::to_string(this->curStringVarNum);
    StringVar strVar;
    strVar.var = var;
    strVar.s = s;
    this->stringVar.push_back(strVar);
    return var;
}

void MidCode::geneCodeBlockBegin() {
    MdCode mdCode;
    mdCode.midCodeType = CODEBLOCKBEGIN;
    this->midCode.push_back(mdCode);
}

void MidCode::geneCodeBlockEnd() {
    MdCode mdCode;
    mdCode.midCodeType = CODEBLOCKEND;
    this->midCode.push_back(mdCode);
}

std::vector<StringVar>& MidCode::getStringVar() {
    return this->stringVar;
}