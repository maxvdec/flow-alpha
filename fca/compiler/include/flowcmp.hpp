/*
 flowcmp.hpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Main compiler header files
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef FCA_FLOWCMP_HPP
#define FCA_FLOWCMP_HPP

#include "lexer.hpp"
#include <cstdint>
#include <string>
#include <vector>

struct Macro {
    std::string name;
    int arg_count;
    std::vector<std::string> arguments;
    std::vector<Token> body;
    std::vector<Token> expand(const std::vector<Token> &arguments);
};

struct Program {
    std::vector<Token> final_tokens = {};
    std::vector<Token> original_tokens = {};
    std::vector<Macro> macros;
    std::vector<Token> body;
};

Program preprocess(const std::string &source, const std::string &path);
Program expand_macros(Program program);
void compile(Program program, std::string output);

std::vector<uint8_t> resolve_number(const std::string &number);
std::vector<uint8_t> int_to_bytes(int number);
int bytes_to_int(const std::vector<uint8_t> &byteVector);
Token expect(Token token, TokenType type);

#endif
