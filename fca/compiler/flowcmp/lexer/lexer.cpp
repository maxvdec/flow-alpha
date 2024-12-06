/*
 lexer.cpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Lexer for the Flow Compiler
 Copyright (c) 2024 Maxims Enterprise
*/

#include "lexer.hpp"
#include "utils.hpp"
#include <cstddef>
#include <cwctype>
#include <iostream>
#include <string>
#include <vector>

std::vector<Token> tokenize(const std::string &str) {
    std::vector<Token> tokens;

    for (int i = 0; i < str.length(); i++) {
        char c = str[i];

        if (isspace(c)) {
            continue;
        } else if (c == '"') {
            std::string string = "";
            while (i + 1 < str.length() && str[i + 1] != '"') {
                string += str[++i];
            }
            i++;
            tokens.push_back({TokenType::String, string});
        } else {
            std::string chain;
            chain += c;
            while (i + 1 < str.length() &&
                   (!isspace(str[i + 1]) && str[i + 1] != ',')) {
                chain += str[++i];
            }

            if (chain == "add") {
                tokens.push_back({TokenType::Add, chain});
            } else if (chain == "sub") {
                tokens.push_back({TokenType::Subtract, chain});
            } else if (chain == "mul") {
                tokens.push_back({TokenType::Multiply, chain});
            } else if (chain == "div") {
                tokens.push_back({TokenType::Divide, chain});
            } else if (chain == "inc") {
                tokens.push_back({TokenType::Increase, chain});
            } else if (chain == "dec") {
                tokens.push_back({TokenType::Decrease, chain});
            } else if (chain == "switch") {
                tokens.push_back({TokenType::Switch, chain});
            } else if (chain == "and") {
                tokens.push_back({TokenType::And, chain});
            } else if (chain == "or") {
                tokens.push_back({TokenType::Or, chain});
            } else if (chain == "xor") {
                tokens.push_back({TokenType::Xor, chain});
            } else if (chain == "not") {
                tokens.push_back({TokenType::Not, chain});
            } else if (chain == "shiftl") {
                tokens.push_back({TokenType::LeftShift, chain});
            } else if (chain == "shiftr") {
                tokens.push_back({TokenType::RightShift, chain});
            } else if (chain == "mov") {
                tokens.push_back({TokenType::Move, chain});
            } else if (chain == "push") {
                tokens.push_back({TokenType::Push, chain});
            } else if (chain == "pop") {
                tokens.push_back({TokenType::Pop, chain});
            } else if (chain == "jmp") {
                tokens.push_back({TokenType::Jump, chain});
            } else if (chain == "jnz") {
                tokens.push_back({TokenType::JumpNotZero, chain});
            } else if (chain == "in") {
                tokens.push_back({TokenType::In, chain});
            } else if (chain == "out") {
                tokens.push_back({TokenType::Out, chain});
            } else if (chain == "jz") {
                tokens.push_back({TokenType::JumpZero, chain});
            } else if (chain == "jgr") {
                tokens.push_back({TokenType::JumpGreater, chain});
            } else if (chain == "jls") {
                tokens.push_back({TokenType::JumpLess, chain});
            } else if (chain == "halt") {
                tokens.push_back({TokenType::Halt, chain});
            } else if (chain == "reset") {
                tokens.push_back({TokenType::Reset, chain});
            } else if (chain == "nothing") {
                tokens.push_back({TokenType::Nothing, chain});
            } else if (chain.ends_with(":")) {
                tokens.push_back(
                    {TokenType::Label, chain.substr(0, chain.length() - 1)});
            } else if (chain == ",") {
                tokens.push_back({TokenType::Comma, chain});
            } else if (is_number(chain)) {
                tokens.push_back({TokenType::Number, chain});
            } else if (chain.starts_with("r")) {
                if (chain == "r1" || chain == "r2" || chain == "r0" ||
                    chain == "r3" || chain == "r4" || chain == "r5" ||
                    chain == "r6" || chain == "r7" || chain.starts_with("g")) {
                    tokens.push_back({TokenType::Register, chain});
                } else {
                    std::cout << "Could not match token: " << chain
                              << std::endl;
                }
            } else if (chain.ends_with("!")) {
                tokens.push_back({TokenType::Constant, chain});
            } else if (chain == "!origin") {
                tokens.push_back({TokenType::Origin, chain});
            } else if (chain == "flow-alpha") {
                tokens.push_back({TokenType::Constant, chain});
            } else if (chain == "!include") {
                tokens.push_back({TokenType::Include, chain});
            } else if (chain == "!lang") {
                tokens.push_back({TokenType::Language, chain});
            } else if (chain.starts_with("$")) {
                tokens.push_back({TokenType::MacroArg, chain});
            } else if (chain.starts_with("&")) {
                tokens.push_back({TokenType::RegisterReference, chain});
            } else if (chain == "@end") {
                tokens.push_back({TokenType::EndMacro, chain});
            } else if (chain.starts_with("(") && chain.ends_with(")")) {
                tokens.push_back({TokenType::LabelReference,
                                  chain.substr(1, chain.length() - 2)});
            } else if (chain.starts_with("@")) {
                if (chain.find("($") != std::string::npos) {
                    int paren_pos = chain.find("($");
                    std::string macro_name = chain.substr(1, paren_pos - 1);
                    std::string macro_arg1 =
                        chain.substr(paren_pos + 1, chain.length() - 1);
                    tokens.push_back({TokenType::MacroDeclaration, macro_name});
                    tokens.push_back({TokenType::MacroArg, macro_arg1});
                } else {
                    tokens.push_back({TokenType::MacroCall, chain});
                }
            } else {
                fca::error("Could not match token: " + chain);
            }
        }
    }

    fca::log("Tokenized successfully. Proceding to macro substitution and "
             "inclusions...");

    return tokens;
}

