#include "compiler.h"


Compiler::Compiler(std::string &compile_file) {
    Output *output = new Output();
    MidCode *midCode = new MidCode();
    this->syntaxParse = new SyntaxParse(compile_file, output, midCode);
    this->optMips = new OptMips(output, midCode);
    this->mips = new Mips(output, midCode);
}

Compiler::~Compiler() {

}

void Compiler::compile() {
    this->syntaxParse->checkSyntax();
    if (this->isOpt) {
        this->optMips->geneMips();
    } else {
        this->mips->geneMips();
    }
}