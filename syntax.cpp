#include "syntax.h"


SyntaxParse::SyntaxParse(std::string &file_path, Output *output, MidCode *midCode) {
    this->output = output;
    this->midCode = midCode;
    this->symbolTable = new SymbolTable();
    this->errorHandler = new ErrorHandler(this->symbolTable, this->output);
    this->tokenParse = new TokenParse(file_path, this->errorHandler);
}

SyntaxParse::~SyntaxParse() {

}

void SyntaxParse::checkSyntax() {
    getSym();
    program();
}

Symbol SyntaxParse::getPreSym(int id) {
    int size = this->preTokens.size();
    if (size < id) {
        for (int i = 0; i < id - size; i++) {
            this->preTokens.push_back(this->tokenParse->getToken());
        }
    }
    return this->preTokens[id - 1].sym;
}

void SyntaxParse::getSym() {
    if (this->preTokens.empty()) {
        this->token = this->tokenParse->getToken();
        this->sym = this->token.sym;
    } else {
        this->token = this->preTokens.front();
        this->preTokens.erase(this->preTokens.begin());
        this->sym = this->token.sym;
    }

    this->output->outSyntax(this->token.symbol + " " + this->token.tok);
}

void SyntaxParse::restract() {
    this->preTokens.insert(this->preTokens.begin(), this->token);
}

void SyntaxParse::program() {
    if (this->sym == CONSTTK) {
        constDeclare();
        getSym();
    }
    if ((this->sym == INTTK || this->sym == CHARTK) && getPreSym(1) == IDENFR && getPreSym(2) != LPARENT) {
        variableDeclare();
        getSym();
    }
    if ((this->sym == INTTK || this->sym == CHARTK) || (this->sym == VOIDTK && getPreSym(1) == IDENFR)) {
        while ((this->sym == INTTK || this->sym == CHARTK) || (this->sym == VOIDTK && getPreSym(1) == IDENFR)) {
            if (this->sym == INTTK || this->sym == CHARTK) {
                returnedFuncDefine();
                getSym();
            } else {
                unreturnedFuncDefine();
                getSym();
            }
        }
    }

    mainFunc();

    this->output->outSyntax("<程序>");
}

void SyntaxParse::mainFunc() {
    this->funcType = UNRETURNEDFUNC;
    // void
    getSym();
    // main
    std::string func = this->token.tok;
    std::string label = this->token.tok;
    std::vector<std::string> params;
    std::vector<SyntaxType> paramsType;
    this->midCode->geneFuncDefMidCode(func, params, paramsType);
    this->midCode->geneLabelMidCode(label);
    getSym();
    // (
    getSym();
    if (this->sym != RPARENT) {
        this->errorHandler->notParentError();
        restract();
    }
    getSym();
    // {
    this->symbolTable->addScope();
    if (getPreSym(1) == RBRACE)
        this->sym = NON;
    else
        getSym();
    compoundStatement();
    getSym();
    // }
    this->symbolTable->decreaseScope();

    this->output->outSyntax("<主函数>");
}

bool SyntaxParse::unsignedInt() {
    bool isUnsignedInt = true;
    if (this->sym != INTCON) {
        isUnsignedInt = false;
    }

    this->output->outSyntax("<无符号整数>");
    return isUnsignedInt;
}

bool SyntaxParse::intcon() {
    bool isIntCon;
    bool isMinu = false;
    if (this->sym == PLUS || this->sym == MINU) {
        if (this->sym == MINU)
            isMinu = true;
        getSym();
    }
    isIntCon = unsignedInt();
    if (isIntCon) {
        this->conVal = this->token.tok;
        if (isMinu)
            this->conVal = "-" + this->conVal;
    }
    this->output->outSyntax("<整数>");

    return isIntCon;
}

bool SyntaxParse::strcon() {
    bool isStrCon = true;
    if (this->sym != STRCON) {
        isStrCon = false;
    }

    this->output->outSyntax("<字符串>");
    return isStrCon;
}

void SyntaxParse::declareHeader() {
    // int or char
    getSym();
    // identifier

    this->output->outSyntax("<声明头部>");
}

void SyntaxParse::constDeclare() {
    // const
    getSym();
    constDefine();
    getSym();
    if (this->sym != SEMICN) {
        this->errorHandler->notSemiError();
        restract();
    }
    while (getPreSym(1) == CONSTTK) {
        getSym();
        // const
        getSym();
        constDefine();
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
    }

    this->output->outSyntax("<常量说明>");
}

