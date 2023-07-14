#include<iostream>
#include<string>
#ifdef MAC
#include<OpenCL/cl.h>
#else
#include<CL/cl.h>
#endif

int main(){
    cl_context context;
    cl_uint ref_count;
    cl_platform_id platform;
    cl_device_id *devices;
    cl_int err;
    cl_uint num_devices;
    char name_data[48], ext_data[4096]; 

    err = clGetPlatformIDs(1, &platform, NULL);
    if(err<0){
        std::cout<<"Couldn't find any platforms."<<std::endl;
        exit(1);
    }

    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, NULL, &num_devices);
    if(err < 0){
        std::cout<<"Couldn't find any devices."<<std::endl;
        exit(1);
    }

    devices = (cl_device_id*)malloc(sizeof(cl_device_id*) *num_devices);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);

    context = clCreateContext(NULL, 1, devices, NULL, NULL, &err);
    if(err < 0){
        std::cout<<"Couldn't create a context"<<std::endl;
        exit(1);
    }

    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(ref_count), &ref_count, NULL);//1
    if(err < 0){
        std::cout<<"Couldn't read the reference count"<<std::endl;
        exit(1);
    }

    std::cout<<"Initializing reference count:"<<ref_count<<std::endl;
    clRetainContext(context);//2

    clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(ref_count), &ref_count, NULL);
    std::cout<<"Reference Count:"<<ref_count<<std::endl;
    clReleaseContext(context);//1
    clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(ref_count), &ref_count, NULL);
    std::cout<<"Reference Count:"<<ref_count<<std::endl;

    clReleaseContext(context);//0 
    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(ref_count), &ref_count, NULL);
    if(err < 0){
        std::cout<<"Couldn't read the reference count"<<std::endl;// 会在这里报错，猜测是因为当context_referenct为0的时候，context会被自动回收
        exit(1);
    }
    std::cout<<"Reference Count:"<<ref_count<<std::endl;
    return 0;
}