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

OCLEncrypt::OCLEncrypt(std::string filename) {
	this->filename = filename;
}

OCLEncrypt::~OCLEncrypt() {

}

int OCLEncrypt::doCrypt(std::string mode, std::vector<char> &fileCont) {	
	fileCont = getFileCont(filename.c_str());
	while (fileCont.size() % 16 != 0)
		fileCont.push_back(' ');
	//fileCont.push_back('\0');
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
	cl_int iters = fileCont.size() / 16;

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

	deviceContext.copyToHost(&fileCont[0], 1, (fileCont.size() * sizeof(unsigned char)));

	delete[] roundKeys;
	
	return 0;
}

int OCLEncrypt::encryptFile() {
	std::vector<char> cont;
	doCrypt("encrypt", cont);
	if (!putFileCont((filename + "encrypted").c_str(), cont)) {
		printf("Unable to output to file!");
		exit(-1);
	}
	return 0;
}

int OCLEncrypt::decryptFile() {
	std::vector<char> cont;
	filename = filename + "encrypted";
	doCrypt("decrypt", cont);
	if (!putFileCont((filename + "decrypted").c_str(), cont)) {
		printf("Unable to output to file!");
		exit(-1);
	}
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