void SyntaxParse::constDefine() {
    if (this->sym == INTTK) {
        getSym();
        // identifier
        this->errorHandler->identifierDupError(this->token.tok, CONSTINT);
        // cur identifier name
        std::string var = this->token.tok;
        getSym();
        // =
        getSym();
        bool isIntCon = intcon();
        if (!isIntCon) {
            this->errorHandler->constDefineError();
        } else {
            this->midCode->geneConDefMidCode(CONSTINT, var, this->conVal);
        }
        while (getPreSym(1) == COMMA) {
            getSym();
            // ,
            getSym();
            // identifier
            this->errorHandler->identifierDupError(this->token.tok, CONSTINT);
            var = this->token.tok;
            getSym();
            // =
            getSym();
            isIntCon = intcon();
            if (!isIntCon) {
                this->errorHandler->constDefineError();
            } else {
                this->midCode->geneConDefMidCode(CONSTINT, var, this->conVal);
            }
        }
    } else if (this->sym == CHARTK) {
        getSym();
        // identifier
        this->errorHandler->identifierDupError(this->token.tok, CONSTCHAR);
        std::string var = this->token.tok;
        getSym();
        // =
        getSym();
        if (this->sym != CHARCON)
            this->errorHandler->constDefineError();
        else {
            this->conVal = "'" + this->token.tok + "'";
            this->midCode->geneConDefMidCode(CONSTCHAR, var, this->conVal);
        }
        while (getPreSym(1) == COMMA) {
            getSym();
            // ,
            getSym();
            // identifier
            this->errorHandler->identifierDupError(this->token.tok, CONSTCHAR);
            var = this->token.tok;
            getSym();
            // =
            getSym();
            if (this->sym != CHARCON)
                this->errorHandler->constDefineError();
            else {
                this->conVal = "'" + this->token.tok + "'";
                this->midCode->geneConDefMidCode(CONSTCHAR, var, this->conVal);
            }
        }
    }

    this->output->outSyntax("<常量定义>");
}

void SyntaxParse::variableDeclare() {
    variableDefine();
    getSym();
    if (this->sym != SEMICN) {
        this->errorHandler->notSemiError();
        restract();
    }
    while ((getPreSym(1) == INTTK || getPreSym(1) == CHARTK) && getPreSym(3) != LPARENT) {
        getSym();
        variableDefine();
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
    }

    this->output->outSyntax("<变量说明>");
}

void SyntaxParse::variableDefine() {
    // int or char
    Symbol symbol = this->sym;
    getSym();
    // identifier
    std::string var = this->token.tok;
    if (getPreSym(1) == LBRACK) {
        if (symbol == INTTK)
            this->errorHandler->identifierDupError(this->token.tok, INTARRAY);
        else
            this->errorHandler->identifierDupError(this->token.tok, CHARARRAY);
    } else {
        if (symbol == INTTK)
            this->errorHandler->identifierDupError(this->token.tok, INT);
        else
            this->errorHandler->identifierDupError(this->token.tok, CHAR);
    }
    if (getPreSym(1) == LBRACK) {
        getSym();
        // [
        getSym();
        bool isUnsignedInt = unsignedInt();
        if (!isUnsignedInt)
            this->errorHandler->inValidArrayIndexError();
        else {
            if (symbol == INTTK)
                this->midCode->geneArrayDefMidCode(INT, var, std::stoi(this->token.tok));
            else
                this->midCode->geneArrayDefMidCode(CHAR, var, std::stoi(this->token.tok));
        }
        getSym();
        if (this->sym != RBRACK) {
            this->errorHandler->notBracketError();
            restract();
        }
    } else {
        if (symbol == INTTK)
            this->midCode->geneVarDefMidCode(INT, var);
        else
            this->midCode->geneVarDefMidCode(CHAR, var);
    }
    while (getPreSym(1) == COMMA) {
        getSym();
        // ,
        getSym();
        // identifier
        var = this->token.tok;
        if (getPreSym(1) == LBRACK) {
            if (symbol == INTTK)
                this->errorHandler->identifierDupError(this->token.tok, INT);
            else
                this->errorHandler->identifierDupError(this->token.tok, CHAR);
        } else {
            if (symbol == INTTK)
                this->errorHandler->identifierDupError(this->token.tok, INT);
            else
                this->errorHandler->identifierDupError(this->token.tok, CHAR);
        }
        if (getPreSym(1) == LBRACK) {
            getSym();
            // [
            getSym();
            bool isUnsignedInt = unsignedInt();
            if (!isUnsignedInt)
                this->errorHandler->inValidArrayIndexError();
            else {
                if (symbol == INTTK)
                    this->midCode->geneArrayDefMidCode(INT, var, std::stoi(this->token.tok));
                else
                    this->midCode->geneArrayDefMidCode(CHAR, var, std::stoi(this->token.tok));
            }
            getSym();
            if (this->sym != RBRACK) {
                this->errorHandler->notBracketError();
                restract();
            }
        } else {
            if (symbol == INTTK)
                this->midCode->geneVarDefMidCode(INT, var);
            else
                this->midCode->geneVarDefMidCode(CHAR, var);
        }
    }

    this->output->outSyntax("<变量定义>");
}

