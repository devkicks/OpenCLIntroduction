#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include "CL/cl.hpp"
#include <fstream>
#include <iostream>

int main()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);

	auto device = devices.front();

	std::ifstream helloWorldFile("HelloWorld.cl");
	if (helloWorldFile.fail())
		std::cout << "unable to open cl file" << std::endl;


	std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));
	std::cout << src << std::endl;

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	cl::Context context(device);
	cl::Program program(context, sources);

	auto err = program.build("-cl-std=CL1.2");

	char buf[16];

	std::cout << buf << std::endl;

	cl::Buffer memBuf(context, CL_MEM_WRITE_ONLY, sizeof(buf));
	cl::Kernel kernel(program, "HelloWorld", &err);
	kernel.setArg(0, memBuf);

	cl::CommandQueue queue(context, device);
	// enqueueTask ::  Executes the kernel only once
	queue.enqueueTask(kernel);
	queue.enqueueReadBuffer(memBuf, CL_TRUE, 0, sizeof(buf), (char*)buf);
	//queue.finish();
	std::cout << buf << std::endl;

	std::cin.get();

	return 1;
}