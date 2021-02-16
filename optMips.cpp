#include "optMips.h"


OptMips::OptMips(Output *output, MidCode *midCode) {
    this->output = output;
    this->midCode = midCode;
    this->tempVarTable = new TempVarTable();
    this->optimize = new Optimize(this->tempVarTable);
}

OptMips::~OptMips() {}

void OptMips::initRegAllVars() {
    std::vector<Reg> &regs = this->optimize->getRegUse();
    for (Reg &reg: regs) {
        std::string r = "$" + std::to_string(reg.regNumber);
        geneStoreVar(r, reg.varAddr, reg.isGlobal, reg.syntaxType);
        this->tempVarTable->updataVarRegUse(reg.var);
    }
    this->optimize->initReg();
}

void OptMips::initRegGlobalVars() {
    std::vector<Reg> &regs = this->optimize->getRegUse();
    for (Reg &reg: regs) {
        if (reg.isGlobal) {
            std::string r = "$" + std::to_string(reg.regNumber);
            geneStoreVar(r, reg.varAddr, reg.isGlobal, reg.syntaxType);
            this->tempVarTable->updataVarRegUse(reg.var);
        }
    }
}

int OptMips::stringToInt(const std::string &val, SyntaxType syntaxType) {
    if (syntaxType == CONSTINT) {
        return std::stoi(val);
    } else {
        char c = val.at(1);
        return c;
    }
}

bool OptMips::varIsConst(SyntaxType syntaxType) {
    return syntaxType == CONSTINT || syntaxType == CONSTCHAR;
}

TempVar& OptMips::getVar(std::string &var, bool needLoad, SyntaxType syntaxType) {
    TempVar &tempVar = this->tempVarTable->getVarAddr(var, syntaxType);
    SyntaxType s = tempVar.syntaxType;
    if (s == CONSTINT || s == CONSTCHAR || s == INTARRAY || s == CHARARRAY)
        return tempVar;
    if (tempVar.regNumber != -1) {
        this->optimize->updateReg(tempVar.regNumber);
        return tempVar;
    }
    Reg reg = this->optimize->getReg();
    if (reg.isNew) {
        reg.isNew = false;
        reg.var = tempVar.var;
        reg.syntaxType = tempVar.syntaxType;
        reg.isGlobal = tempVar.isGlobal;
        reg.varAddr = tempVar.addr;
        tempVar.regNumber = reg.regNumber;
        tempVar.reg = "$" + std::to_string(tempVar.regNumber);
    } else {
        geneStoreVar("$" + std::to_string(reg.regNumber), reg.varAddr, reg.isGlobal, reg.syntaxType);
        reg.var = tempVar.var;
        reg.varAddr = tempVar.addr;
        reg.isGlobal = tempVar.isGlobal;
        reg.syntaxType = tempVar.syntaxType;
        tempVar.regNumber = reg.regNumber;
        tempVar.reg = "$" + std::to_string(tempVar.regNumber);
    }
    this->optimize->updateReg(reg);
    if (needLoad) {
        geneLoadVar(tempVar.reg, tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
    }
    return tempVar;
}

void OptMips::geneExitMips() {
    this->output->outMips("li $v0, 10");
    this->output->outMips("syscall");
}

void OptMips::geneMove(const std::string &reg1, const std::string &reg2) {
    this->output->outMips("move " + reg1 + ", " + reg2);
}

void OptMips::printEnter() {
    std::string c = "'\\n'";
    geneLi("$a0", c);
    geneLi("$v0", "11");
    this->output->outMips("syscall");
}

void OptMips::printString(std::string &s) {
    this->output->outMips("la $a0, " + s);
    geneLi("$v0", "4");
    this->output->outMips("syscall");
}

void OptMips::printVar(SyntaxType syntaxType) {
    if (syntaxType == INT || syntaxType == CONSTINT) {
        geneLi("$v0", "1");
    } else {
        geneLi("$v0", "11");
    }
    this->output->outMips("syscall");
}

void OptMips::geneLoadVar(const std::string &reg, const std::string &addr, bool isGlobal, SyntaxType syntaxType) {
    if (isGlobal) {
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("lw " + reg + ", " + addr + "($sp)");
        } else {
            this->output->outMips("lb " + reg + ", " + addr + "($sp)");
        }
    } else {
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("lw " + reg + ", " + addr + "($fp)");
        } else {
            this->output->outMips("lb " + reg + ", " + addr + "($fp)");
        }
    }
}