void SyntaxParse::returnedFuncDefine() {
    // int or char
    Symbol headerType = this->sym;
    if (headerType == INTTK)
        this->funcType = INTRETURNEDFUNC;
    else
        this->funcType = CHARRETURNFUNC;
    declareHeader();
    std::string func = this->token.tok;
    std::string label = func;

    getSym();
    // (
    if (getPreSym(1) == RPARENT)
        this->sym = NON;
    else
        getSym();
    std::vector<Paramter> params = parameters();
    std::vector<SyntaxType> paramsType;
    std::vector<std::string> paramsName;
    for (Paramter &param: params) {
        paramsName.push_back(param.identifier);
        paramsType.push_back(param.syntaxType);
    }
    if (headerType == INTTK)
        this->errorHandler->identifierDupError(func, INTRETURNEDFUNC, params);
    else
        this->errorHandler->identifierDupError(func, CHARRETURNFUNC, params);
    getSym();
    if (this->sym != RPARENT) {
        this->errorHandler->notParentError();
        restract();
    }
    // gene func def mid code
    this->midCode->geneFuncDefMidCode(func, paramsName, paramsType);
    this->midCode->geneLabelMidCode(func);
    getSym();
    // {
    this->symbolTable->addScope();
    for (Paramter &param: params) {
        this->symbolTable->push(param.identifier, param.syntaxType);
    }
    if (getPreSym(1) == RBRACE)
        this->sym = NON;
    else
        getSym();
    bool haveReturn = compoundStatement();
    if (!haveReturn)
        this->errorHandler->returnFuncError();
    getSym();
    // }
    this->midCode->geneEndFunc();
    this->symbolTable->decreaseScope();

    this->output->outSyntax("<有返回值函数定义>");
}

void SyntaxParse::unreturnedFuncDefine() {
    this->funcType = UNRETURNEDFUNC;
    // void
    getSym();
    // identifier
    std::string func = this->token.tok;
    std::string label = func;
    getSym();
    // (
    if (getPreSym(1) == RPARENT)
        this->sym = NON;
    else
        getSym();
    std::vector<Paramter> params = parameters();
    std::vector<SyntaxType> paramsType;
    std::vector<std::string> paramsName;
    for (Paramter &param: params) {
        paramsType.push_back(param.syntaxType);
        paramsName.push_back(param.identifier);
    }
    this->errorHandler->identifierDupError(func, UNRETURNEDFUNC, params);
    getSym();
    if (this->sym != RPARENT) {
        this->errorHandler->notParentError();
        restract();
    }
    this->midCode->geneFuncDefMidCode(func, paramsName, paramsType);
    this->midCode->geneLabelMidCode(label);
    getSym();
    // {
    this->symbolTable->addScope();
    for (Paramter &param: params) {
        this->symbolTable->push(param.identifier, param.syntaxType);
    }
    if (getPreSym(1) == RBRACE)
        this->sym = NON;
    else
        getSym();
    compoundStatement();
    getSym();
    // }
    this->midCode->geneEndFunc();
    this->symbolTable->decreaseScope();

    this->output->outSyntax("<无返回值函数定义>");
}

