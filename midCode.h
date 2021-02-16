#ifndef COMPILER_MIDCODE_H
#define COMPILER_MIDCODE_H


#include "stdafx.h"
#include "symbolTable.h"
#include "tempVarTable.h"


enum MidCodeType {
    CONASSIGN,
    ARRAYREF,
    ARRAYASSIGN,
    ARRAYDEF,
    EXPR,
    SIGNALVARASSIGN,
    JUMP,
    DIRJUMP,
    CONDEF,
    VARDEF,
    CALLFUNC,
    FUNCDEF,
    LABEL,
    RETURN,
    SCANF,
    PRINTF,
    ENDFUNC,
    INITREGS,
    CODEBLOCKBEGIN,
    CODEBLOCKEND
};


typedef struct {
    std::string tarVar;
    SyntaxType syntaxType;
    std::string var;
}SignalVarAssignMidCode;


typedef struct {
    std::string tarVar;
    std::string val;
    SyntaxType syntaxType;
}ConAssignMidCode;


typedef struct {
    std::string tarVar;
    SyntaxType syntaxType;
    std::string arrayVar;
    std::string indexVar;
}ArrayRefMidCode;


typedef struct {
    std::string var;
    std::string arrayVar;
    std::string indexVar;
}ArrayAssignMidCode;


typedef struct {
    SyntaxType syntaxType;
    std::string arrayVar;
    int arrayLen;
}ArrayDefMidCode;


typedef struct {
    std::string label;
}DirJumpMidCode;


typedef struct {
    std::string tarVar;
    SyntaxType tarType;
    std::string var1;
    std::string var2;
    std::string op;
}ExprMidCode;


typedef struct {
    std::string var1;
    std::string var2;
    std::string op;
    std::string label;
    bool isInitReg;
}JumpMidCode;


typedef struct {
    SyntaxType syntaxType;
    std::string var;
    std::string val;
}ConDefMidCode;


typedef struct {
    SyntaxType syntaxType;
    std::string var;
}VarDefMidCode;


typedef struct {
    std::string func;
    std::string var;
    SyntaxType syntaxType;
    std::vector<std::string> vars;
    std::vector<std::string> params;
}CallFuncMidCode;


typedef struct {
    std::string func;
    std::vector<std::string> params;
    std::vector<SyntaxType> paramType;
}FuncDefMidCode;


typedef struct {
    std::string label;
}LabelMidCode;


typedef struct {
    std::string var;
    bool haveVar;
}ReturnMidCode;


typedef struct {
    std::vector<std::string> var;
}ScanfMidCode;


typedef struct {
    std::string s;
    std::string var;
    bool isDouble;
    bool isVar;
}PrintfMidCode;


typedef struct {
    MidCodeType midCodeType;
    int *p;
}MdCode;


typedef struct {
    std::string var;
    std::string s;
}StringVar;


class MidCode {
public:
    MidCode();
    ~MidCode();

    std::string getVar();
    std::string getConVar();
    void resetVar();
    std::string getIfLabel();
    std::string getElseLabel();
    std::string getEndIfLabel();
    std::string getForLabel();
    std::string getEndForLabel();
    std::string getWhileLabel();
    std::string getEndWhileLabel();
    std::string getDoLabel();
    std::string getEndDoLabel();

    void geneSignalVarAssignMidCode(std::string &tarVar, std::string &var, SyntaxType syntaxType);
    void geneSignalVarAssignMidCode(std::string &tarVar, std::string &var);
    void geneConAssignMidCode(std::string &tarVar, SyntaxType syntaxType, const std::string &val);
    void geneArrayRefMidCode(std::string &tarVar, const std::string &arrayVar, std::string &indexVar, SyntaxType syntaxType);
    void geneArrayAssignMidCode(std::string &var, std::string &arrayVar, std::string &indexVar);
    void geneArrayDefMidCode(SyntaxType arrType, std::string &arrayVar, int arrayLen);
    void geneExprMidCode(std::string &tarVar, SyntaxType tarType, std::string &var1,
                         std::string &var2, const std::string &op);
    void geneJumpMidCode(std::string &var1, std::string &var2, const std::string &op,
            std::string &label, bool isInitReg=false);
    void geneConDefMidCode(SyntaxType syntaxType, std::string &var, const std::string &val);
    void geneVarDefMidCode(SyntaxType syntaxType, std::string &var);
    void geneCallFuncMidCode(std::string &func, std::vector<std::string> &vars, std::vector<std::string> &params,
            std::string &var, SyntaxType syntaxType);
    void geneFuncDefMidCode(std::string &func, std::vector<std::string> &params, std::vector<SyntaxType> &paramType);
    void geneLabelMidCode(std::string &label);
    void geneReturnMidCode(std::string &var);
    void geneReturnMidCode();
    void geneScanfMidCode(std::vector<std::string> &var);
    void genePrintfMidCode(std::string &v, bool isVar);
    void genePrintfMidCode(std::string &s, std::string &var);
    void geneDirJumpMidCode(std::string &label);
    void geneEndFunc();
    void geneInitRegs();

    void geneCodeBlockBegin();
    void geneCodeBlockEnd();

    std::vector<MdCode>& getMidCode();

    std::string pushStringVar(const std::string &s);
    std::vector<StringVar>& getStringVar();

private:
    int curVarNum = 0;
    int curStringVarNum = 0;

    int curIfLabelNum = 0;
    int curElseLabelNum = 0;
    int curEndIfLabelNum = 0;
    int curForLabelNum = 0;
    int curEndForLabelNum = 0;
    int curWhileLabelNum = 0;
    int curEndWhileLabelNum = 0;
    int curDoLabelNum = 0;
    int curEndDoLabelNum = 0;
    int curConVarNum = 0;

    std::vector<MdCode> midCode;
    std::string varPre = "@t";
    std::string conPre = "@con";
    std::string ifLabelPre = "IF";
    std::string elseLabelPre = "ELSE";
    std::string endIfLabelPre = "ENDIF";
    std::string forLabelPre = "FOR";
    std::string endForLabelPre = "ENDFOR";
    std::string whileLabelPre = "WHILE";
    std::string endWhileLabelPre = "ENDWHILE";
    std::string doLabelPre = "DO";
    std::string endDoLabelPre = "ENDDO";

    std::string stringPre = "String";
    std::vector<StringVar> stringVar;
};


#endif //COMPILER_MIDCODE_H
