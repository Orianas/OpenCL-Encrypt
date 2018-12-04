/**
 * @file OCL.cpp
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief OpenCL management code
 * @version 0.1
 * @date 2018-11-24
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "OCL.h"

cl_program OCL::getProgram(const char* progName) {
	std::fstream kernelFile(progName, std::ios::in | std::ios::binary);
	std::string fileCont(
		(std::istreambuf_iterator<char>(kernelFile)),
		std::istreambuf_iterator<char>()
	);

	const char* kernelCharArray = fileCont.c_str();
	const size_t fileLen = fileCont.length();

	cl_int ret;
	cl_program program = clCreateProgramWithSource(context, 1,
		(const char**)&kernelCharArray, &fileLen, &ret);
	OCLError(ret);

	ret = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		size_t size;
		clGetProgramBuildInfo(program, deviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
		char* log = new char[size];
		clGetProgramBuildInfo(program, deviceID, CL_PROGRAM_BUILD_LOG, size, log, NULL);

		printf("\nKernel Build Log: \n");
		printf("%s\n", log);
		delete[] log;

		OCLError(ret);
	}
	return program;
}

OCL::OCL(unsigned int platformNum, unsigned int deviceNum) {

	// Find platforms and save the one we are using.
	cl_uint	numPlatforms;
	cl_int ret = clGetPlatformIDs(0, NULL, &numPlatforms);
	OCLError(ret);
	cl_platform_id* platforms = (cl_platform_id*)new cl_platform_id[numPlatforms];

	ret = clGetPlatformIDs(numPlatforms, platforms, NULL);
	OCLError(ret);
	if (platformNum >= numPlatforms) {
		delete[] platforms;
		return;
	}

	platformID = platforms[platformNum];
	delete[] platforms;

	// Find devices and save the one we are using.
	cl_uint	numDevices;
	ret = clGetDeviceIDs(platformID, CL_DEVICE_TYPE_ALL, NULL, NULL, &numDevices);
	OCLError(ret);
	cl_device_id* devices = (cl_device_id*)new cl_device_id[numDevices];

	ret = clGetDeviceIDs(platformID, CL_DEVICE_TYPE_ALL, numDevices,
		devices, &numDevices);
	OCLError(ret);
	if (deviceNum >= numDevices) {
		delete[] devices;
		return;
	}

	deviceID = devices[deviceNum];
	delete[] devices;

	// Create Context
	cl_context_properties props[3] = { CL_CONTEXT_PLATFORM,
			(cl_context_properties)platformID, 0 };
	context = clCreateContext(props, 1, &deviceID, NULL, NULL, &ret);
	OCLError(ret);

	// Create Command Queue
	commQueue = clCreateCommandQueue(context, deviceID, 0, &ret);
	OCLError(ret);
}

OCL::~OCL() {

}

cl_kernel OCL::getKernel(const char* programName, const char* kernelName) {
	// If we don't have a program
	if (kernels.find(programName) == kernels.end()) {
		cl_program program = getProgram(programName);

		kernels[programName] = std::pair<cl_program, std::map<std::string, cl_kernel>>
			(program, std::map<std::string, cl_kernel>());
	}

	// If we don't have a free kernel
	if (kernels[programName].second.find(kernelName) == kernels[programName].second.end()) {
		cl_int ret;
		cl_kernel kernel = clCreateKernel(kernels[programName].first, kernelName, &ret);
		OCLError(ret);

		kernels[programName].second[kernelName] = kernel;
	}
	
	return kernels[programName].second[kernelName];
}

cl_mem OCL::devMemAlloc(int index, size_t size) {
	cl_int ret;
	if (buffers.find(index) != buffers.end()) {
		ret = clReleaseMemObject(buffers[index]);
		OCLError(ret);
	}

	cl_mem mem = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, &ret);
	OCLError(ret);

	buffers[index] = mem;
	return mem;
}

void OCL::copyToDevice(void* buffer, int index, size_t size) {
	cl_int ret = clEnqueueWriteBuffer(commQueue, buffers[index], CL_TRUE, 0, size,
		buffer, 0, NULL, NULL);
	OCLError(ret);
}

void OCL::copyToHost(void* buffer, int index, size_t size) {
	cl_int ret = clEnqueueReadBuffer(commQueue, buffers[index], CL_TRUE, 0, size,
		buffer, 0, NULL, NULL);
	OCLError(ret);
}

void OCL::printInfo() {
	printf("Platform: ");
	printPlatformInfo(platformID);
	printf("Device: ");
	printDeviceInfo(deviceID);
}