bool SyntaxParse::statement() {
    bool haveReturn = false;
    if (this->sym == IFTK) {
        bool hasReturn = ifStatement();
        haveReturn = hasReturn || haveReturn;
    } else if (this->sym == WHILETK || this->sym == FORTK || this->sym == DOTK) {
        bool hasReturn = forStatement();
        haveReturn = hasReturn || haveReturn;
    } else if (this->sym == LBRACE) {
        if (getPreSym(1) == RBRACE)
            this->sym = NON;
        else
            getSym();
        bool hasReturn = statements();
        haveReturn = haveReturn || hasReturn;
        getSym();
        // }
    } else if (this->sym == IDENFR && getPreSym(1) == LPARENT) {
        std::string var;
        callFuncStatement(&var);
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
    } else if (this->sym == IDENFR) {
        assignStatement();
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
    } else if (this->sym == SCANFTK) {
        scanfStatement();
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
    } else if (this->sym == PRINTFTK) {
        printfStatement();
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
    } else if (this->sym == RETURNTK) {
        bool hasReturn = returnStatement();
        haveReturn = haveReturn || hasReturn;
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
    } else if (this->sym == SEMICN) {

    } else if (this->sym == NON) {

    } else {
        this->errorHandler->notSemiError();
        restract();
    }

    this->output->outSyntax("<语句>");
    return haveReturn;
}

bool SyntaxParse::compoundStatement() {
    if (this->sym == CONSTTK) {
        constDeclare();
        if (getPreSym(1) == RBRACE)
            this->sym = NON;
        else
            getSym();
    }
    if ((this->sym == INTTK || this->sym == CHARTK) && getPreSym(1) == IDENFR && getPreSym(2) != LPARENT) {
        variableDeclare();
        if (getPreSym(1) == RBRACE)
            this->sym = NON;
        else
            getSym();
    }
    bool haveReturn = statements();

    this->output->outSyntax("<复合语句>");
    return haveReturn;
}

bool SyntaxParse::statements() {
    Symbol symbol;
    symbol = this->sym;
    bool haveReturn = false;
    while (symbol == IFTK || symbol == FORTK || symbol == WHILETK || symbol == DOTK || symbol == LBRACE ||
           symbol == IDENFR || symbol == SCANFTK || symbol == PRINTFTK || symbol == RETURNTK || symbol == SEMICN) {
        bool hasReturn = statement();
        haveReturn = haveReturn || hasReturn;
        if (getPreSym(1) == RBRACE)
            break;
        getSym();
        symbol = this->sym;
    }

    this->output->outSyntax("<语句列>");
    return haveReturn;
}

bool SyntaxParse::forStatement() {
    this->midCode->resetVar();
    bool haveReturn = false;
    std::string label1;
    std::string label2;
    if (this->sym == WHILETK) {
        getSym();
        label1 = this->midCode->getWhileLabel();
        label2 = this->midCode->getEndWhileLabel();
        this->midCode->geneInitRegs();
        this->midCode->geneLabelMidCode(label1);

        // (
        getSym();
        qualification(label2, true);

        getSym();
        if (this->sym != RPARENT) {
            this->errorHandler->notParentError();
            restract();
        }
        getSym();
        bool hasReturn = statement();
        this->midCode->geneInitRegs();
        this->midCode->geneDirJumpMidCode(label1);
        this->midCode->geneLabelMidCode(label2);
        haveReturn = haveReturn || hasReturn;
    } else if (this->sym == DOTK) {
        label1 = this->midCode->getDoLabel();
        label2 = this->midCode->getEndDoLabel();
        this->midCode->geneInitRegs();
        this->midCode->geneLabelMidCode(label1);
        getSym();
        bool hasReturn = statement();
        haveReturn = haveReturn || hasReturn;
        getSym();
        if (this->sym != WHILETK) {
            this->errorHandler->lackWhileError();
            restract();
        }
        getSym();
        // (
        getSym();
        qualification(label2, true);
        this->midCode->geneDirJumpMidCode(label1);
        this->midCode->geneLabelMidCode(label2);
        getSym();
        if (this->sym != RPARENT) {
            this->errorHandler->notParentError();
            restract();
        }
    } else if (this->sym == FORTK) {
        label1 = this->midCode->getForLabel();
        label2 = this->midCode->getEndForLabel();
        getSym();
        // (
        getSym();
        // identifier
        this->errorHandler->identifierNotDefError(this->token.tok);
        this->errorHandler->changeConstError(this->token.tok);
        std::string var = this->token.tok;
        getSym();
        // =
        getSym();
        std::string expr_var;
        expression(&expr_var);
        this->midCode->geneSignalVarAssignMidCode(var, expr_var, INT);
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
        this->midCode->geneInitRegs();
        this->midCode->geneLabelMidCode(label1);
        getSym();
        qualification(label2, true);
        getSym();
        if (this->sym != SEMICN) {
            this->errorHandler->notSemiError();
            restract();
        }
        getSym();
        // identifier
        var = this->token.tok;
        this->errorHandler->identifierNotDefError(this->token.tok);
        this->errorHandler->changeConstError(this->token.tok);
        getSym();
        // =
        getSym();
        // identifier
        std::string var1 = this->token.tok;
        this->errorHandler->identifierNotDefError(this->token.tok);
        getSym();
        // + or -
        Symbol symbol = this->sym;
        getSym();
        std::string var2;
        step(&var2);

        getSym();
        if (this->sym != RPARENT) {
            this->errorHandler->notParentError();
            restract();
        }
        getSym();
        bool hasReturn = statement();

        if (symbol == PLUS)
            this->midCode->geneExprMidCode(var, INT, var1, var2, "+");
        else
            this->midCode->geneExprMidCode(var, INT, var1, var2, "-");
        this->midCode->geneInitRegs();
        this->midCode->geneDirJumpMidCode(label1);
        this->midCode->geneLabelMidCode(label2);
        haveReturn = haveReturn || hasReturn;
    }

    this->output->outSyntax("<循环语句>");
    return haveReturn;
}

