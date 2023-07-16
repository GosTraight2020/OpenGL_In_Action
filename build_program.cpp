#include<iostream>
#ifdef __APPLE__
#include<OpenCL/cl.h>
#else
#include<CL/cl.h>
#endif

#define NUM_FILES 2
#define PROGRAM_FILE_1 "good.cl"
#define PROGRAM_FILE_2 "bad.cl"

int main(){
    cl_program program;
    FILE *program_handle;
    char *program_buffer[NUM_FILES];
    char *program_log;
    const char *file_name[] = {PROGRAM_FILE_1, PROGRAM_FILE_2};
    const char options[] = "-cl-finite-math-only -cl-no-signed-zeros";
    size_t program_size[NUM_FILES];
    size_t log_size;
    cl_int err;


    // 获取平台
    cl_platform_id platform;
    cl_uint num_platform;
    err = clGetPlatformIDs(5, NULL, &num_platform);
    if(err < 0){
        std::cout<<"Couldn't find any platform"<<std::endl;
        exit(1);
    }
    clGetPlatformIDs(1, &platform, NULL);

    //获取设备
    cl_device_id *devices;
    cl_uint num_devices;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 5, NULL, &num_devices);
    if(err < 0){
        std::cout<<"Couldn't find any devices"<<std::endl;
        exit(1);
    }
    devices = (cl_device_id*) malloc(sizeof(cl_device_id*)*num_devices);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);
    
    //获取上下文
    cl_context context;
    context = clCreateContext(NULL, 1, devices, NULL, NULL, &err);
    if(err < 0){
        std::cout<<"Couldn't create context"<<std::endl;
        exit(1);
    }


    for(cl_int i=0; i<NUM_FILES; i++){
        program_handle = fopen(file_name[i], "r");
        if(program_handle == NULL){
            std::cout<<"Couldn't find the program file!"<<std::endl;
            exit(1);
        }
        fseek(program_handle, 0, SEEK_END);
        program_size[i] = ftell(program_handle);
        rewind(program_handle); // 文件内部的指针重新指向文件开头
        program_buffer[i] = (char*)malloc(program_size[i]+1);
        program_buffer[program_size[i]] = '\0';
        fread(program_buffer[i], sizeof(char), program_size[i], program_handle);
        fclose(program_handle);
    }

    program  = clCreateProgramWithSource(context, NUM_FILES, (const char**)program_buffer, program_size, &err);
    if(err < 0){
        std::cout<<"Couldn't create the program"<<std::endl;
        exit(1);
    }
    err = clBuildProgram(program, 1, devices, options, NULL, NULL);
    if(err < 0){
        clGetProgramBuildInfo(program, *devices, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        program_log = (char*) malloc (log_size+1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, *devices, CL_PROGRAM_BUILD_LOG, log_size+1, program_log, NULL);
        std::cout<<program_log<<std::endl;
        free(program_log);
    }

    for(cl_int i=0; i<NUM_FILES; i++){
        free(program_buffer[i]);
    }
}


