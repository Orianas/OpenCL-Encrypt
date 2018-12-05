/**
 * @file OCLEncrypt.cpp
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief OpenCL Encryption Implementation
 * @version 0.1
 * @date 2018-11-24
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "OCLEncrypt.h"

OCLEncrypt::OCLEncrypt() {

}

OCLEncrypt::~OCLEncrypt() {

}

void OCLEncrypt::execute()
{
	if (info)
		printOCLInfo();
	else {
		if (operation == "")
			inferOperation();
		if (operation == "encrypt")
			encryptFile();
		else if (operation == "decrypt")
			decryptFile();
		else {
			std::cerr << "Invalid Operation." << std::endl;
			exit(-1);
		}
	}

}

int OCLEncrypt::doCrypt(std::string mode, std::vector<char> &fileCont) {	
	cl_kernel kernel = deviceContext.getKernel((encryptType + "-kernel.cl").c_str(),
		mode.c_str());

	// Allocate Memory on Device
	cl_mem plainText = deviceContext.devMemAlloc(0, fileCont.size());
	cl_mem cipherText = deviceContext.devMemAlloc(1, fileCont.size());
	cl_mem rndKeys = deviceContext.devMemAlloc(2, rounds * 16);

	deviceContext.copyToDevice(&fileCont[0], 0, fileCont.size());

	unsigned char* roundKeys = nullptr;
	genRoundKeys(roundKeys, 16);
	deviceContext.copyToDevice(roundKeys, 2, rounds * 16);

	// How many iterations we need to break the encryption into
	cl_int iters = static_cast<cl_int>(fileCont.size() / 16);

	cl_int ret;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &plainText);
	OCLError(ret);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cipherText);
	OCLError(ret);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &rndKeys);
	OCLError(ret);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_int), &rounds);
	OCLError(ret);
	ret = clSetKernelArg(kernel, 4, sizeof(cl_int), &iters);
	OCLError(ret);

	ret = clFinish(deviceContext.getQueue());
	OCLError(ret);

	size_t iterations = fileCont.size() / 16;
	ret = clEnqueueNDRangeKernel(deviceContext.getQueue(), kernel, 1, NULL, 
		&iterations, NULL, 0, NULL, NULL);
	OCLError(ret);

	ret = clFinish(deviceContext.getQueue());
	OCLError(ret);

	deviceContext.copyToHost(&fileCont[0], 1, fileCont.size());

	delete[] roundKeys;
	
	return 0;
}

void OCLEncrypt::printOCLInfo()
{
	std::cout << "This option is currently disabled." << std::endl;
}

void OCLEncrypt::encryptFile() {
	std::vector<char> fileCont;
	fileCont = getFileCont(inputFilename.c_str());
		
	// Pad file out to multiple of 16 (128-bit)
	int padding = 0;
	if (fileCont.size() % 16 != 0) {
		padding = 16 - (fileCont.size() % 16);
		for (int i = padding; i > 0; i--)
			fileCont.push_back(' ');
	}

	std::string fileExtension = getFileExtension(inputFilename);

	// Configure end of file pad
	std::string eof = std::to_string(padding) + " " + fileExtension;
	while (eof.size() < 16)
		eof += ' ';

	doCrypt("encrypt", fileCont);
	
	for (int i = 0; i < 16; i++)
		fileCont.push_back(eof[i]);

	if (outputFilename == "")
		outputFilename = inputFilename + ".enc";

	if (!putFileCont((outputFilename).c_str(), fileCont)) {
		printf("Unable to output to file!");
		exit(-1);
	}
}

void OCLEncrypt::decryptFile() {
	std::vector<char> fileCont;
	fileCont = getFileCont(inputFilename.c_str());
	
	// Pull the Padded Info
	std::string padCont = "";
	for (auto i = (fileCont.size() - 16); i < fileCont.size(); i++)
		padCont += fileCont[i];
	for (int i = 16; i > 0; i--)
		fileCont.pop_back();

	// Pull the info from the string
	std::istringstream inStream(padCont);
	int pad;
	std::string fileExtension;
	inStream >> pad >> fileExtension;
	inputFilename = inputFilename.substr(0, (inputFilename.size() - fileExtension.size() - 1));

	//Decrypt & Drop the pads
	doCrypt("decrypt", fileCont);
	for (int i = pad; i > 0; i--)
		fileCont.pop_back();

	if (outputFilename == "")
		outputFilename = inputFilename + "." + fileExtension;
	if (!putFileCont((outputFilename).c_str(), fileCont)) {
		printf("Unable to output to file!");
		exit(-1);
	}
}

void OCLEncrypt::inferOperation()
{
	std::string inFileExtension = getFileExtension(inputFilename);
	std::string outFileExtension = getFileExtension(outputFilename);

	if (inFileExtension == "enc" || outFileExtension == "dec")
		operation = "decrypt";
	else if (outFileExtension == "enc")
		operation = "encrypt";
	else {
		std::cerr << "Operation not specified and unable to infer." << std::endl;
		exit(-1);
	}
}

std::string OCLEncrypt::getFileExtension(std::string filename)
{
	// Get the file extension
	std::string::size_type idx = filename.rfind('.');
	std::string fileExtension = "";
	if (idx != std::string::npos) {
		fileExtension = inputFilename.substr(idx + 1);
		inputFilename = inputFilename.substr(0, idx);
	}
	return fileExtension;
}

std::vector<std::string> OCLEncrypt::setOptions(std::vector<std::pair<std::string, std::string>> flags)
{
	std::vector<std::string> returnVals;
	for (auto flag : flags) {
		if (flag.first == "info")
			this->info = true;
		else if (flag.first == "device")
			this->device = std::stoi(flag.second);
		else if (flag.first == "platform")
			this->platform = std::stoi(flag.second);
	}

	if (!info)
		for (auto flag : flags) {
			if (flag.first == "output")
				this->outputFilename = flag.second;
			else if (flag.first == "input")
				this->inputFilename = flag.second;
			else if (flag.first == "inplace")
				this->inplace = true;
			else if (flag.first == "encrypt")
				this->operation = "encrypt";
			else if (flag.first == "decrypt")
				this->operation = "decrypt";
			else
				returnVals.push_back(flag.first);
		}

	return returnVals;
}

void OCLEncrypt::genRoundKeys(unsigned char* &roundKeys, size_t size) {
	if (size == 16)
		rounds = 11;

	roundKeys = new unsigned char[rounds * 16];
	for (int i = 0; i < rounds * 16; i++)
		roundKeys[i] = '0';

	unsigned char rotWords[4];

	for (int i = 0; i < size; i++)
		roundKeys[i] = key[i];
	for (int i = 1; i < rounds; i++) {
		size_t offset = size + (i - 1) * 16;

		// Calc Rotated Words
		rotWords[0] = sBox[roundKeys[offset - 3]];
		rotWords[1] = sBox[roundKeys[offset - 2]];
		rotWords[2] = sBox[roundKeys[offset - 1]];
		rotWords[3] = sBox[roundKeys[offset - 0]];

		// First word
		roundKeys[offset + 0] = roundKeys[offset - 16] ^ rotWords[0] ^ rCon[i];
		roundKeys[offset + 1] = roundKeys[offset - 15] ^ rotWords[1];
		roundKeys[offset + 2] = roundKeys[offset - 14] ^ rotWords[2];
		roundKeys[offset + 3] = roundKeys[offset - 13] ^ rotWords[3];

		// Second, third and fourth words
		((unsigned int*)roundKeys)[offset / 4 + 1] =
			((unsigned int*)roundKeys)[offset / 4 + 0] ^
			((unsigned int*)roundKeys)[offset / 4 - 3];
		((unsigned int*)roundKeys)[offset / 4 + 2] =
			((unsigned int*)roundKeys)[offset / 4 + 1] ^
			((unsigned int*)roundKeys)[offset / 4 - 2];
		((unsigned int*)roundKeys)[offset / 4 + 1] =
			((unsigned int*)roundKeys)[offset / 4 + 0] ^
			((unsigned int*)roundKeys)[offset / 4 + 3];
		((unsigned int*)roundKeys)[offset / 4 + 3] =
			((unsigned int*)roundKeys)[offset / 4 + 2] ^
			((unsigned int*)roundKeys)[offset / 4 - 1];
	}

}

void selectDeviceParams() {

}