void SyntaxParse::assignStatement() {
    this->midCode->resetVar();
    if (this->sym == IDENFR && getPreSym(1) == LBRACK) {
        std::string arrVar = this->token.tok;
        // identifier
        this->errorHandler->identifierNotDefError(this->token.tok);
        this->errorHandler->changeConstError(this->token.tok);
        getSym();
        // [
        getSym();
        std::string index_var;
        bool isInt = expression(&index_var);
        if (!isInt)
            this->errorHandler->inValidArrayIndexError();
        getSym();
        if (this->sym != RBRACK) {
            this->errorHandler->notBracketError();
            restract();
        }
        getSym();
        // =
        getSym();
        std::string expr_var;
        expression(&expr_var);
        this->midCode->geneArrayAssignMidCode(expr_var, arrVar, index_var);
    } else if (this->sym == IDENFR) {
        std::string var = this->token.tok;
        // identifier
        this->errorHandler->identifierNotDefError(this->token.tok);
        this->errorHandler->changeConstError(this->token.tok);
        getSym();
        // =
        getSym();
        std::string expr_var;
        expression(&expr_var);
        this->midCode->geneSignalVarAssignMidCode(var, expr_var);
    }

    this->output->outSyntax("<赋值语句>");
}

bool SyntaxParse::ifStatement() {
    this->midCode->resetVar();
    std::string label1;
    std::string label2;
    bool haveReturn = false;
    // if
    getSym();
    // (
    label1 = this->midCode->getElseLabel();
    label2 = this->midCode->getEndIfLabel();
    getSym();
    std::string label = this->midCode->getIfLabel();

    this->midCode->geneLabelMidCode(label);
    qualification(label1, true);
    getSym();
    if (this->sym != RPARENT) {
        this->errorHandler->notParentError();
        restract();
    }
    getSym();
    bool hasReturn = statement();
    haveReturn = haveReturn || hasReturn;
    if (getPreSym(1) == ELSETK) {
        getSym();
        // else
        this->midCode->geneInitRegs();
        this->midCode->geneDirJumpMidCode(label2);
        this->midCode->geneLabelMidCode(label1);
        getSym();
        hasReturn = statement();
        this->midCode->geneInitRegs();
        this->midCode->geneLabelMidCode(label2);
        haveReturn = haveReturn || hasReturn;
    } else {
        this->midCode->geneInitRegs();
        this->midCode->geneLabelMidCode(label1);

        this->midCode->geneLabelMidCode(label2);
    }

    this->output->outSyntax("<条件语句>");
    return haveReturn;
}

