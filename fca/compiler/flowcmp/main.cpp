/*
 main.cpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Entry point for the Flow Computer Assembly compiler
 Copyright (c) 2024 Maxims Enterprise
*/

#include "flowcmp.hpp"
#include "lexer.hpp"
#include "utils.hpp"
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Usage: flowcmp <source file>" << std::endl;
        return 1;
    } else {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }

        std::string line;
        std::string contents;
        while (std::getline(file, line)) {
            contents += line + "\n";
        }

        fca::log("Compiling " + std::string(argv[1]) + "...");

        Program program = preprocess(contents, argv[1]);
        program = expand_macros(program);

        compile(program, "./tests/out.fch");
        fca::log_bold("Compiled successfully.");
        return 0;
    }
}