void OptMips::geneLoadArray(const std::string &reg, const std::string &addrReg, bool isGlobal, SyntaxType syntaxType) {
    if (isGlobal) {
        geneExpr("$a0", addrReg, "$sp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("lw " + reg + ", " + "0($a0)");
        } else {
            this->output->outMips("lb " + reg + ", " + "0($a0)");
        }
    } else {
        geneExpr("$a0", addrReg, "$fp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("lw " + reg + ", " + "0($a0)");
        } else {
            this->output->outMips("lb " + reg + ", " + "0($a0)");
        }
    }
}

void OptMips::geneStoreVar(const std::string &reg, const std::string &addr, bool isGlobal, SyntaxType syntaxType) {
    if (isGlobal) {
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("sw " + reg + ", " + addr + "($sp)");
        } else {
            this->output->outMips("sb " + reg + ", " + addr + "($sp)");
        }
    } else {
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("sw " + reg + ", " + addr + "($fp)");
        } else {
            this->output->outMips("sb " + reg + ", " + addr + "($fp)");
        }
    }
}

void OptMips::geneStoreArray(const std::string &reg, const std::string &addrReg, bool isGlobal, SyntaxType syntaxType) {
    if (isGlobal) {
        geneExpr("$a0", addrReg, "$sp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("sw " + reg + ", " + "0($a0)");
        } else {
            this->output->outMips("sb " + reg + ", " + "0($a0)");
        }
    } else {
        geneExpr("$a0", addrReg, "$fp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("sw " + reg + ", " + "0($a0)");
        } else {
            this->output->outMips("sb " + reg + ", " + "0($a0)");
        }
    }
}

void OptMips::geneStoreFunc(const std::string &reg, const std::string &addr, const std::string &addrReg,
                     SyntaxType syntaxType) {
    if (syntaxType == INT || syntaxType == CONSTINT) {
        this->output->outMips("sw " + reg + ", " + addr + "(" + addrReg + ")");
    } else {
        this->output->outMips("sb " + reg + ", " + addr + "(" + addrReg + ")");
    }
}

void OptMips::geneLi(const std::string &reg, const std::string &val) {
    this->output->outMips("li " + reg + ", " + val);
}

void OptMips::geneExpr(const std::string &resReg, const std::string &reg, const std::string &regOrVal,
        const std::string &op) {
    if (op == "+")
        this->output->outMips("add " + resReg + ", " + reg + ", " + regOrVal);
    else if (op == "-")
        this->output->outMips("sub " + resReg + ", " + reg + ", " + regOrVal);
    else if (op == "*")
        this->output->outMips("mul " + resReg + ", " + reg + ", " + regOrVal);
    else {
        if (regOrVal.at(0) == '$') {
            this->output->outMips("div " + reg + ", " + regOrVal);
            this->output->outMips("mflo " + resReg);
        } else {
            this->output->outMips("li $a0, " + regOrVal);
            this->output->outMips("div " + reg + ", $a0");
            this->output->outMips("mflo " + resReg);
        }
    }

}

void OptMips::geneJ(const std::string &label) {
    this->output->outMips("j " + label);
}

void OptMips::geneJar(std::string &func) {
    this->output->outMips("jal " + func);
}

void OptMips::geneJr() {
    this->output->outMips("jr $ra");
}

void OptMips::geneLabel(std::string &label) {
    this->output->outMips(label + ":");
}

void OptMips::geneJump(std::string &label, const std::string &reg, const std::string &regOrVal, std::string &op) {
    if (op == ">=") {
        this->output->outMips("bge " + reg + ", " + regOrVal + ", " + label);
    } else if (op == ">") {
        this->output->outMips("bgt " + reg + ", " + regOrVal + ", " + label);
    } else if (op == "<=") {
        this->output->outMips("ble " + reg + ", " + regOrVal + ", " + label);
    } else if (op == "<") {
        this->output->outMips("blt " + reg + ", " + regOrVal + ", " + label);
    } else if (op == "==") {
        this->output->outMips("beq " + reg + ", " + regOrVal + ", " + label);
    } else {
        this->output->outMips("bne " + reg + ", " + regOrVal + ", " + label);
    }
}