void SyntaxParse::callFuncStatement(std::string *var) {
    std::string new_var;
    std::string cur_var;
    // identifier
    this->errorHandler->identifierNotDefError(this->token.tok);
    std::string func = this->token.tok;
    getSym();
    // (
    if (getPreSym(1) == RPARENT)
        this->sym = NON;
    else
        getSym();
    std::vector<Paramter> params = valueParm();
    std::vector<SyntaxType> paramsType;
    std::vector<std::string> vars;
    std::vector<std::string> funcParams;
    funcParams = this->symbolTable->getFuncParams(func);
    for (Paramter &param: params) {
        paramsType.push_back(param.syntaxType);
        vars.push_back(param.identifier);
    }
    new_var = this->midCode->getVar();
    if (this->symbolTable->varIsInt(func)) {
        this->midCode->geneCallFuncMidCode(func, vars, funcParams, new_var, INT);
    } else {
        this->midCode->geneCallFuncMidCode(func, vars, funcParams, new_var, CHAR);
    }
    cur_var = new_var;
    this->errorHandler->funcParamError(func, paramsType);
    getSym();
    if (this->sym != RPARENT) {
        this->errorHandler->notParentError();
        restract();
    }
    if (this->symbolTable->funcIsReturn(func)) {
        this->output->outSyntax("<有返回值函数调用语句>");
    } else {
        this->output->outSyntax("<无返回值函数调用语句>");
    }
    *var = cur_var;
}

void SyntaxParse::scanfStatement() {
    std::vector<std::string> vars;
    // scanf
    getSym();
    // (
    getSym();
    // identifier
    vars.push_back(this->token.tok);
    this->errorHandler->identifierNotDefError(this->token.tok);
    while (getPreSym(1) == COMMA) {
        getSym();
        // ,
        getSym();
        // identifier
        vars.push_back(this->token.tok);
        this->errorHandler->identifierNotDefError(this->token.tok);
    }
    getSym();
    if (this->sym != RPARENT) {
        this->errorHandler->notParentError();
        restract();
    }
    this->midCode->geneScanfMidCode(vars);

    this->output->outSyntax("<读语句>");
}

void SyntaxParse::printfStatement() {
    this->midCode->resetVar();
    // printf
    getSym();
    // (
    getSym();
    if (this->sym == STRCON) {
        std::string var;
        std::string s;
        strcon();
        s = this->token.tok;
        var = this->midCode->pushStringVar(s);
        if (getPreSym(1) == COMMA) {
            getSym();
            // ,
            getSym();
            std::string expr_var;
            expression(&expr_var);
            this->midCode->genePrintfMidCode(var, expr_var);
        } else {
            this->midCode->genePrintfMidCode(var, false);
        }
    } else {
        std::string expr_var;
        expression(&expr_var);
        this->midCode->genePrintfMidCode(expr_var, true);
    }
    getSym();
    if (this->sym != RPARENT) {
        this->errorHandler->notParentError();
        restract();
    }

    this->output->outSyntax("<写语句>");
}

bool SyntaxParse::returnStatement() {
    this->midCode->resetVar();
    bool isHaveValue = false;
    bool isIntExpr = false;
    // return
    if (getPreSym(1) == LPARENT) {
        isHaveValue = true;
        getSym();
        // (
        getSym();
        std::string expr_var;
        isIntExpr = expression(&expr_var);
        this->midCode->geneReturnMidCode(expr_var);
        getSym();
        if (this->sym != RPARENT) {
            this->errorHandler->notParentError();
            restract();
        }
    } else {
        this->midCode->geneReturnMidCode();
    }
    if (this->funcType == INTRETURNEDFUNC) {
        if (!isHaveValue || !isIntExpr)
            this->errorHandler->returnFuncError();
    } else if (this->funcType == CHARRETURNFUNC) {
        if (!isHaveValue || isIntExpr)
            this->errorHandler->returnFuncError();
    } else {
        if (isHaveValue)
            this->errorHandler->unreturnFuncError();
    }

    this->output->outSyntax("<返回语句>");
    return true;
}

