/*
 compile.cpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Compiler entry point
 Copyright (c) 2024 Maxims Enterprise
*/

#include "flowcmp.hpp"
#include "lexer.hpp"
#include "utils.hpp"
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

void compile(Program program, std::string output) {
    std::vector<uint8_t> binary;
    int offset = 0x10000;
    std::unordered_map<std::string, int> labels = {};
    bool offset_instruction = false;

    for (int i = 0; i < program.body.size(); i++) {
        if (program.body[i].type == TokenType::Language) {
            i++;
        } else if (program.body[i].type == TokenType::Origin) {
            i++;
            if (program.body[i].type != TokenType::Number) {
                fca::error("Expected number after !origin");
            }
            std::vector<uint8_t> number = resolve_number(program.body[i].value);
            binary.push_back(0x2A);
            binary.insert(binary.end(), number.begin(), number.end());
            offset = bytes_to_int(number);
            offset_instruction = true;
        } else {
            if (!offset_instruction) {
                binary.push_back(0x2A);
                binary.push_back(0x10);
                binary.push_back(0x00);
                binary.push_back(0x00);
                offset_instruction = true;
            }
            switch (program.body[i].type) {
            case TokenType::Label:
                labels[program.body[i].value] = offset + binary.size();
                std::cout << "Label: " << program.body[i].value << " at "
                          << offset + binary.size() << std::endl;
                break;
            case TokenType::Add:
                binary.push_back(0x10);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after add");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Subtract:
                binary.push_back(0x11);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after sub");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Multiply:
                binary.push_back(0x12);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after mul");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Divide:
                binary.push_back(0x13);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after div");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Increase:
                binary.push_back(0x14);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after inc");
                }
                i++;
                break;
            case TokenType::Decrease:
                binary.push_back(0x15);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after dec");
                }
                i++;
                break;
            case TokenType::Switch:
                binary.push_back(0x16);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after switch");
                }
                i++;
                break;
            case TokenType::And:
                binary.push_back(0x17);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after and");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Or:
                binary.push_back(0x18);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after or");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Xor:
                binary.push_back(0x19);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after xor");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Not:
                binary.push_back(0x1A);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after not");
                }
                i++;
                break;
            case TokenType::LeftShift:
                binary.push_back(0x1B);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after lshift");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::RightShift:
                binary.push_back(0x1C);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after rshift");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Move:
                binary.push_back(0x1D);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else {
                    fca::error("Expected register after mov");
                }
                i++;
                i++;
                expect(program.body[i], TokenType::Comma);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                } else if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Push:
                binary.push_back(0x1E);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                }
                i++;
                break;
            case TokenType::Pop:
                binary.push_back(0x1F);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                }
                i++;
                break;
            case TokenType::Jump:
                binary.push_back(0x20);
                if (program.body[i + 1].type == TokenType::Number) {
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                } else if (program.body[i + 1].type ==
                           TokenType::LabelReference) {
                    std::string label = program.body[i + 1].value;
                    if (labels.find(label) == labels.end()) {
                        fca::error("Label not found: " + label);
                    }
                    int labelOffset = labels[label];
                    std::vector<uint8_t> number = int_to_bytes(labelOffset);
                    binary.insert(binary.end(), number.begin(), number.end());
                    i++;
                }
                break;
            case TokenType::JumpNotZero:
                binary.push_back(0x21);
                if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                } else if (program.body[i + 1].type ==
                           TokenType::LabelReference) {
                    std::string label = program.body[i + 1].value;
                    if (labels.find(label) == labels.end()) {
                        fca::error("Label not found: " + label);
                    }
                    int labelOffset = labels[label];
                    std::vector<uint8_t> number = int_to_bytes(labelOffset);
                    binary.insert(binary.end(), number.begin(), number.end());
                    i++;
                }
            case TokenType::In:
                binary.push_back(0x22);
                if (program.body[i + 1].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(1)));
                }
                if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                i++;
                break;
            case TokenType::Out:
                binary.push_back(0x23);
                if (program.body[i + 1].type == TokenType::Number) {
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                }
                binary.push_back(0xFE);
                if (program.body[i + 2].type == TokenType::Register) {
                    binary.push_back(
                        std::stoi(program.body[i + 2].value.substr(1)));
                } else if (program.body[i + 2].type == TokenType::Number) {
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 2].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 2].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(
                        std::stoi(program.body[i + 2].value.substr(2)));
                }
                i++;
                break;
            case TokenType::JumpZero:
                binary.push_back(0x24);
                if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                } else if (program.body[i + 1].type ==
                           TokenType::LabelReference) {
                    std::string label = program.body[i + 1].value;
                    if (labels.find(label) == labels.end()) {
                        fca::error("Label not found: " + label);
                    }
                    int labelOffset = labels[label];
                    std::vector<uint8_t> number = int_to_bytes(labelOffset);
                    binary.insert(binary.end(), number.begin(), number.end());
                    i++;
                }
                i++;
                break;
            case TokenType::JumpGreater:
                binary.push_back(0x25);
                if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                } else if (program.body[i + 1].type ==
                           TokenType::LabelReference) {
                    std::string label = program.body[i + 1].value;
                    if (labels.find(label) == labels.end()) {
                        fca::error("Label not found: " + label);
                    }
                    int labelOffset = labels[label];
                    std::vector<uint8_t> number = int_to_bytes(labelOffset);
                    binary.insert(binary.end(), number.begin(), number.end());
                    i++;
                }
                i++;
                break;
            case TokenType::JumpLess:
                binary.push_back(0x26);
                if (program.body[i + 1].type == TokenType::Number) {
                    binary.push_back(0xFD);
                    std::vector<uint8_t> number =
                        resolve_number(program.body[i + 1].value);
                    binary.insert(binary.end(), number.begin(), number.end());
                } else if (program.body[i + 1].type ==
                           TokenType::RegisterReference) {
                    binary.push_back(0xFF);
                    binary.push_back(
                        std::stoi(program.body[i + 1].value.substr(2)));
                } else if (program.body[i + 1].type ==
                           TokenType::LabelReference) {
                    std::string label = program.body[i + 1].value;
                    if (labels.find(label) == labels.end()) {
                        fca::error("Label not found: " + label);
                    }
                    int labelOffset = labels[label];
                    std::vector<uint8_t> number = int_to_bytes(labelOffset);
                    binary.insert(binary.end(), number.begin(), number.end());
                    i++;
                }
                i++;
                break;
            case TokenType::Halt:
                binary.push_back(0x27);
                break;
            case TokenType::Reset:
                binary.push_back(0x28);
                break;
            case TokenType::Nothing:
                binary.push_back(0x29);
                break;
            default:
                fca::error("Didn't expect to find: " +
                           type_to_string(program.body[i].type));
            }

            // Check if the number of bytes is even
            if (binary.size() % 2 != 0) {
                binary.push_back(0x00);
            }
        }
    }

    for (int i = 0; i < binary.size(); i++) {
        std::cout << std::hex << (int)binary[i] << " ";
    }

    std::ofstream output_stream(output, std::ios::binary);
    if (!output_stream) {
        fca::error("Could not open output file: " + output);
    }

    output_stream.write(reinterpret_cast<const char *>(binary.data()),
                        binary.size());
    output_stream.close();
}

