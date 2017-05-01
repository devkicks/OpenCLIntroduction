#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

/* Hello World example using Generic CreateProgram code from OpenCLHelper */
#include "CL\cl.hpp"
#include <fstream>
#include <iostream>
#include "OpenCLHelper.h"

int main()
{
	auto program = CreateProgram("HelloWorld.cl");
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();

	char buf[16];
	cl::Buffer memBuf(context, CL_MEM_WRITE_ONLY, sizeof(buf));
	cl::Kernel kernel(program, "HelloWorld");
	kernel.setArg(0, memBuf);

	cl::CommandQueue queue(context, device);
	// enqueueTask ::  Executes the kernel only once
	queue.enqueueTask(kernel);
	queue.enqueueReadBuffer(memBuf, CL_TRUE, 0, sizeof(buf), buf);

	std::cout << buf;
	std::cin.get();

}