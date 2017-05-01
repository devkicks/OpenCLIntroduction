#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include "CL/cl.hpp"

int main()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	_ASSERT(platforms.size() > 0);

	auto platform = platforms.front();
	auto platformname = platform.getInfo < CL_PLATFORM_NAME>();
	auto platformvendor = platform.getInfo<CL_PLATFORM_VENDOR>();
	auto platformversion = platform.getInfo<CL_PLATFORM_VERSION>();

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	
	_ASSERT(devices.size() > 0);

	auto device = devices.front();
	auto devicename = device.getInfo<CL_DEVICE_NAME>();
	auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
	auto version = device.getInfo<CL_DEVICE_VERSION>();
	auto deviceopenclver = device.getInfo<CL_DEVICE_OPENCL_C_VERSION>();
	auto devicemaxcomput = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
	auto devicelocalmem = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() / 1024; //KB
	auto deviceglobalmem = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() /  (1024*1024); //MB
	auto devicemaxmemalloc = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() / (1024 * 1024); // MB
	auto devicemaxworkgroup = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
	auto devicemaxworkitems = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
	
	
	return 1;
}