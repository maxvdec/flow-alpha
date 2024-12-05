/*
 preprocess.cpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Preprocess process for inclusions and macros
 Copyright (c) 2024 Maxims Enterprise
*/

#include "flowcmp.hpp"
#include "lexer.hpp"
#include "utils.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

Program preprocess(const std::string &source, const std::string &path) {
    std::vector<Token> tokens = tokenize(source);
    Program program;

    program.original_tokens = tokens;

    std::vector<Token> saved;

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TokenType::Include) {
            std::string file_path;
            i++;
            if (tokens[i].type == TokenType::String) {
                file_path = tokens[i].value;
            } else {
                fca::error("Expected string after !include");
            }

            std::string total_path =
                path.substr(0, path.find_last_of("/")) + "/" + file_path;

            std::ifstream file(total_path);
            if (!file.is_open()) {
                fca::error("Could not open file: " + file_path);
            }

            std::string line;
            std::string contents;

            while (std::getline(file, line)) {
                contents += line + "\n";
            }

            Program included_program = preprocess(contents, file_path);

            program.macros.insert(program.macros.end(),
                                  included_program.macros.begin(),
                                  included_program.macros.end());
            saved.insert(saved.end(), included_program.body.begin(),
                         included_program.body.end());
        } else if (tokens[i].type == TokenType::MacroDeclaration) {
            std::vector<std::string> arguments;
            std::string macro_name = tokens[i].value;
            i++;
            while (tokens[i].type == TokenType::MacroArg) {
                if (tokens[i].value.ends_with(")")) {
                    tokens[i].value.pop_back();
                }
                arguments.push_back(tokens[i].value);
                i++;
                if (tokens[i].type == TokenType::Comma) {
                    i++;
                }
            }

            std::vector<Token> body;

            while (tokens[i].type != TokenType::EndMacro) {
                body.push_back(tokens[i]);
                i++;
            }

            Macro macro = {macro_name, (int)arguments.size(), arguments, body};
            program.macros.push_back(macro);
        } else {
            program.body.push_back(tokens[i]);
        }
    }

    program.body.insert(program.body.end(), saved.begin(), saved.end());

    return program;
}

Program expand_macros(Program program) {
    std::vector<std::string> macro_names;
    for (Macro m : program.macros) {
        macro_names.push_back(m.name);
    }

    for (int i = 0; i < program.body.size(); i++) {
        Token t = program.body[i];
        if (t.type == TokenType::MacroCall) {
            int first_index = i;
            std::vector<Token> arguments;
            int macro_index =
                std::find(macro_names.begin(), macro_names.end(), t.value) -
                macro_names.begin() - 1;

            if (macro_index == -1) {
                fca::error("Macro " + t.value + " not found");
            }

            Macro macro = program.macros[macro_index];
            i++;

            while (true) {
                arguments.push_back(program.body[i]);
                i++;
                if (program.body[i].type != TokenType::Comma) {
                    break;
                }
                i++;
            }

            std::vector<Token> expanded = macro.expand(arguments);

            program.body.erase(program.body.begin() + first_index,
                               program.body.begin() + i);

            program.body.insert(program.body.begin() + first_index,
                                expanded.begin(), expanded.end());
        }
    }

    return program;
}

std::vector<Token> Macro::expand(const std::vector<Token> &arguments) {
    std::vector<Token> expanded_body;

    for (Token t : body) {
        if (t.type == TokenType::MacroArg) {
            int arg_index = -1;
            for (int i = 0; i < this->arguments.size(); i++) {
                if (t.value == this->arguments[i]) {
                    arg_index = i;
                    break;
                }
            }
            if (arg_index == -1) {
                fca::error("Argument " + t.value +
                           " not defined in macro scope");
            }
            expanded_body.push_back(arguments[arg_index]);
        } else {
            expanded_body.push_back(t);
        }
    }

    return expanded_body;
}
