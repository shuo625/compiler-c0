#ifndef COMPILER_TOKEN_H_
#define COMPILER_TOKEN_H_


#include "stdafx.h"
#include "errorHandler.h"


const int RESERVEDWORDNUM = 14;
const int SYMBOLTYPENUM = 37;


enum Symbol {
    NON = 0,
    CONSTTK,
    INTTK,
    CHARTK,
    VOIDTK,
    MAINTK,
    IFTK,
    ELSETK,
    DOTK,
    WHILETK,
    FORTK,
    SCANFTK,
    PRINTFTK,
    RETURNTK,
    IDENFR,
    INTCON,
    CHARCON,
    STRCON,
    PLUS,
    MINU,
    MULT,
    DIV,
    LSS,
    LEQ,
    GRE,
    GEQ,
    EQL,
    NEQ,
    ASSIGN,
    SEMICN,
    COMMA,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE
};


typedef struct Token {
    std::string tok;
    std::string symbol;
    Symbol sym;
}Token;


class TokenParse {

public:
    
    TokenParse(std::string &file_path, ErrorHandler *errorHandler);
    // get a token
    Token getToken();
    bool isEof();
    ~TokenParse();
    // test func
    bool isOpen();

private:
    ErrorHandler *errorHandler;
    // input file stream
    std::fstream file;
    // current char
    char c;
    // reserved words table
    std::string reservedWord[SYMBOLTYPENUM];
    // symbol type num
    std::string symbolType[SYMBOLTYPENUM];
    // current token symbol
    Token token;

    // read a token from file
    void readToken();

    // init reserved word table
    void initReservedWord();
    // init symbol type table
    void initSymbolType();
    // see next char
    char nextChar();
    // get a new char
    void getChar();
    bool isDivi(char a);
    bool isEqu(char a);
    
    // skip space char
    bool skipSpace();
    // is space char
    bool isSpace();
    bool isTab();
    bool isNewLine();
    
    // judge signal char
    bool isLetter();
    bool isDigit();
    bool isDigitZero();
    bool isDigitNoZero();
    bool isLSS();
    bool isGRE();
    bool isNo();
    bool isComma();
    bool isSemi();
    bool isStar();
    bool isLpar();
    bool isRpar();
    bool isEqu();
    bool isPlus();
    bool isMinus();
    bool isDivi();
    bool isSignalQuote();
    bool isDoubleQuote();
    bool isLbrack();
    bool isRbrack();
    bool isLbrace();
    bool isRbrace();
    bool isEscape();
    // link char and token
    void catTok();
    void catTok(char c);
    // restract a char
    void restract();
    // find symbol
    int reserve();
    // transform token to number
    // int transNum();
    // clear token
    void clearTok();

};


#endif