bool is_number(const std::string &s) {
    try {
        size_t pos;
        std::stoi(s, &pos, 0);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

std::string type_to_string(TokenType type) {
    switch (type) {
    case TokenType::Add:
        return "Add";
    case TokenType::Subtract:
        return "Subtract";
    case TokenType::Multiply:
        return "Multiply";
    case TokenType::Divide:
        return "Divide";
    case TokenType::Increase:
        return "Increase";
    case TokenType::Decrease:
        return "Decrease";
    case TokenType::Switch:
        return "Switch";
    case TokenType::And:
        return "And";
    case TokenType::Or:
        return "Or";
    case TokenType::Xor:
        return "Xor";
    case TokenType::Not:
        return "Not";
    case TokenType::LeftShift:
        return "LeftShift";
    case TokenType::RightShift:
        return "RightShift";
    case TokenType::Move:
        return "Move";
    case TokenType::Push:
        return "Push";
    case TokenType::Pop:
        return "Pop";
    case TokenType::Jump:
        return "Jump";
    case TokenType::JumpNotZero:
        return "JumpNotZero";
    case TokenType::In:
        return "In";
    case TokenType::Out:
        return "Out";
    case TokenType::JumpZero:
        return "JumpZero";
    case TokenType::JumpGreater:
        return "JumpGreater";
    case TokenType::JumpLess:
        return "JumpLess";
    case TokenType::Halt:
        return "Halt";
    case TokenType::Reset:
        return "Reset";
    case TokenType::Nothing:
        return "Nothing";
    case TokenType::Label:
        return "Label";
    case TokenType::Number:
        return "Number";
    case TokenType::Register:
        return "Register";
    case TokenType::Origin:
        return "Origin";
    case TokenType::MacroCall:
        return "MacroCall";
    case TokenType::Constant:
        return "Constant";
    case TokenType::EndMacro:
        return "EndMacro";
    case TokenType::Comma:
        return "Comma";
    case TokenType::Include:
        return "Include";
    case TokenType::Language:
        return "Language";
    case TokenType::String:
        return "String";
    case TokenType::MacroDeclaration:
        return "MacroDeclaration";
    case TokenType::MacroArg:
        return "MacroArg";
    case TokenType::RegisterReference:
        return "RegisterReference";
    default:
        return "Unknown";
    }
}
