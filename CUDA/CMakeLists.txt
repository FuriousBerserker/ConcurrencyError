find_package(CUDA REQUIRED)

set(GROUP_NAME CUDA)

set(CUDA_HOST_COMPILER "gcc")
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -arch=sm_60")

set(SUBDIR_LIST "")
listSubDir(SUBDIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})
foreach(DIR ${SUBDIR_LIST})
    file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${DIR}/*.cu)
    if (SRCS)
       CUDA_ADD_EXECUTABLE("${DIR}-cu" ${SRCS}) 
    endif()
endforeach()
