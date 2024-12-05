/*
 lexer.hpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Lexer header for flowcmp
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef FCA_LEXER_HPP
#define FCA_LEXER_HPP

#include <string>
#include <vector>

enum class TokenType {
    Add,
    Subtract,
    Multiply,
    Divide,
    Increase,
    Decrease,
    Switch,
    And,
    Or,
    Xor,
    Not,
    LeftShift,
    RightShift,
    Move,
    Push,
    Pop,
    Jump,
    JumpNotZero,
    In,
    Out,
    JumpZero,
    JumpGreater,
    JumpLess,
    Halt,
    Reset,
    Nothing,
    Label,
    Number,
    Register,
    Origin,
    MacroCall,
    Constant,
    EndMacro,
    Comma,
    Include,
    Language,
    String,
    MacroDeclaration,
    MacroArg,
    RegisterReference,
    LabelReference,
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string &source);
bool is_number(const std::string &s);
std::string type_to_string(TokenType type);

#endif
