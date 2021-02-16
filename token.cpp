#include "token.h"


TokenParse::TokenParse(std::string &file_path, ErrorHandler *errorHandler) {
    this->errorHandler = errorHandler;
    this->file.open(file_path, std::ios::in);
    initReservedWord();
    initSymbolType();
}

TokenParse::~TokenParse() {
    
}

bool TokenParse::isEof() {
    return this->file.peek() == EOF;
}

void TokenParse::initReservedWord() {
    this->reservedWord[NON] = "non";
    this->reservedWord[INTTK] = "int";
    this->reservedWord[CHARTK] = "char";
    this->reservedWord[CONSTTK] = "const";
    this->reservedWord[VOIDTK] = "void";
    this->reservedWord[MAINTK] = "main";
    this->reservedWord[IFTK] = "if";
    this->reservedWord[ELSETK] = "else";
    this->reservedWord[DOTK] = "do";
    this->reservedWord[WHILETK] = "while";
    this->reservedWord[FORTK] = "for";
    this->reservedWord[SCANFTK] = "scanf";
    this->reservedWord[PRINTFTK] = "printf";
    this->reservedWord[RETURNTK] = "return";
}

void TokenParse::initSymbolType() {
    this->symbolType[NON] = "NON";
    this->symbolType[IDENFR] = "IDENFR";
    this->symbolType[INTCON] = "INTCON";
    this->symbolType[CHARCON] = "CHARCON";
    this->symbolType[STRCON] = "STRCON";
    this->symbolType[CONSTTK] = "CONSTTK";
    this->symbolType[INTTK] = "INTTK";
    this->symbolType[CHARTK] = "CHARTK";
    this->symbolType[VOIDTK] = "VOIDTK";
    this->symbolType[MAINTK] = "MAINTK";
    this->symbolType[IFTK] = "IFTK";
    this->symbolType[ELSETK] = "ELSETK";
    this->symbolType[DOTK] = "DOTK";
    this->symbolType[WHILETK] = "WHILETK";
    this->symbolType[FORTK] = "FORTK";
    this->symbolType[SCANFTK] = "SCANFTK";
    this->symbolType[PRINTFTK] = "PRINTFTK";
    this->symbolType[RETURNTK] = "RETURNTK";
    this->symbolType[PLUS] = "PLUS";
    this->symbolType[MINU] = "MINU";
    this->symbolType[MULT] = "MULT";
    this->symbolType[DIV] = "DIV";
    this->symbolType[LSS] = "LSS";
    this->symbolType[LEQ] = "LEQ";
    this->symbolType[GRE] = "GRE";
    this->symbolType[GEQ] = "GEQ";
    this->symbolType[EQL] = "EQL";
    this->symbolType[NEQ] = "NEQ";
    this->symbolType[ASSIGN] = "ASSIGN";
    this->symbolType[SEMICN] = "SEMICN";
    this->symbolType[COMMA] = "COMMA";
    this->symbolType[LPARENT] = "LPARENT";
    this->symbolType[RPARENT] = "RPARENT";
    this->symbolType[LBRACK] = "LBRACK";
    this->symbolType[RBRACK] = "RBRACK";
    this->symbolType[LBRACE] = "LBRACE";
    this->symbolType[RBRACE] = "RBRACE";
}

bool TokenParse::isSpace() {
    return this->c == ' ' || this->c == '\r';
}

bool TokenParse::isTab() {
    return this->c == '\t';
}

bool TokenParse::isNewLine() {
    return this->c == '\n';
}

bool TokenParse::isComma() {
    return this->c == ',';
}

bool TokenParse::isSemi() {
    return this->c == ';';
}

bool TokenParse::isLetter() {
    return (this->c >= 'a' && this->c <= 'z') ||
           (this->c >= 'A' && this->c <= 'Z') || this->c == '_';
}

bool TokenParse::isDigit() {
    return this->c >= '0' && this->c <= '9';
}

bool TokenParse::isDigitZero() {
    return this->c == '0';
}

bool TokenParse::isDigitNoZero() {
    return this->c >= '1' && this->c <= '9';
}

bool TokenParse::isEqu() {
    return this->c == '=';
}

bool TokenParse::isEqu(char a) {
    return a == '=';
}

bool TokenParse::isPlus() {
    return this->c == '+';
}

bool TokenParse::isMinus() {
    return this->c == '-';
}

bool TokenParse::isDivi(char a) {
    return a == '/';
}

bool TokenParse::isDivi() {
    return this->c == '/';
}

bool TokenParse::isLSS() {
    return this->c == '<';
}

bool TokenParse::isGRE() {
    return this->c == '>';
}

bool TokenParse::isNo() {
    return this->c == '!';
}

bool TokenParse::isSignalQuote() {
    return this->c == '\'';
}

bool TokenParse::isDoubleQuote() {
    return this->c == '\"';
}

bool TokenParse::isStar() {
    return this->c == '*';
}

bool TokenParse::isLpar() {
    return this->c == '(';
}

bool TokenParse::isRpar() {
    return this->c == ')';
}

bool TokenParse::isLbrack() {
    return this->c == '[';
}

bool TokenParse::isRbrack() {
    return this->c == ']';
}

bool TokenParse::isLbrace() {
    return this->c == '{';
}

bool TokenParse::isRbrace() {
    return this->c == '}';
}

bool TokenParse::isEscape() {
    return this->c == '\\';
}

