#include "OpenCLHelper.h"
#include <array>
#include <iostream>

int main()
{
	cl::Program program = CreateProgram("ProcessMultidimensionalArray.cl");
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();

	// int arr[3][2]
	const int numRows = 3;
	const int numCols = 2;
	const int count = numRows*numCols;
	std::array<std::array<int, numCols>, numRows> arr = { { {1, 1}, {2, 2}, {3, 3} } };

	cl::Buffer buf(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*count, arr.data());

	cl::Kernel kernel(program, "ProcessMultidimensionalArray");
	kernel.setArg(0, buf);

	cl::CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(2, 3));
	queue.enqueueReadBuffer(buf, CL_TRUE, 0, sizeof(int) * count, arr.data());

	//queue.finish();
	std::cin.get();

}