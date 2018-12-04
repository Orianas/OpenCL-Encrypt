/**
 * @file OCL-Utils.cpp
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief OpenCL Utility Function Definition File
 * @version 0.1
 * @date 2018-11-24
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "OCL-Utils.h"

int chooseBestPlatform(cl_platform_id* platforms) {
	//TODO: Implement proper choosing algorithm.
	return 0;
}

int chooseBestDevice(cl_device_id* devices) {
	//TODO: Implement proper choosing algorithm.
	return 0;
}

void printPlatformInfo(cl_platform_id platID) {
	size_t length;
	cl_int ret = clGetPlatformInfo(platID, CL_PLATFORM_NAME, NULL, NULL, &length);
	OCLError(ret);

	char* info = new char[length];
	ret = clGetPlatformInfo(platID, CL_PLATFORM_NAME, length, info, NULL);
	OCLError(ret);

	printf("%s\n", info);
	delete[] info;
}

void printDeviceInfo(cl_device_id devID) {
	size_t length;
	cl_int ret = clGetDeviceInfo(devID, CL_DEVICE_NAME, NULL, NULL, &length);
	OCLError(ret);

	char* info = new char[length];
	ret = clGetDeviceInfo(devID, CL_DEVICE_NAME, length, info, NULL);
	OCLError(ret);

	printf("%s\n", info);
	delete[] info;

	cl_ulong size;
	ret = clGetDeviceInfo(devID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size), &size, NULL);
	OCLError(ret);
	printf("Total device memory: %d MB\n", static_cast<int>(size >> 20));

	ret = clGetDeviceInfo(devID, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size), &size, NULL);
	OCLError(ret);
	printf("Maximum buffer size: %d MB\n", static_cast<int>(size >> 20));
}

std::vector<char> getFileCont(const char* filename) {
	std::ifstream inFile(filename, std::ios::in | std::ios::binary);
	std::vector<char> cont{};
	if (inFile) {
		inFile.seekg(0, std::ios::end);
		cont.resize((int)inFile.tellg());
		inFile.seekg(0, std::ios::beg);
		inFile.read(&cont.front(), cont.size());
		inFile.close();
	}
	return cont;
}

bool putFileCont(const char* filename, std::vector<char> &cont) {
	std::ofstream outFile(filename, std::ios::out | std::ios::binary);
	if (outFile) {
		outFile.seekp(0, std::ios::beg);
		outFile.write(&cont.front(), cont.size());
		outFile.close();
		return true;
	}
	else
		return false;
}

bool OCLError(cl_int errorCode) {
	if (errorCode != CL_SUCCESS)
	{
		if (errorCode == CL_DEVICE_NOT_FOUND)
			printf("CL_DEVICE_NOT_FOUND\n");
		if (errorCode == CL_DEVICE_NOT_AVAILABLE)
			printf("CL_DEVICE_NOT_AVAILABLE\n");
		if (errorCode == CL_COMPILER_NOT_AVAILABLE)
			printf("CL_COMPILER_NOT_AVAILABLE\n");
		if (errorCode == CL_MEM_OBJECT_ALLOCATION_FAILURE)
			printf("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
		if (errorCode == CL_OUT_OF_RESOURCES)
			printf("CL_OUT_OF_RESOURCES\n");
		if (errorCode == CL_OUT_OF_HOST_MEMORY)
			printf("CL_OUT_OF_HOST_MEMORY\n");
		if (errorCode == CL_PROFILING_INFO_NOT_AVAILABLE)
			printf("CL_PROFILING_INFO_NOT_AVAILABLE\n");
		if (errorCode == CL_MEM_COPY_OVERLAP)
			printf("CL_MEM_COPY_OVERLAP\n");
		if (errorCode == CL_IMAGE_FORMAT_MISMATCH)
			printf("CL_IMAGE_FORMAT_MISMATCH\n");
		if (errorCode == CL_IMAGE_FORMAT_NOT_SUPPORTED)
			printf("CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
		if (errorCode == CL_BUILD_PROGRAM_FAILURE)
			printf("CL_BUILD_PROGRAM_FAILURE\n");
		if (errorCode == CL_MAP_FAILURE)
			printf("CL_MAP_FAILURE\n");
		if (errorCode == CL_MISALIGNED_SUB_BUFFER_OFFSET)
			printf("CL_MISALIGNED_SUB_BUFFER_OFFSET\n");
		if (errorCode == CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST)
			printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n");
		if (errorCode == CL_COMPILE_PROGRAM_FAILURE)
			printf("CL_COMPILE_PROGRAM_FAILURE\n");
		if (errorCode == CL_LINKER_NOT_AVAILABLE)
			printf("CL_LINKER_NOT_AVAILABLE\n");
		if (errorCode == CL_LINK_PROGRAM_FAILURE)
			printf("CL_LINK_PROGRAM_FAILURE\n");
		if (errorCode == CL_DEVICE_PARTITION_FAILED)
			printf("CL_DEVICE_PARTITION_FAILED\n");
		if (errorCode == CL_KERNEL_ARG_INFO_NOT_AVAILABLE)
			printf("CL_KERNEL_ARG_INFO_NOT_AVAILABLE\n");
		if (errorCode == CL_INVALID_VALUE)
			printf("CL_INVALID_VALUE\n");
		if (errorCode == CL_INVALID_DEVICE_TYPE)
			printf("CL_INVALID_DEVICE_TYPE\n");
		if (errorCode == CL_INVALID_PLATFORM)
			printf("CL_INVALID_PLATFORM\n");
		if (errorCode == CL_INVALID_DEVICE)
			printf("CL_INVALID_DEVICE\n");
		if (errorCode == CL_INVALID_CONTEXT)
			printf("CL_INVALID_CONTEXT\n");
		if (errorCode == CL_INVALID_QUEUE_PROPERTIES)
			printf("CL_INVALID_QUEUE_PROPERTIES\n");
		if (errorCode == CL_INVALID_COMMAND_QUEUE)
			printf("CL_INVALID_COMMAND_QUEUE\n");
		if (errorCode == CL_INVALID_HOST_PTR)
			printf("CL_INVALID_HOST_PTR\n");
		if (errorCode == CL_INVALID_MEM_OBJECT)
			printf("CL_INVALID_MEM_OBJECT\n");
		if (errorCode == CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
			printf("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n");
		if (errorCode == CL_INVALID_IMAGE_SIZE)
			printf("CL_INVALID_IMAGE_SIZE\n");
		if (errorCode == CL_INVALID_SAMPLER)
			printf("CL_INVALID_SAMPLER\n");
		if (errorCode == CL_INVALID_BINARY)
			printf("CL_INVALID_BINARY\n");
		if (errorCode == CL_INVALID_BUILD_OPTIONS)
			printf("CL_INVALID_BUILD_OPTIONS\n");
		if (errorCode == CL_INVALID_PROGRAM)
			printf("CL_INVALID_PROGRAM\n");
		if (errorCode == CL_INVALID_PROGRAM_EXECUTABLE)
			printf("CL_INVALID_PROGRAM_EXECUTABLE\n");
		if (errorCode == CL_INVALID_KERNEL_NAME)
			printf("CL_INVALID_KERNEL_NAME\n");
		if (errorCode == CL_INVALID_KERNEL_DEFINITION)
			printf("CL_INVALID_KERNEL_DEFINITION\n");
		if (errorCode == CL_INVALID_KERNEL)
			printf("CL_INVALID_KERNEL\n");
		if (errorCode == CL_INVALID_ARG_INDEX)
			printf("CL_INVALID_ARG_INDEX\n");
		if (errorCode == CL_INVALID_ARG_VALUE)
			printf("CL_INVALID_ARG_VALUE\n");
		if (errorCode == CL_INVALID_ARG_SIZE)
			printf("CL_INVALID_ARG_SIZE\n");
		if (errorCode == CL_INVALID_KERNEL_ARGS)
			printf("CL_INVALID_KERNEL_ARGS\n");
		if (errorCode == CL_INVALID_WORK_DIMENSION)
			printf("CL_INVALID_WORK_DIMENSION\n");
		if (errorCode == CL_INVALID_WORK_GROUP_SIZE)
			printf("CL_INVALID_WORK_GROUP_SIZE\n");
		if (errorCode == CL_INVALID_WORK_ITEM_SIZE)
			printf("CL_INVALID_WORK_ITEM_SIZE\n");
		if (errorCode == CL_INVALID_GLOBAL_OFFSET)
			printf("CL_INVALID_GLOBAL_OFFSET\n");
		if (errorCode == CL_INVALID_EVENT_WAIT_LIST)
			printf("CL_INVALID_EVENT_WAIT_LIST\n");
		if (errorCode == CL_INVALID_EVENT)
			printf("CL_INVALID_EVENT\n");
		if (errorCode == CL_INVALID_OPERATION)
			printf("CL_INVALID_OPERATION\n");
		if (errorCode == CL_INVALID_GL_OBJECT)
			printf("CL_INVALID_GL_OBJECT\n");
		if (errorCode == CL_INVALID_BUFFER_SIZE)
			printf("CL_INVALID_BUFFER_SIZE\n");
		if (errorCode == CL_INVALID_MIP_LEVEL)
			printf("CL_INVALID_MIP_LEVEL\n");
		if (errorCode == CL_INVALID_GLOBAL_WORK_SIZE)
			printf("CL_INVALID_GLOBAL_WORK_SIZE\n");
		if (errorCode == CL_INVALID_PROPERTY)
			printf("CL_INVALID_PROPERTY\n");
		if (errorCode == CL_INVALID_IMAGE_DESCRIPTOR)
			printf("CL_INVALID_IMAGE_DESCRIPTOR\n");
		if (errorCode == CL_INVALID_COMPILER_OPTIONS)
			printf("CL_INVALID_COMPILER_OPTIONS\n");
		if (errorCode == CL_INVALID_LINKER_OPTIONS)
			printf("CL_INVALID_LINKER_OPTIONS\n");
		if (errorCode == CL_INVALID_DEVICE_PARTITION_COUNT)
			printf("CL_INVALID_DEVICE_PARTITION_COUNT\n");
		
		exit(-1);
	}
	return true;
}