bool SyntaxParse::expression(std::string *var) {

    std::string new_var;
    // a var used to express current expr
    std::string cur_var;
    // a temp var
    std::string v;
    bool isIntExpr = false;
    bool isMinu = false;
    if (this->sym == PLUS || this->sym == MINU) {
        if (this->sym == MINU)
            isMinu = true;
        getSym();
        isIntExpr = true;
    }
    std::string item_var;
    bool isCon1;
    bool isCon2;
    bool isIntItem = item(&item_var, &isCon1);
    cur_var = item_var;

    if (isMinu) {
        v = this->midCode->getConVar();
        this->midCode->geneConAssignMidCode(v, CONSTINT, "0");
        if (cur_var.at(0) == '@' && !isCon1) {
            new_var = cur_var;
        } else {
            new_var = this->midCode->getVar();
        }
        this->midCode->geneExprMidCode(new_var, INT, v, cur_var, "-");
        cur_var = new_var;
    }

    isIntExpr = isIntExpr || isIntItem;
    while (getPreSym(1) == PLUS || getPreSym(1) == MINU) {
        getSym();
        // + or -
        Symbol symbol = this->sym;
        getSym();
        item(&item_var, &isCon2);
        isIntExpr = true;
        if (!isCon1 && cur_var.at(0) == '@') {
            new_var = cur_var;
        } else if (!isCon2 && item_var.at(0) == '@') {
            new_var = item_var;
        } else {
            new_var = this->midCode->getVar();
        }
        isCon1 = false;
        if (symbol == PLUS)
            this->midCode->geneExprMidCode(new_var, INT, cur_var, item_var, "+");
        else
            this->midCode->geneExprMidCode(new_var, INT, cur_var, item_var, "-");
        cur_var = new_var;
    }

    *var = cur_var;
    this->output->outSyntax("<表达式>");
    return isIntExpr;
}

bool SyntaxParse::item(std::string *var, bool *isCon) {
    std::string new_var;
    std::string cur_var;
    std::string v;
    std::string factor_var;
    bool isIntItem = false;
    bool isCon1;
    bool isCon2;
    isIntItem = factor(&factor_var, &isCon1);
    cur_var = factor_var;
    *isCon = isCon1;
    while (getPreSym(1) == MULT || getPreSym(1) == DIV) {
        *isCon = false;
        getSym();
        // * or /
        Symbol symbol = this->sym;
        getSym();
        factor(&factor_var, &isCon2);
        isIntItem = true;
        if (cur_var.at(0) == '@' && !isCon1) {
            new_var = cur_var;
        } else if (factor_var.at(0) == '@' && !isCon2) {
            new_var = factor_var;
        } else {
            new_var = this->midCode->getVar();
        }
        isCon1 = false;
        if (symbol == MULT)
            this->midCode->geneExprMidCode(new_var, INT, cur_var, factor_var, "*");
        else
            this->midCode->geneExprMidCode(new_var, INT, cur_var, factor_var, "/");
        cur_var = new_var;
    }
    *var = cur_var;
    this->output->outSyntax("<项>");
    return isIntItem;
}

bool SyntaxParse::factor(std::string *var, bool *isCon) {
    bool isIntFactor = false;
    std::string new_var;
    std::string cur_var;
    if (this->sym == IDENFR && getPreSym(1) == LBRACK) {
        this->errorHandler->identifierNotDefError(this->token.tok);
        if (this->symbolTable->varIsInt(this->token.tok))
            isIntFactor = true;
        std::string arrName = this->token.tok;
        getSym();
        // [
        getSym();
        std::string indexVar;
        bool isInt = expression(&indexVar);
        if (!isInt)
            this->errorHandler->inValidArrayIndexError();
        getSym();
        if (this->sym != RBRACK) {
            this->errorHandler->notBracketError();
            restract();
        }
        new_var = this->midCode->getVar();
        if (isIntFactor)
            this->midCode->geneArrayRefMidCode(new_var, arrName, indexVar,INT);
        else
            this->midCode->geneArrayRefMidCode(new_var, arrName, indexVar, CHAR);
        cur_var = new_var;
        *isCon = false;
    } else if (this->sym == LPARENT) {
        getSym();
        std::string exprVar;
        expression(&exprVar);
        isIntFactor = true;
        getSym();
        if (this->sym != RPARENT) {
            this->errorHandler->notParentError();
            restract();
        }
        cur_var = exprVar;
        *isCon = false;
    } else if (this->sym == CHARCON) {
        new_var = this->midCode->getConVar();
        this->midCode->geneConAssignMidCode(new_var, CONSTCHAR, "'" + this->token.tok + "'");
        cur_var = new_var;
        *isCon = true;
    } else if (this->sym == IDENFR && getPreSym(1) == LPARENT) {
        if (this->symbolTable->varIsInt(this->token.tok))
            isIntFactor = true;
        new_var = this->midCode->getVar();
        callFuncStatement(&new_var);
        cur_var = new_var;
        *isCon = false;
    } else if (this->sym == IDENFR) {
        this->errorHandler->identifierNotDefError(this->token.tok);
        if (this->symbolTable->varIsInt(this->token.tok))
            isIntFactor = true;
        cur_var = this->token.tok;
        *isCon = false;
    } else if (this->sym == PLUS || this->sym == MINU || this->sym == INTCON) {
        isIntFactor = intcon();
        new_var = this->midCode->getConVar();
        this->midCode->geneConAssignMidCode(new_var, CONSTINT, this->conVal);
        cur_var = new_var;
        *isCon = true;
    } else if (this->sym == NON) {

    }

    *var = cur_var;
    this->output->outSyntax("<因子>");
    return isIntFactor;
}

