#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H


#include "stdafx.h"


enum OutLevel {
    NONE = 0,
    CONSOLE,
    OUTFILE
};


class Output {
public:
    Output();
    ~Output();
    void outSyntax(const std::string &outcon);
    void outError(int row, char errorCode);
    void outMips(const std::string &mips_code);
    void outTestInfo(int row, const std::string &outcon);

private:
    OutLevel outSyntaxLevel = NONE;
    OutLevel outErrorLevel = NONE;
    OutLevel outMipsLevel = OUTFILE;
    OutLevel outTestInfoLevel = NONE;

    std::string output_test_info = "testInfo.txt";
    std::string output_syntax = "syntax.txt";
    std::string output_error = "error.txt";
    std::string output_mips = "mips.txt";
    std::fstream out_test_info;
    std::fstream out_error;
    std::fstream out_mips;
    std::fstream out_syntax;
};


#endif //COMPILER_OUTPUT_H
