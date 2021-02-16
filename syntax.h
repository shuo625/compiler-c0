#ifndef COMPILER_SYNTAX_H
#define COMPILER_SYNTAX_H


#include "stdafx.h"
#include "token.h"
#include "symbolTable.h"
#include "output.h"
#include "errorHandler.h"
#include "midCode.h"


class SyntaxParse {
public:
    SyntaxParse(std::string &file_path, Output *output, MidCode *midCode);
    void checkSyntax();
    ~SyntaxParse();

private:
    ErrorHandler *errorHandler;
    TokenParse *tokenParse;
    SymbolTable *symbolTable;
    Output *output;
    Token token;
    Symbol sym;
    std::vector<Token> preTokens;
    SyntaxType funcType;
    MidCode *midCode;
    std::string conVal;

    void getSym();
    void restract();
    Symbol getPreSym(int id);
    
    // child program
    void program();
    void mainFunc();

    bool unsignedInt();
    bool intcon();
    bool strcon();
    void declareHeader();
    
    void constDeclare();
    void constDefine();
    void variableDeclare();
    void variableDefine();

    void returnedFuncDefine();
    void unreturnedFuncDefine();

    // statements
    bool statement();
    bool compoundStatement();
    bool forStatement();
    void assignStatement();
    bool ifStatement();
    void callFuncStatement(std::string *var);
    void scanfStatement();
    void printfStatement();
    bool returnStatement();
    bool statements();

    bool expression(std::string *var);
    bool item(std::string *var, bool *isCon);
    bool factor(std::string *var, bool *isCon);

    std::vector<Paramter> parameters();
    void qualification(std::string &label, bool isInitReg=false);
    void step(std::string *var);
    std::vector<Paramter> valueParm();

};


#endif //COMPILER_SYNTAX_H