void OptMips::geneRead(SyntaxType syntaxType) {
    if (syntaxType == INT) {
        geneLi("$v0", "5");
        this->output->outMips("syscall");
    } else {
        geneLi("$v0", "12");
        this->output->outMips("syscall");
    }
}

void OptMips::geneArrayAssignMips(MdCode &mdCode) {
    ArrayAssignMidCode *code = (ArrayAssignMidCode*)mdCode.p;
    TempVar tempVar = getVar(code->var, true);
    TempVar tempArrVar = this->tempVarTable->getVarAddr(code->arrayVar);
    TempVar tempIndexVar = getVar(code->indexVar, true);

    geneLi("$a0", tempArrVar.addr);
    if (varIsConst(tempIndexVar.syntaxType)) {
        int offset = 4 * stringToInt(tempIndexVar.val, tempIndexVar.syntaxType);
        geneExpr("$a0", "$a0", std::to_string(offset), "+");
    } else {
        geneExpr("$v1", tempIndexVar.reg, "4", "*");
        geneExpr("$a0", "$a0", "$v1", "+");
    }
    if (varIsConst(tempVar.syntaxType)) {
        geneLi("$v1", tempVar.val);
        geneStoreArray("$v1", "$a0", tempArrVar.isGlobal, tempArrVar.syntaxType);
    } else {
        geneStoreArray(tempVar.reg, "$a0", tempArrVar.isGlobal, tempArrVar.syntaxType);
    }
}

void OptMips::geneArrayDefMips(MdCode &mdCode) {
    ArrayDefMidCode *code = (ArrayDefMidCode*)mdCode.p;
    this->tempVarTable->pushArray(code->arrayVar, code->syntaxType, code->arrayLen);
}

void OptMips::geneArrayRefMips(MdCode &mdCode) {
    ArrayRefMidCode *code = (ArrayRefMidCode*)mdCode.p;
    TempVar tempArrVar = this->tempVarTable->getVarAddr(code->arrayVar);
    TempVar tempVar = getVar(code->tarVar, false, code->syntaxType);
    TempVar tempIndexVar = getVar(code->indexVar, true);
    geneLi("$a0", tempArrVar.addr);
    if (varIsConst(tempIndexVar.syntaxType)) {
        int offset = 4 * stringToInt(tempIndexVar.val, tempIndexVar.syntaxType);
        geneExpr("$a0", "$a0", std::to_string(offset), "+");
    } else {
        geneExpr("$v1", tempIndexVar.reg, "4", "*");
        geneExpr("$a0", "$a0", "$v1", "+");
    }
    geneLoadArray(tempVar.reg, "$a0", tempArrVar.isGlobal, tempArrVar.syntaxType);
}

void OptMips::geneSignalVarAssignMips(MdCode &mdCode) {
    SignalVarAssignMidCode *code = (SignalVarAssignMidCode*)mdCode.p;
    TempVar tempTarVar = getVar(code->tarVar, false, code->syntaxType);
    TempVar tempVar = getVar(code->var, true);
    if (varIsConst(tempVar.syntaxType)) {
        geneLi(tempTarVar.reg, tempVar.val);
    } else {
        geneMove(tempTarVar.reg, tempVar.reg);
    }
}

void OptMips::geneConAssignMips(MdCode &mdCode) {
    ConAssignMidCode *code = (ConAssignMidCode*)mdCode.p;
    this->tempVarTable->pushConVar(code->tarVar, code->val, code->syntaxType);
}

void OptMips::geneDirJumpMips(MdCode &mdCode) {
    DirJumpMidCode *code = (DirJumpMidCode*)mdCode.p;
    geneJ(code->label);
}

