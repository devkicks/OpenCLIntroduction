#include "OpenCLHelper.h"
#include <array>
#include <iostream>
#include <numeric>
int main()
{
	cl::Program program = CreateProgram("NumericalReduction.cl");
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();

	std::vector<int> vec(1024 * 2);
	int count = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		vec[i] = i;
		// contain index of the vec inside vec
		// vec[0] = 0;
		// vec[1] = 1;
		//  ...
		//  ...
		// vec[1023] = 1023;
		count += i;
	}

	cl_int err = 0;
	cl::Kernel kernel(program, "NumericalReduction", &err);
	auto workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device, &err);

	auto numWorkGroups = vec.size() / workGroupSize;

	cl::Buffer buf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * vec.size(), vec.data());

	cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * numWorkGroups);

	kernel.setArg(0, buf);
	kernel.setArg(1, sizeof(int) * workGroupSize, nullptr); // local memory - we dont need to allocate this - just need to pass the size
	kernel.setArg(2, outBuf);

	std::vector<int> outVec(numWorkGroups);

	cl::CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()), cl::NDRange(workGroupSize));
	queue.enqueueReadBuffer(outBuf, CL_TRUE, 0, sizeof(int) * outVec.size(), outVec.data());

	for (int i = 0; i < outVec.size(); i++)
	{
		std::cout << outVec[i] << std::endl;
	}
	auto sum = std::accumulate(outVec.cbegin(), outVec.cend(), 0);
	std::cin.get();


}