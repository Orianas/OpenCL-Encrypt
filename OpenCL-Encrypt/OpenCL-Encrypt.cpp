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

    // TODO: Impliment argment checking later - Hardcode for now
    //if( argc < 3 )
    //   printUsage(argc, argv);
    std::string file = "./test.txt";
	OCLEncrypt device{file};
	
	device.encryptFile();
	device.decryptFile();

    return 0;
}