bool TokenParse::skipSpace() {
    // if a '\n' add row num
    if (isNewLine())
        this->errorHandler->addRow();
    return isSpace() || isTab() || isNewLine();
}

void TokenParse::getChar() {
    // read letter until letter is valid
    this->c = this->file.get();
}

char TokenParse::nextChar() {
    return file.peek();
}

void TokenParse::clearTok() {
    this->token.tok.clear();
}

void TokenParse::catTok() {
    this->token.tok.append(1, this->c);
}

void TokenParse::catTok(char c) {
    this->token.tok.append(1, c);
}

void TokenParse::restract() {
    this->file.putback(this->c);
}

int TokenParse::reserve() {
    for (int i = 1; i < SYMBOLTYPENUM; i++) {
        if (this->reservedWord[i] == this->token.tok) {
            return i;
        }
    }
    return 0;
}

void TokenParse::readToken() {
    clearTok();
    getChar();
    while (skipSpace()) {
        getChar();
    }

    if (isLetter()) {
        while (isLetter() || isDigit()) {
            catTok();
            getChar();
        }
        restract();
        int sym = reserve();
        if (sym == 0) {
            this->token.symbol = this->symbolType[IDENFR];
            this->token.sym = IDENFR;
        } else {
            this->token.symbol = this->symbolType[sym];
            this->token.sym = Symbol(sym);
        }
    } else if (isDigit()) {
        while (isDigit()) {
            catTok();
            getChar();
        }
        restract();
        this->token.symbol = this->symbolType[INTCON];
        this->token.sym = INTCON;
    } else if (isSignalQuote()) {
        getChar();
        if (!isDivi() && !isStar() && !isPlus() && !isMinus() && !isLetter() && !isDigit())
            this->errorHandler->inValidLetterError();
        catTok();
        getChar();
        this->token.symbol = this->symbolType[CHARCON];
        this->token.sym = CHARCON;
    } else if (isDoubleQuote()) {
        getChar();
        while (!isDoubleQuote()) {
            if (this->c != 32 && this->c != 33 && !(this->c >= 35 && this->c <= 126))
                this->errorHandler->inValidLetterError();
            if (isEscape())
                catTok('\\');
            catTok();
            getChar();
        }
        this->token.symbol = this->symbolType[STRCON];
        this->token.sym = STRCON;
    } else if (isDivi()) {
        catTok();
        this->token.symbol = this->symbolType[DIV];
        this->token.sym = DIV;
    } else if (isLSS()) {
        catTok();
        if (isEqu(nextChar())) {
            getChar();
            catTok();
            this->token.symbol = this->symbolType[LEQ];
            this->token.sym = LEQ;
        } else {
            this->token.symbol = this->symbolType[LSS];
            this->token.sym = LSS;
        }
    } else if (isGRE()) {
        catTok();
        if (isEqu(nextChar())) {
            getChar();
            catTok();
            this->token.symbol = this->symbolType[GEQ];
            this->token.sym = GEQ;
        } else {
            this->token.symbol = this->symbolType[GRE];
            this->token.sym = GRE;
        }
    } else if (isEqu()) {
        catTok();
        if (isEqu(nextChar())) {
            getChar();
            catTok();
            this->token.symbol = this->symbolType[EQL];
            this->token.sym = EQL;
        } else {
            this->token.symbol = this->symbolType[ASSIGN];
            this->token.sym = ASSIGN;
        }
    } else if (isPlus()) {
        catTok();
        this->token.symbol = this->symbolType[PLUS];
        this->token.sym = PLUS;
    } else if (isMinus()) {
        catTok();
        this->token.symbol = this->symbolType[MINU];
        this->token.sym = MINU;
    } else if (isStar()) {
        catTok();
        this->token.symbol = this->symbolType[MULT];
        this->token.sym = MULT;
    } else if (isNo()) {
        catTok();
        getChar();
        catTok();
        this->token.symbol = this->symbolType[NEQ];
        this->token.sym = NEQ;
    } else if (isSemi()) {
        catTok();
        this->token.symbol = this->symbolType[SEMICN];
        this->token.sym = SEMICN;
    } else if (isComma()) {
        catTok();
        this->token.symbol = this->symbolType[COMMA];
        this->token.sym = COMMA;
    } else if (isLpar()) {
        catTok();
        this->token.symbol = this->symbolType[LPARENT];
        this->token.sym = LPARENT;
    } else if (isRpar()) {
        catTok();
        this->token.symbol = this->symbolType[RPARENT];
        this->token.sym = RPARENT;
    } else if (isLbrack()) {
        catTok();
        this->token.symbol = this->symbolType[LBRACK];
        this->token.sym = LBRACK;
    } else if (isRbrack()) {
        catTok();
        this->token.symbol = this->symbolType[RBRACK];
        this->token.sym = RBRACK;
    } else if (isLbrace()) {
        catTok();
        this->token.symbol = this->symbolType[LBRACE];
        this->token.sym = LBRACE;
    } else if (isRbrace()) {
        catTok();
        this->token.symbol = this->symbolType[RBRACE];
        this->token.sym = RBRACE;
    } else {
        this->errorHandler->inValidLetterError();
        readToken();
    }
}

Token TokenParse::getToken() {
    this->errorHandler->setLastRow();
    readToken();
    return this->token;
}

// test func
bool TokenParse::isOpen() {
    return this->file.is_open();
}