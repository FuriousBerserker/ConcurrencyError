find_package(OpenMP REQUIRED)
find_package(CUDA REQUIRED)
find_package(Clang REQUIRED)

set(CMAKE_C_COMPILER clang)
set(GROUP_NAME OpenMP)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

#file(GLOB SUBDIR_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR})
set(TARGET_OPTION "-fopenmp-targets=nvptx64-nvidia-cuda --cuda-path=${CUDA_TOOLKIT_ROOT_DIR}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_OPTION}")

#include(CheckCCompilerFlag)
#CHECK_C_COMPILER_FLAG(${CMAKE_C_FLAGS} SUPPORT_OFFLOADING)
#
#if(NOT SUPPORT_OFFLOADING)
#    message(FATAL_ERROR "${CMAKE_C_COMPILER} doesn't support openmp offloading flags")
#endif()

macro(listSubDir RESULT ROOT_PATH)
    file(GLOB FILES RELATIVE ${ROOT_PATH} *)
    set(${RESULT} "")
    foreach(FILE_NAME ${FILES})
        if(IS_DIRECTORY "${ROOT_PATH}/${FILE_NAME}")
            if (${ARGC} GREATER "2")
                set(FILE_NAME ${FILE_NAME}.${ARGV2})
            endif()
            list(APPEND ${RESULT} ${FILE_NAME})
        endif()
    endforeach()
endmacro()

macro(removeSuffix RESULT INPUT)
    string(FIND ${INPUT} "." DOT_POS REVERSE)
    string(SUBSTRING ${INPUT} "0" ${DOT_POS} ${RESULT})
endmacro()

set(SUBDIR_LIST "")
listSubDir(SUBDIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})
foreach(DIR ${SUBDIR_LIST})
    file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${DIR}/*.c)   
    if (SRCS)
        add_executable(${DIR} ${SRCS})
    endif()
endforeach()

# add target to generate PTX files
set(PTX_FILE_LIST "")
set(PTX_OPTION "${CMAKE_C_FLAGS} -S")
set(OUTPUT_PTX "output-ptx")
separate_arguments(PTX_OPTION)

foreach(DIR ${SUBDIR_LIST})
    set(PTX_FILE "${DIR}.s")
    file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${DIR}/*.c)
    if (SRCS)
        set(PTX_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PTX_FILE}")
        add_custom_command(OUTPUT ${PTX_FILE} COMMAND ${CMAKE_C_COMPILER} ${PTX_OPTION} ${SRCS} "-o" ${PTX_FILE_PATH} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        list(APPEND PTX_FILE_LIST ${PTX_FILE})
    endif()
endforeach()

add_custom_target(${OUTPUT_PTX} DEPENDS ${PTX_FILE_LIST})
