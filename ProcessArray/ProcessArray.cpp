#include "OpenCLHelper.h"
#include <iostream>

int main()
{
	cl::Program program = CreateProgram("ProcessArray.cl");
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();

	std::vector<int> vec(1024);
//	std::fill(vec.begin(), vec.end(), 1);
	// if we want to fill this vector on the GPU - SEE GPUFILL Below

	cl_int err = 0;
	cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * vec.size(), vec.data(), &err);
	cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int)*vec.size(), nullptr, &err);
	cl::Kernel kernel(program, "ProcessArray", &err);
	err = kernel.setArg(0, inBuf);
	err = kernel.setArg(1, outBuf);

	cl::CommandQueue queue(context, device);

	// Offset to not set the first 10 elements - this is the case
	err = queue.enqueueFillBuffer(inBuf, 1, sizeof(int) * 10, sizeof(int)*(vec.size() - 10));
	err = queue.enqueueNDRangeKernel(kernel, cl::NullRange /*offset range two values for multiple dimensions: starting from which index of vec*/, cl::NDRange(vec.size()) /*Global size*/ /*, local size we leave this for now - compiler will generate */);
	err = queue.enqueueReadBuffer(outBuf, CL_FALSE, 0, sizeof(int) * vec.size(), vec.data());

	// As we have selected CL_FALSE for the blocking code - it will execute in the background and go the next line in code.
	// If we selecte CL_TRUE it will wait to finish and then go to next line
	// read back - we have to first wait for the queue items to finish
	// 
	// therefore we want to wait for it to finish

	// The guy on youtube had this as cl::finish - when infact this needs to be queue.finish() the finish function is related to the object - not a static function
	queue.finish();

	std::cin.get();
	//queue.enqueueReadBuffer()


}