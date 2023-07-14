#include<iostream>
#include<string.h>

#ifdef MAC
#include<OpenCL/cl.h>
#else
#include<CL/cl.h>
#endif

int main(){
    cl_platform_id platform;
    cl_device_id *devices;
    cl_uint num_devices, addr_data;   
    cl_int i, err;
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

    for (i=0; i<num_devices; i++){
        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(name_data), name_data, NULL);
        if(err < 0){
            std::cout<<"Couldn't read extension data"<<std::endl;
            exit(1);
        }
        clGetDeviceInfo(devices[i], CL_DEVICE_ADDRESS_BITS, sizeof(ext_data), &addr_data, NULL);
        clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, sizeof(ext_data), ext_data, NULL);
        std::cout<<"name:"<<name_data<<std::endl;
        std::cout<<"address_width:"<<addr_data<<std::endl;
        std::cout<<"extension:"<<ext_data<<std::endl;
    }

    free(devices);





    return 0;
}