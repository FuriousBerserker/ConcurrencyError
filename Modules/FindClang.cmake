# output:
#  CLANG_FOUND
#  CLANG_VERSION
#  CLANG_C_COMPILER
#  CLANG_CPP_COMPILER

function(isClangInstalled)
    find_program(CLANG_COMMAND NAMES "clang" PATHS ENV "PATH")
    if(CLANG_COMMAND)
        set(CLANG_FOUND 1 PARENT_SCOPE)
        execute_process(COMMAND ${CLANG_COMMAND} "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(REGEX MATCH "[0-9]+\.[0-9]+\.[0-9]+" CLANG_VERSION ${CLANG_VERSION_OUTPUT})
        set(CLANG_VERSION ${CLANG_VERSION} PARENT_SCOPE)
    else()
        set(CLANG_FOUND 0 PARENT_SCOPE)
    endif()
endfunction()

isClangInstalled()
if(NOT ${CLANG_FOUND})
    message(FATAL_ERROR "Clang is not installed. The benchmarks are used to test clang's support to OpenMP target") 
elseif(CLANG_VERSION VERSION_LESS "7.0.0")
    message(FATAL_ERROR "Require clang version larger than 7.0,0")
endif()