void OptMips::geneJumpMips(MdCode &mdCode) {
    JumpMidCode *code = (JumpMidCode*)mdCode.p;
    TempVar tempVar1 = getVar(code->var1, true);
    TempVar tempVar2 = getVar(code->var2, true);
    std::string reg;
    std::string regOrVal;
    if (varIsConst(tempVar1.syntaxType)) {
        geneLi("$a0", tempVar1.val);
        reg = "$a0";
    } else {
        reg = tempVar1.reg;
    }
    if (varIsConst(tempVar2.syntaxType)) {
        regOrVal = tempVar2.val;
    } else {
        regOrVal = tempVar2.reg;
    }
    if (code->isInitReg) {
        initRegAllVars();
    }
    geneJump(code->label, reg, regOrVal, code->op);
}

void OptMips::geneExprMips(MdCode &mdCode) {
    ExprMidCode *code = (ExprMidCode*)mdCode.p;
    TempVar tempVar1 = getVar(code->var1, true);
    TempVar tempVar2 = getVar(code->var2, true);
    TempVar tempTarVar = getVar(code->tarVar, false, code->tarType);
    if (varIsConst(tempVar1.syntaxType) && varIsConst(tempVar2.syntaxType)) {
        int ans;
        int var1 = stringToInt(tempVar1.val, tempVar1.syntaxType);
        int var2 = stringToInt(tempVar2.val, tempVar2.syntaxType);
        if (code->op == "+") {
            ans = var1 + var2;
        } else if (code->op == "-") {
            ans = var1 - var2;
        } else if (code->op == "*") {
            ans = var1 * var2;
        } else {
            ans = var1 / var2;
        }
        geneLi(tempTarVar.reg, std::to_string(ans));
    } else if (varIsConst(tempVar1.syntaxType)) {
        geneExpr(tempTarVar.reg, tempVar2.reg, tempVar1.val, code->op);
    } else if (varIsConst(tempVar2.syntaxType)) {
        geneExpr(tempTarVar.reg, tempVar1.reg, tempVar2.val, code->op);
    } else {
        geneExpr(tempTarVar.reg, tempVar1.reg, tempVar2.reg, code->op);
    }
}

void OptMips::geneConDefMips(MdCode &mdCode) {
    ConDefMidCode *code = (ConDefMidCode*)mdCode.p;
    this->tempVarTable->pushConVar(code->var, code->val, code->syntaxType);
}

void OptMips::geneVarDefMips(MdCode &mdCode) {
    VarDefMidCode *code = (VarDefMidCode*)mdCode.p;
    this->tempVarTable->pushVar(code->var, code->syntaxType);
}

void OptMips::geneLabelMips(MdCode &mdCode) {
    LabelMidCode *code = (LabelMidCode*)mdCode.p;
    geneLabel(code->label);
}

void OptMips::geneReturnMips(MdCode &mdCode) {
    ReturnMidCode *code = (ReturnMidCode*)mdCode.p;
    if (code->haveVar) {
        TempVar tempVar = getVar(code->var, true);
        if (varIsConst(tempVar.syntaxType)) {
            geneLi("$v0", tempVar.val);
        } else {
            geneMove("$v0", tempVar.reg);
        }
    }
    initRegGlobalVars();
    geneJr();
}

void OptMips::geneScanfMips(MdCode &mdCode) {
    ScanfMidCode *code = (ScanfMidCode*)mdCode.p;
    for (std::string &v: code->var) {
        TempVar tempVar = getVar(v);
        geneRead(tempVar.syntaxType);
        geneMove(tempVar.reg, "$v0");
    }
}

void OptMips::geneFuncDefMips(MdCode &mdCode) {
    this->optimize->initReg();
    FuncDefMidCode *code = (FuncDefMidCode*)mdCode.p;
    this->tempVarTable->pushFunc(code->func, code->params, code->paramType);
}

