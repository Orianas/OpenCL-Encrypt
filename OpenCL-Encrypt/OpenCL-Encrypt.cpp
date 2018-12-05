/**
 * @file OpenCL-Encrypt.cpp
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief Driver Application for OpenCL Encrypt Kernels
 * @version 0.1
 * @date 2018-11-24
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "stdfx.h"
#include "OCLEncrypt.h"

int main(int argc, char* argv[]) {
	std::vector<std::pair<std::string, std::string>> flags;

	if (argc < 2) {
		printUsage(argc, argv);
		exit(-1);
	}
	else
		flags = parseCommandLine(argc, argv);

	OCLEncrypt device;
	std::vector<std::string> unrecognized = device.setOptions(flags);
	if (unrecognized.size() != 0)
	{
		std::cerr << "Unrecognized Flags: ";
		for (std::string flag : unrecognized)
			std::cerr << flag << " ";
		std::cerr << std::endl;
		exit(-1);
	}
	
	device.execute();

    return 0;
}
