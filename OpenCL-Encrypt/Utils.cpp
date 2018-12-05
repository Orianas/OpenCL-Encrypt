/**
 * @file Utils.cpp
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief Utility Function Definitions
 * @version 0.1
 * @date 2018-11-24
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "Utils.h"

#ifdef _WIN32
	const char sep = '\\';
#else
	const char sep = '/';
#endif



void printUsage(int argc, char* argv[]) {	
	std::string progName(argv[0]);

	std::string::size_type idx = progName.rfind(sep);
	progName = progName.substr(idx + 1);

	std::cout << "usage: " << progName
		<< "[-aes] [-ocl] [-dec | -enc] [-ip] [-p idx] [-d idx] [-h] [-i inFile]"
		<< "[-o outFile] inputFile [outFile]" << std::endl
		<< "\t-aes: \t\t\t\tSpecifies AES Encryption scheme (default behavior)." << std::endl
		<< "\t-ocl | -info: \t\t\tPrints out information for OpenCL devices." << std::endl
		<< "\t-dec | -decrypt: \t\tSpecifies decryption." << std::endl
		<< "\t-enc | -encrypt: \t\tSpecifies encryption." << std::endl
		<< "\t-ip  | -inplace: \t\tSpecifies encryption/decryption will happen inplace (infile overwritten)." << std::endl
		<< "\t-p   | -platform idx: \t\tSpecify index (idx) of platform to select." << std::endl
		<< "\t-d   | -device idx: \t\tSpecify index (idx) of device to select." << std::endl
		<< "\t-h   | -help    | /?: \t\tPrints the help (this information)." << std::endl
		<< "\t-i   | -input inFile: \t\tSpecify selected input file (inFile)." << std::endl
		<< "\t-o   | -output outFile: \tSpecify selected output file (outFile)." << std::endl
		<< std::endl << std::endl;
	for (int i = 0; i < argc; i++)
		std::cout << argv[i] << " " << std::endl;


}

std::vector<std::pair<std::string, std::string>> parseCommandLine(int argc, char* argv[])
{
	std::vector<std::pair<std::string, std::string>> returnVals;

	for (int i = 1; i < argc; i++) {
		std::string flag(argv[i]);
		if (flag[0] == '-') {
			if (flag[1] == '-')
				flag.erase(1, 1);
			if (flag == "-o" || flag == "-output")
				returnVals.push_back(parseDoubleOption(argc, argv, i++, "output"));
			else if (flag == "-i" || flag == "-input")
				returnVals.push_back(parseDoubleOption(argc, argv, i++, "input"));
			else if (flag == "-ip" || flag == "-inplace")
				returnVals.push_back(parseSingleOption(argc, argv, i, "inplace"));
			else if (flag == "-p" || flag == "-platform")
				returnVals.push_back(parseDoubleOption(argc, argv, i++, "platform"));
			else if (flag == "-d" || flag == "-device")
				returnVals.push_back(parseModalOption(argc, argv, i++, "device"));
			else if (flag == "-enc" || flag == "-encrypt")
				returnVals.push_back(parseSingleOption(argc, argv, i, "encrypt"));
			else if (flag == "-dec" || flag == "-decrypt")
				returnVals.push_back(parseSingleOption(argc, argv, i, "decrypt"));
			else if (flag == "-aes")
				returnVals.push_back(parseSingleOption(argc, argv, i, "aes"));
			else if (flag == "-ocl" || flag == "-info")
				returnVals.push_back(parseSingleOption(argc, argv, i, "info"));
			else if (flag == "-h" || flag == "-help" || flag == "/?") {
				printUsage(argc, argv);
				return std::vector<std::pair<std::string, std::string>>();
			}
		}
		else {
			if (i + 1 < argc && argv[i + 1][0] == '-') {
				std::cerr << "ERROR: Invalid command line arguments." << std::endl;
				printUsage(argc, argv);
				exit(-1);
			}
			if (i + 2 == argc) {
				returnVals.push_back(parseDoubleOption(argc, argv, i - 1, "input"));
				returnVals.push_back(parseDoubleOption(argc, argv, i++, "output"));
			}
			else
				returnVals.push_back(parseDoubleOption(argc, argv, i - 1, "input"));
		}
	}
	return returnVals;
}

std::pair<std::string, std::string> parseSingleOption(int argc, char* argv[], int index, std::string flag) {
	std::pair<std::string, std::string> returnVal;
	returnVal.first = flag;
	returnVal.second = "";
	return returnVal;
}

std::pair<std::string, std::string> parseDoubleOption(int argc, char* argv[], int index, std::string flag) {
	if (index + 1 < argc) {
		if (argv[index + 1][0] == '-') {
			std::cerr << "Unexpected argument following " << argv[index] << std::endl;
			exit(-1);
		}
	} 
	else {
		std::cerr << "Unexpected end of command line following " << argv[index] << std::endl;
	}
	std::pair<std::string, std::string> returnVal;
	returnVal.first = flag;
	returnVal.second = std::string(argv[index + 1]);
	return returnVal;
}

std::pair<std::string, std::string> parseModalOption(int argc, char* argv[], int index, std::string flag) {
	std::pair<std::string, std::string> returnVal;
	if (index + 1 < argc) {
		if (argv[index + 1][0] == '-') 
			returnVal.second = "";
		else
			returnVal.second = std::string(argv[index + 1]);
	}
	else
		std::cerr << "Unexpected end of command line following " << argv[index] << std::endl;

	returnVal.first = flag;
	return returnVal;
}