void OptMips::geneCallFuncMips(MdCode &mdCode) {
    CallFuncMidCode *code = (CallFuncMidCode*)mdCode.p;
    int curAddr = this->tempVarTable->getCurAddr();
    curAddr -= 4;
    geneStoreVar("$fp", std::to_string(curAddr), false, INT);
    curAddr -= 4;
    geneStoreVar("$ra", std::to_string(curAddr), false, INT);
    int offset = curAddr;
    for (std::string &var: code->vars) {
        curAddr -= 4;
        TempVar tempVar = getVar(var, true);
        if (varIsConst(tempVar.syntaxType)) {
            geneLi("$a0", tempVar.val);
            geneStoreVar("$a0", std::to_string(curAddr), false, tempVar.syntaxType);
        } else {
            geneStoreVar(tempVar.reg, std::to_string(curAddr), false, tempVar.syntaxType);
        }
    }
    initRegAllVars();
    geneExpr("$fp", "$fp", std::to_string(offset), "+");

    geneJar(code->func);

    geneLoadVar("$ra", "0", false, INT);
    geneLoadVar("$fp", "4", false, INT);
    TempVar tempVar = getVar(code->var, false, code->syntaxType);
    geneMove(tempVar.reg, "$v0");

}

void OptMips::genePrintfMips(MdCode &mdCode) {
    PrintfMidCode *code = (PrintfMidCode*)mdCode.p;
    if (code->isDouble) {
        printString(code->s);
        TempVar tempVar = getVar(code->var, true);
        if (tempVar.syntaxType == CONSTINT || tempVar.syntaxType == CONSTCHAR) {
            geneLi("$a0", tempVar.val);
        } else {
            geneMove("$a0", tempVar.reg);
        }
        printVar(tempVar.syntaxType);
        printEnter();
    } else {
        if (code->isVar) {
            TempVar tempVar = getVar(code->var, true);
            if (tempVar.syntaxType == CONSTINT || tempVar.syntaxType == CONSTCHAR) {
                geneLi("$a0", tempVar.val);
            } else {
                geneMove("$a0", tempVar.reg);
            }
            printVar(tempVar.syntaxType);
            printEnter();
        } else {
            printString(code->s);
            printEnter();
        }
    }
}

void OptMips::geneEndFuncMips() {
    this->tempVarTable->decreaseScope();
    initRegGlobalVars();
    this->geneJr();

}

void OptMips::geneInitRegsMips() {
    initRegAllVars();
}

void OptMips::geneMips() {
    std::vector<StringVar> &stringVar = this->midCode->getStringVar();
    this->output->outMips(".data");
    for (StringVar &var: stringVar) {
        this->output->outMips(var.var + ": .asciiz " + "\"" + var.s + "\"");
    }
    this->output->outMips(".text");
    std::vector<MdCode> &mdCode = this->midCode->getMidCode();
    geneMove("$fp", "$sp");
    for (MdCode &code: mdCode) {
        switch (code.midCodeType) {
            case CONASSIGN:
                geneConAssignMips(code);
                break;
            case ARRAYREF:
                geneArrayRefMips(code);
                break;
            case ARRAYASSIGN:
                geneArrayAssignMips(code);
                break;
            case ARRAYDEF:
                geneArrayDefMips(code);
                break;
            case EXPR:
                geneExprMips(code);
                break;
            case SIGNALVARASSIGN:
                geneSignalVarAssignMips(code);
                break;
            case JUMP:
                geneJumpMips(code);
                break;
            case DIRJUMP:
                geneDirJumpMips(code);
                break;
            case CONDEF:
                geneConDefMips(code);
                break;
            case VARDEF:
                geneVarDefMips(code);
                break;
            case CALLFUNC:
                geneCallFuncMips(code);
                break;
            case LABEL:
                geneLabelMips(code);
                break;
            case RETURN:
                geneReturnMips(code);
                break;
            case SCANF:
                geneScanfMips(code);
                break;
            case PRINTF:
                genePrintfMips(code);
                break;
            case ENDFUNC:
                geneEndFuncMips();
                break;
            case FUNCDEF:
                if (this->firstFunc) {
                    this->firstFunc = false;
                    this->output->outMips(
                            "addi $fp, $fp, " + std::to_string(this->tempVarTable->getCurAddr()));
                    this->output->outMips("jal main");
                    geneExitMips();
                }
                geneFuncDefMips(code);
                break;
            case INITREGS:
                geneInitRegsMips();
                break;
            default:
                break;
        }
    }

}