#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <CL/opencl.h>
 
// OpenCL kernel. Each work item takes care of one element of c
const char *kernelSource =                                       "\n" \
"__kernel void gemm(  __global float *a,                         \n" \
"                       __global float *b,                       \n" \
"                       __global float *c,                       \n" \
"                       const unsigned int n,			  \n" \
"			const unsigned int cols)                  \n" \
"{                                                                \n" \
"    //Get our global thread ID                                   \n" \
"    int id = get_global_id(0);                                   \n" \
"    int k = 0;							  \n" \
"                      						  \n" \
"    //int rows = n/cols;                                         \n" \
"    //Make sure we do not go out of bounds                       \n" \
"    if (id < n)                                                  \n" \
"    {								  \n" \
"      int x = cols*(id/cols);					  \n" \
"      int y = id%cols;						  \n" \
"      for(k = 0; k < cols/*rows*/; k++)			  \n" \
"      {							  \n" \
"          c[id] += a[x+k] * b[k*cols+y]; 			  \n" \
"      }							  \n" \
"    }								  \n" \
"}                                                                \n" \
                                                                 "\n" ;
 
int main( int argc, char* argv[] )
{
    // Length of vectors
    unsigned int n = 1048576;
    unsigned int cols = 1024;

    // Host input vectors
    float *h_a;
    float *h_b;
    // Host output vector
    float *h_c;
 
    // Device input buffers
    cl_mem d_a;
    cl_mem d_b;
    // Device output buffer
    cl_mem d_c;
 
    cl_platform_id cpPlatform;        // OpenCL platform
    cl_device_id device_id;           // device ID
    cl_context context;               // context
    cl_command_queue queue;           // command queue
    cl_program program;               // program
    cl_kernel kernel;                 // kernel
 
    // Size, in bytes, of each vector
    size_t bytes = n*sizeof(float);
 
    // Allocate memory for each vector on host
    h_a = (float*)malloc(bytes);
    h_b = (float*)malloc(bytes);
    h_c = (float*)malloc(bytes);
 
    // Initialize vectors on host
    int i, j, base;
    unsigned int rows = n/cols;
    for( i = 0; i < rows; i++ )
    {
	base = cols*i;
	for(j = 0; j < cols; j++)
	{
        	h_a[base+j] = cosf(j)*sinf(i);
        	h_b[base+j] = cosf(i)*sinf(j);
	}
    }
 
    size_t globalSize, localSize;
    cl_int err;
 
    // Number of work items in each local work group
    localSize = 64;
 
    // Number of total work items - localSize must be devisor
    globalSize = ceil(n/(float)localSize)*localSize;
 
    // Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);
 
    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
 
    // Create a context  
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
 
    // Create a command queue 
    queue = clCreateCommandQueue(context, device_id, 0, &err);
 
    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
                            (const char **) & kernelSource, NULL, &err);
 
    // Build the program executable 
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
 
    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "gemm", &err);
 
    // Create the input and output arrays in device memory for our calculation
    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);
 
    // Write our data set into the input array in device memory
    err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0,
                                   bytes, h_a, 0, NULL, NULL);
    err |= clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0,
                                   bytes, h_b, 0, NULL, NULL);
 
    // Set the arguments to our compute kernel
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);
    err |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &cols);
 
    // Execute the kernel over the entire range of the data set  
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                                              0, NULL, NULL);
 
    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);
 
    // Read the results from the device
    clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0,
                                bytes, h_c, 0, NULL, NULL );
 
    // Output
    FILE* outfile = fopen("out.gpu.txt", "w");
	
    for(i = 0; i < cols; i++)
    {
	base = cols*i;
	for(j = 0; j < cols; j++)
	{
 	    fprintf(outfile, "%f ", h_c[base+j]);
        }
        fprintf(outfile, "\n");
    }

    fclose(outfile);
    printf("Done.\n");
 
    // release OpenCL resources
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_c);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
 
    //release host memory
    free(h_a);
    free(h_b);
    free(h_c);
 
    return 0;
}