std::vector<Paramter> SyntaxParse::parameters() {
    std::vector<Paramter> params;
    Paramter param;
    if (this->sym == INTTK || this->sym == CHARTK) {
        if (this->sym == INTTK)
            param.syntaxType = INT;
        else
            param.syntaxType = CHAR;
        getSym();
        // identifier
        param.identifier = this->token.tok;
        params.push_back(param);
        while (getPreSym(1) == COMMA) {
            getSym();
            // ,
            getSym();
            // int or char
            if (this->sym == INTTK)
                param.syntaxType = INT;
            else
                param.syntaxType = CHAR;
            getSym();
            // identifier
            param.identifier = this->token.tok;
            params.push_back(param);
        }
    }

    this->output->outSyntax("<参数表>");
    return params;
}

void SyntaxParse::qualification(std::string &label, bool isInitReg) {
    std::string new_var;
    std::string cur_var;
    std::string expr_var;
    bool isIntExpr = expression(&expr_var);
    cur_var = expr_var;
    Symbol symbol;
    symbol = getPreSym(1);
    if (symbol == LSS || symbol == LEQ || symbol == GRE || symbol == GEQ || symbol == NEQ || symbol == EQL) {
        getSym();
        getSym();
        isIntExpr = expression(&expr_var) && isIntExpr;
        switch (symbol) {
            case LSS:
                this->midCode->geneJumpMidCode(cur_var, expr_var, ">=", label, isInitReg);
                break;
            case LEQ:
                this->midCode->geneJumpMidCode(cur_var, expr_var, ">", label, isInitReg);
                break;
            case GRE:
                this->midCode->geneJumpMidCode(cur_var, expr_var, "<=", label, isInitReg);
                break;
            case GEQ:
                this->midCode->geneJumpMidCode(cur_var, expr_var, "<", label, isInitReg);
                break;
            case EQL:
                this->midCode->geneJumpMidCode(cur_var, expr_var, "!=", label, isInitReg);
                break;
            case NEQ:
                this->midCode->geneJumpMidCode(cur_var, expr_var, "==", label, isInitReg);
                break;
            default:
                break;
        }
    } else {
        new_var = this->midCode->getConVar();
        this->midCode->geneConAssignMidCode(new_var, CONSTINT, "0");
        this->midCode->geneJumpMidCode(cur_var, new_var, "<=", label, isInitReg);
    }
    if (!isIntExpr)
        this->errorHandler->inValidQualificationError();
    this->output->outSyntax("<条件>");
}

void SyntaxParse::step(std::string *var) {
    std::string cur_var;
    cur_var = this->midCode->getConVar();
    unsignedInt();
    this->midCode->geneConAssignMidCode(cur_var, CONSTINT, this->token.tok);
    *var = cur_var;

    this->output->outSyntax("<步长>");
}

std::vector<Paramter> SyntaxParse::valueParm() {
    std::string expr_var;
    std::vector<Paramter> params;
    if (this->sym != NON) {
        bool isIntExpr = expression(&expr_var);
        if (isIntExpr) {
            Paramter param;
            param.identifier = expr_var;
            param.syntaxType = INT;
            params.push_back(param);
        }
        else {
            Paramter param;
            param.identifier = expr_var;
            param.syntaxType = CHAR;
            params.push_back(param);
        }
        while (getPreSym(1) == COMMA) {
            getSym();
            // ,
            getSym();
            isIntExpr = expression(&expr_var);
            if (isIntExpr) {
                Paramter param;
                param.identifier = expr_var;
                param.syntaxType = INT;
                params.push_back(param);
            }
            else {
                Paramter param;
                param.identifier = expr_var;
                param.syntaxType = CHAR;
                params.push_back(param);
            }
        }
    }

    this->output->outSyntax("<值参数表>");
    return params;
}