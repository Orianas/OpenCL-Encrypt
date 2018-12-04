/**
 * @file OCL-Utils.h
 * @author Steve Gilland (steven.gilland@gmail.com)
 * @brief OpenCL Utility Function Header File
 * @version 0.1
 * @date 2018-11-24
 *
 * @copyright Copyright (c) 2018
 *
 */

#pragma once
#include "stdfx.h"

cl_program getProgram(const char* progName);
bool OCLError(cl_int errorCode);

int chooseBestPlatform(cl_platform_id* platforms);
int chooseBestDevice(cl_device_id* devices);

void printPlatformInfo(cl_platform_id platID);
void printDeviceInfo(cl_device_id devID);

std::vector<char> getFileCont(const char* filename);
bool putFileCont(const char* filename, std::vector<char> &cont);