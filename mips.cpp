#include "mips.h"


Mips::Mips(Output *output, MidCode *midCode) {
    this->output = output;
    this->midCode = midCode;
    this->tempVarTable = new TempVarTable();
}

Mips::~Mips() {}

void Mips::geneExitMips() {
    this->output->outMips("li $v0, 10");
    this->output->outMips("syscall");
}

void Mips::geneMove(const std::string &reg1, const std::string &reg2) {
    this->output->outMips("move " + reg1 + ", " + reg2);
}

void Mips::printEnter() {
    std::string c = "'\\n'";
    geneLi("$a0", c);
    geneLi("$v0", "11");
    this->output->outMips("syscall");
}

void Mips::printString(std::string &s) {
    this->output->outMips("la $a0, " + s);
    geneLi("$v0", "4");
    this->output->outMips("syscall");
}

void Mips::printVar(SyntaxType syntaxType) {
    if (syntaxType == INT || syntaxType == CONSTINT) {
        geneLi("$v0", "1");
    } else {
        geneLi("$v0", "11");
    }
    this->output->outMips("syscall");
}

void Mips::geneLoad(const std::string &reg, const std::string &addr, bool isGlobal, SyntaxType syntaxType) {
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

void Mips::geneLoadByReg(const std::string &reg, const std::string &addrReg, bool isGlobal, SyntaxType syntaxType) {
    if (isGlobal) {
        geneAddSub("$s0", addrReg, "$sp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("lw " + reg + ", " + "0($s0)");
        } else {
            this->output->outMips("lb " + reg + ", " + "0($s0)");
        }
    } else {
        geneAddSub("$s0", addrReg, "$fp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("lw " + reg + ", " + "0($s0)");
        } else {
            this->output->outMips("lb " + reg + ", " + "0($s0)");
        }
    }
}

void Mips::geneStore(const std::string &reg, const std::string &addr, bool isGlobal, SyntaxType syntaxType) {
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

void Mips::geneStoreByReg(const std::string &reg, const std::string &addrReg, bool isGlobal, SyntaxType syntaxType) {
    if (isGlobal) {
        geneAddSub("$s0", addrReg, "$sp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("sw " + reg + ", " + "0($s0)");
        } else {
            this->output->outMips("sb " + reg + ", " + "0($s0)");
        }
    } else {
        geneAddSub("$s0", addrReg, "$fp", "+");
        if (syntaxType == INT || syntaxType == CONSTINT) {
            this->output->outMips("sw " + reg + ", " + "0($s0)");
        } else {
            this->output->outMips("sb " + reg + ", " + "0($s0)");
        }
    }
}

void Mips::geneStoreFunc(const std::string &reg, const std::string &addr, const std::string &addrReg,
                     SyntaxType syntaxType) {
    if (syntaxType == INT || syntaxType == CONSTINT) {
        this->output->outMips("sw " + reg + ", " + addr + "(" + addrReg + ")");
    } else {
        this->output->outMips("sb " + reg + ", " + addr + "(" + addrReg + ")");
    }
}

void Mips::geneLi(const std::string &reg, const std::string &val) {
    this->output->outMips("li " + reg + ", " + val);
}

void Mips::geneAddSub(const std::string &resReg, const std::string &reg1, const std::string &reg2, const std::string &op) {
    if (op == "+")
        this->output->outMips("add " + resReg + ", " + reg1 + ", " + reg2);
    else
        this->output->outMips("sub " + resReg + ", " + reg1 + ", " + reg2);
}

void Mips::geneMulDiv(const std::string &reg1, const std::string &reg2, std::string op) {
    if (op == "*")
        this->output->outMips("mult " + reg1 + ", " + reg2);
    else
        this->output->outMips("div " + reg1 + ", " + reg2);
}

void Mips::geneJ(const std::string &label) {
    this->output->outMips("j " + label);
}

void Mips::geneJar(std::string &func) {
    this->output->outMips("jal " + func);
}

void Mips::geneJr() {
    this->output->outMips("jr $ra");
}

void Mips::geneLabel(std::string &label) {
    this->output->outMips(label + ":");
}

void Mips::geneJump(std::string &label, const std::string &reg1, const std::string &reg2, std::string &op) {
    if (op == ">=") {
        this->output->outMips("sub $s0, " + reg1 + ", " + reg2);
        this->output->outMips("bgez $s0, " + label);
    } else if (op == ">") {
        this->output->outMips("sub $s0, " + reg1 + ", " + reg2);
        this->output->outMips("bgtz $s0, " + label);
    } else if (op == "<=") {
        this->output->outMips("sub $s0, " + reg1 + ", " + reg2);
        this->output->outMips("blez $s0, " + label);
    } else if (op == "<") {
        this->output->outMips("sub $s0, " + reg1 + ", " + reg2);
        this->output->outMips("bltz $s0, " + label);
    } else if (op == "==") {
        this->output->outMips("beq " + reg1 + ", " + reg2 + ", " + label);
    } else {
        this->output->outMips("bne " + reg1 + ", " + reg2 + ", " + label);
    }
}

void Mips::geneMflo(const std::string &reg) {
    this->output->outMips("mflo " + reg);
}

void Mips::geneRead(SyntaxType syntaxType) {
    if (syntaxType == INT) {
        geneLi("$v0", "5");
        this->output->outMips("syscall");
    } else {
        geneLi("$v0", "12");
        this->output->outMips("syscall");
    }
}

void Mips::geneArrayAssignMips(MdCode &mdCode) {
    ArrayAssignMidCode *code = (ArrayAssignMidCode*)mdCode.p;
    TempVar tempVar = this->tempVarTable->getVarAddr(code->var);
    TempVar tempArrVar = this->tempVarTable->getVarAddr(code->arrayVar);
    TempVar tempIndexVar = this->tempVarTable->getVarAddr(code->indexVar);
    // $t0 is addr of array head
    geneLi("$t0", tempArrVar.addr);
    geneLoad("$t1", tempIndexVar.addr, tempIndexVar.isGlobal, tempIndexVar.syntaxType);
    geneLi("$t2", "4");
    geneMulDiv("$t1", "$t2", "*");
    geneMflo("$t1");
    geneAddSub("$t0", "$t0", "$t1", "+");
    geneLoad("$t1", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
    geneStoreByReg("$t1", "$t0", tempArrVar.isGlobal, tempArrVar.syntaxType);
}

void Mips::geneArrayDefMips(MdCode &mdCode) {
    ArrayDefMidCode *code = (ArrayDefMidCode*)mdCode.p;
    this->tempVarTable->pushArray(code->arrayVar, code->syntaxType, code->arrayLen);
}

void Mips::geneArrayRefMips(MdCode &mdCode) {
    ArrayRefMidCode *code = (ArrayRefMidCode*)mdCode.p;
    TempVar tempArrVar = this->tempVarTable->getVarAddr(code->arrayVar);
    TempVar tempVar = this->tempVarTable->getVarAddr(code->tarVar, code->syntaxType);
    TempVar tempIndexVar = this->tempVarTable->getVarAddr(code->indexVar);
    // $t0 is the addr of array head
    geneLi("$t0", tempArrVar.addr);
    geneLoad("$t1", tempIndexVar.addr, tempIndexVar.isGlobal, tempIndexVar.syntaxType);
    geneLi("$t2", "4");
    geneMulDiv("$t1", "$t2", "*");
    geneMflo("$t1");
    geneAddSub("$t0", "$t0", "$t1", "+");
    geneLoadByReg("$t1", "$t0", tempArrVar.isGlobal, tempArrVar.syntaxType);
    geneStore("$t1", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
}

void Mips::geneSignalVarAssignMips(MdCode &mdCode) {
    SignalVarAssignMidCode *code = (SignalVarAssignMidCode*)mdCode.p;
    TempVar tempTarVar = this->tempVarTable->getVarAddr(code->tarVar, code->syntaxType);
    TempVar tempVar = this->tempVarTable->getVarAddr(code->var);
    geneLoad("$t0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
    geneStore("$t0", tempTarVar.addr, tempTarVar.isGlobal, tempTarVar.syntaxType);
}

void Mips::geneConAssignMips(MdCode &mdCode) {
    ConAssignMidCode *code = (ConAssignMidCode*)mdCode.p;
    TempVar tempVar = this->tempVarTable->getVarAddr(code->tarVar, code->syntaxType);
    geneLi("$t0", code->val);
    geneStore("$t0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
}

void Mips::geneDirJumpMips(MdCode &mdCode) {
    DirJumpMidCode *code = (DirJumpMidCode*)mdCode.p;
    geneJ(code->label);
}

void Mips::geneJumpMips(MdCode &mdCode) {
    JumpMidCode *code = (JumpMidCode*)mdCode.p;
    TempVar tempVar1 = this->tempVarTable->getVarAddr(code->var1);
    TempVar tempVar2 = this->tempVarTable->getVarAddr(code->var2);
    geneLoad("$t0", tempVar1.addr, tempVar1.isGlobal, tempVar1.syntaxType);
    geneLoad("$t1", tempVar2.addr, tempVar2.isGlobal, tempVar2.syntaxType);
    geneJump(code->label, "$t0", "$t1", code->op);
}

void Mips::geneExprMips(MdCode &mdCode) {
    ExprMidCode *code = (ExprMidCode*)mdCode.p;
    TempVar tempVar1 = this->tempVarTable->getVarAddr(code->var1);
    TempVar tempVar2 = this->tempVarTable->getVarAddr(code->var2);
    TempVar tempTarVar = this->tempVarTable->getVarAddr(code->tarVar, code->tarType);
    geneLoad("$t0", tempVar1.addr, tempVar1.isGlobal, tempVar1.syntaxType);
    geneLoad("$t1", tempVar2.addr, tempVar2.isGlobal, tempVar2.syntaxType);
    if (code->op == "+" || code->op == "-") {
        geneAddSub("$t2", "$t0", "$t1", code->op);
    } else {
        geneMulDiv("$t0", "$t1", code->op);
        geneMflo("$t2");
    }
    geneStore("$t2", tempTarVar.addr, tempTarVar.isGlobal, tempTarVar.syntaxType);
}

void Mips::geneConDefMips(MdCode &mdCode) {
    ConDefMidCode *code = (ConDefMidCode*)mdCode.p;
    TempVar tempVar = this->tempVarTable->getVarAddr(code->var, code->syntaxType);
    geneLi("$t0", code->val);
    geneStore("$t0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
}

void Mips::geneVarDefMips(MdCode &mdCode) {
    VarDefMidCode *code = (VarDefMidCode*)mdCode.p;
    this->tempVarTable->pushVar(code->var, code->syntaxType);
}

void Mips::geneLabelMips(MdCode &mdCode) {
    LabelMidCode *code = (LabelMidCode*)mdCode.p;
    this->geneLabel(code->label);
}

void Mips::geneReturnMips(MdCode &mdCode) {
    ReturnMidCode *code = (ReturnMidCode*)mdCode.p;
    if (code->haveVar) {
        TempVar tempVar = this->tempVarTable->getVarAddr(code->var);
        geneLoad("$v0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
    }
    geneJr();
}

void Mips::geneScanfMips(MdCode &mdCode) {
    ScanfMidCode *code = (ScanfMidCode*)mdCode.p;
    for (std::string &v: code->var) {
        TempVar tempVar = this->tempVarTable->getVarAddr(v);
        geneRead(tempVar.syntaxType);
        geneStore("$v0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
    }
}

void Mips::geneFuncDefMips(MdCode &mdCode) {
    FuncDefMidCode *code = (FuncDefMidCode*)mdCode.p;
    this->tempVarTable->pushFunc(code->func, code->params, code->paramType);
}

void Mips::geneCallFuncMips(MdCode &mdCode) {
    CallFuncMidCode *code = (CallFuncMidCode*)mdCode.p;
    geneStore("$fp", std::to_string(this->tempVarTable->getCurAddr() - 4), false, INT);
    geneStore("$ra", std::to_string(this->tempVarTable->getCurAddr() - 8), false, INT);
    geneLi("$t0", std::to_string(this->tempVarTable->getCurAddr() - 8));
    geneAddSub("$t0", "$t0", "$fp", "+");
    std::vector<std::string> paramsAddr = this->tempVarTable->getFuncParamAddr(code->func);
    for (int i = 0; i < code->vars.size(); i++) {
        TempVar tempVar = this->tempVarTable->getVarAddr(code->vars.at(i));
        geneLoad("$t1", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
        geneStoreFunc("$t1", paramsAddr.at(i), "$t0", tempVar.syntaxType);
    }
    geneMove("$fp", "$t0");
    geneJar(code->func);
    geneLoad("$ra", "0", false, INT);
    geneLoad("$fp", "4", false, INT);
    TempVar tempVar = this->tempVarTable->getVarAddr(code->var);
    geneStore("$v0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
}

void Mips::genePrintfMips(MdCode &mdCode) {
    PrintfMidCode *code = (PrintfMidCode*)mdCode.p;
    if (code->isDouble) {
        printString(code->s);
        TempVar tempVar = this->tempVarTable->getVarAddr(code->var);
        geneLoad("$a0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
        printVar(tempVar.syntaxType);
        printEnter();
    } else {
        if (code->isVar) {
            TempVar tempVar = this->tempVarTable->getVarAddr(code->var);
            geneLoad("$a0", tempVar.addr, tempVar.isGlobal, tempVar.syntaxType);
            printVar(tempVar.syntaxType);
            printEnter();
        } else {
            printString(code->s);
            printEnter();
        }
    }
}

void Mips::geneEndFuncMips() {
    this->tempVarTable->decreaseScope();
    this->geneJr();
}

void Mips::geneMips() {
    std::vector<StringVar> &stringVar = this->midCode->getStringVar();
    this->output->outMips(".data");
    for (StringVar &var: stringVar) {
        this->output->outMips(var.var + ": .asciiz " + "\"" + var.s + "\"");
    }
    this->output->outMips(".text");
    std::vector<MdCode> &midCode = this->midCode->getMidCode();
    geneMove("$fp", "$sp");
    for (MdCode &code: midCode) {
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
            default:
                break;
        }
    }

}