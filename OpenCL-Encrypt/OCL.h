/**
 * @file OCL.h
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief OpenCL management header
 * @version 0.1
 * @date 2018-11-24
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#pragma once

#include "stdfx.h"
#include "OCL-Utils.h"

class OCL {
    private:
        cl_platform_id         platformID = NULL;
        cl_device_id           deviceID = NULL;
        cl_context             context;
        cl_command_queue       commQueue;

		// Map Kernel and program to an english name
		std::map<std::string, std::pair<cl_program, std::map<std::string, cl_kernel>>> kernels;
		
		// Give each buffer and index number
		std::map<int, cl_mem> buffers;

		cl_program getProgram(const char* progName);

    public:
		OCL(unsigned int platformNum = 0, unsigned int deviceNum = 0);
		~OCL();

		cl_kernel getKernel(const char* programName, const char* kernelName);
		cl_mem devMemAlloc(int index, size_t size);
		void copyToDevice(void* buffer, int index, size_t size);
		void copyToHost(void* buffer, int index, size_t size);
		void printInfo();

		cl_command_queue getQueue() { return commQueue; };
};