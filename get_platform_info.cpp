#include<iostream>
#include<string.h>

#ifdef __APPLE__
#include<OpenCL/cl.h>
#else
#include<CL/cl.h>
#endif

int main(){
    cl_platform_id *platforms;
    uint num_platforms;
    int  i, err, platform_index = -1;

    char* ext_data;
    size_t ext_size;
    const char icd_ext[] = "cl_khr_icd";
    
    err = clGetPlatformIDs(1, NULL, &num_platforms);
    if(err<0){
        std::cout<<"Couldn't find any platforms."<<std::endl;
        exit(1);
    }

    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id*) * (num_platforms));
    clGetPlatformIDs(num_platforms, platforms, NULL);

    for (int i=0; i<num_platforms; i++){
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &ext_size);
        if(err < 0){
            std::cout<<"Couldn't read extension data."<<std::endl;
            exit(1);
        }

        ext_data = (char*)malloc(ext_size);
        clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, ext_size, ext_data, NULL);

        if(strstr(ext_data, icd_ext) != NULL){
            platform_index = i;
        }
        std::cout<<"-----------------------"<<i<<"------------------------"<<std::endl;
        std::cout<<ext_data<<std::endl;

        free(ext_data);
    }
    if(platform_index > -1){
        std::cout<<"Platform "<<i<<"supports the extension.\n"<<std::endl;
    }
    else{
        std::cout<<"No platform support this extension"<<std::endl;
    }
    free(platforms);
    return 0;
}