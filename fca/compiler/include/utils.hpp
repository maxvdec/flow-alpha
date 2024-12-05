/*
 utils.hpp
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Utilities for the Compiler
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef FCA_UTILS_HPP
#define FCA_UTILS_HPP

#include <string>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define ITALIC "\033[3m"

namespace fca {
void log(const std::string &message);
void log_bold(const std::string &message);
void error(const std::string &message);
} // namespace fca

#endif
