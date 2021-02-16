#include "output.h"


Output::Output() {
    if (this->outErrorLevel != NONE)
        this->out_error.open(this->output_error, std::ios::out);
    if (this->outMipsLevel != NONE)
        this->out_mips.open(this->output_mips, std::ios::out);
    if (this->outSyntaxLevel != NONE)
        this->out_syntax.open(this->output_syntax, std::ios::out);
    if (this->outTestInfoLevel != NONE)
        this->out_test_info.open(this->output_test_info, std::ios::out);
}

Output::~Output() {

}

void Output::outSyntax(const std::string &outcon) {
    switch (this->outSyntaxLevel) {
        case CONSOLE:
            std::cout << outcon << std::endl;
            break;
        case OUTFILE:
            this->out_syntax << outcon << std::endl;
        default:
            break;
    }
}

void Output::outError(int row, char errorCode) {
    switch (this->outErrorLevel) {
        case CONSOLE:
            std::cout << row << " " << errorCode << std::endl;
            break;
        case OUTFILE:
            this->out_error << row << " " << errorCode << std::endl;
            break;
        default:
            break;
    }
}

void Output::outTestInfo(int row, const std::string &outcon) {
    switch (this->outTestInfoLevel) {
        case CONSOLE:
            std::cout << row << " " << outcon << std::endl;
            break;
        case OUTFILE:
            this->out_test_info << row << " " << outcon << std::endl;
            break;
        default:
            break;
    }
}

void Output::outMips(const std::string &mips_code) {
    switch (this->outMipsLevel) {
        case CONSOLE:
            std::cout << mips_code << std::endl;
            break;
        case OUTFILE:
            this->out_mips << mips_code << std::endl;
            break;
        default:
            break;
    }
}