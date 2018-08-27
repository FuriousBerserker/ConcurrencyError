#include <stdio.h>
#include <stdlib.h>

__global__ void shiftArray(unsigned* a, unsigned size) {
    int tid = blockDim.x * blockIdx.x + threadIdx.x;
    while (tid < size) {
        a[tid] = a[(tid + 1) % size];
        tid += blockDim.x;
    }
}

int main() {
    unsigned size = 100, i;
    unsigned size_in_byte = size * sizeof(unsigned);
    unsigned *array = (unsigned*)malloc(size_in_byte);
    unsigned *d_array;
    for (i = 0; i < size; i++) {
        array[i] = i;    
    }
    cudaMalloc(&d_array, size_in_byte);
    cudaMemcpy(d_array, array, size_in_byte, cudaMemcpyHostToDevice);

    shiftArray<<<1, 32>>> (d_array, size);

    cudaMemcpy(array, d_array, size_in_byte, cudaMemcpyDeviceToHost);

    for (i = 0; i < size; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }
    return 0;
}
