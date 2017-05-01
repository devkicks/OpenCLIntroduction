
#include "CL/cl.hpp"
#include "OpenCLHelper.h"
#include <fstream>
#include <string>
#include <iostream>

//namespace OpenCLHelper
//{
	cl::Program CreateProgram(const std::string &file)
	{
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		auto platform = platforms.front();
		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);

		auto device = devices.front();

		std::ifstream kernelFile(file);
		if (kernelFile.fail())
		{
			std::cout << "unable to open cl file" << std::endl;
			exit(EXIT_FAILURE);
		}


		std::string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));
		std::cout << src << std::endl;

		cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

		cl::Context context(device);
		cl::Program program(context, sources);

		auto err = program.build("-cl-std=CL1.2");

		if (err != 0)
		{
			std::cout << "Error building " << file << std::endl;
			exit(EXIT_FAILURE);
		}

		return program;
	}
//}