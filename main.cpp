#include "compiler.h"


int main() {
    std::string file = "testfile.txt";
    Compiler *compiler = new Compiler(file);
    compiler->compile();
}
