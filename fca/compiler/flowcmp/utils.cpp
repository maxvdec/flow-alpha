/*
 utils.cpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Utility functions implementation
 Copyright (c) 2024 Maxims Enterprise
*/

#include "utils.hpp"
#include <cstdlib>
#include <iostream>

namespace fca {
void log(const std::string &message) {
    std::cout << GREEN << ITALIC << message << RESET << std::endl;
}

void error(const std::string &message) {
    std::cerr << RED << BOLD << "Error: " << message << RESET << std::endl;
    std::exit(1);
}

void log_bold(const std::string &message) {
    std::cout << GREEN << BOLD << message << RESET << std::endl;
}

} // namespace fca
