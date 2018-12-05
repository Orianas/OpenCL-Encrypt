/**
 * @file Utils.h
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief Utility Function Header File
 * @version 0.1
 * @date 2018-11-24
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#pragma once
#include "stdfx.h"

void printUsage(int argc, char* argv[]);
std::vector<std::pair<std::string, std::string>> parseCommandLine(int argc, char* argv[]);
std::pair<std::string, std::string> parseSingleOption(int argc, char* argv[], int index, std::string flag);
std::pair<std::string, std::string> parseDoubleOption(int argc, char* argv[], int index, std::string flag);
std::pair<std::string, std::string> parseModalOption(int argc, char* argv[], int index, std::string flag);