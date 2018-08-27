#include <stdio.h>

#define cudaErrorCheck(call)                                         \
    do {                                                             \
        cudaError_t cuErr = call;                                    \
        if (cudaSuccess != cuErr) {                                  \
            printf("CUDA Error - %s:%d: '%s'\n", __FILE__, __LINE__, \
                   cudaGetErrorString(cuErr));                       \
            exit(0);                                                 \
        }                                                            \
    } while (0)



#define BLOCK_NUM 100
#define BLOCK_DIM 512
__device__ int count = 0;

__global__ void getThreadNum() {
    int oldVal, returnVal;
    do {
        oldVal = count;
        returnVal = atomicCAS(&count, oldVal, oldVal + 1);
    } while (oldVal != returnVal);
}

int main() {
    int result = -1;
    getThreadNum<<<BLOCK_NUM, BLOCK_DIM>>>();
    cudaErrorCheck(cudaGetLastError());
    cudaDeviceSynchronize();
    int *count_addr;
    cudaErrorCheck(cudaGetSymbolAddress((void**)&count_addr, count));
    cudaErrorCheck(cudaMemcpy(&result, count_addr, sizeof(int), cudaMemcpyDeviceToHost)); 
    printf("thread num, expected = %d, actual = %d\n", BLOCK_NUM * BLOCK_DIM, result);
    return 0;
}