std::vector<uint8_t> resolve_number(const std::string &numStr) {
    std::vector<uint8_t> result;
    std::string hex = "";
    std::cout << numStr << std::endl;

    if (!numStr.starts_with("0x")) {
        int decimal = std::stoi(numStr);

        std::stringstream ss;
        ss << std::hex << decimal;
        hex = ss.str();
    } else {
        hex = numStr.substr(2);
    }

    if (hex.size() % 2 != 0) {
        hex = "0" + hex;
    }

    for (int i = 0; i < hex.size(); i += 2) {
        std::string byte = hex.substr(i, 2);

        uint8_t byteVal = std::stoi(byte, nullptr, 16);
        result.push_back(byteVal);
    }

    return result;
}

std::vector<uint8_t> int_to_bytes(int number) {
    std::vector<uint8_t> byteVector;

    for (size_t i = sizeof(int); i > 0; --i) {
        uint8_t byte = (number >> ((i - 1) * 8)) & 0xFF;
        byteVector.push_back(byte);
    }

    return byteVector;
}

int bytes_to_int(const std::vector<uint8_t> &byteVector) {
    int result = 0;

    for (size_t i = 0; i < byteVector.size(); ++i) {
        result = (result << 8) | byteVector[i];
    }

    return result;
}

Token expect(Token token, TokenType type) {
    if (token.type != type) {
        fca::error("Expected " + type_to_string(type) + " but found " +
                   type_to_string(token.type));
    }
    return token;
}
