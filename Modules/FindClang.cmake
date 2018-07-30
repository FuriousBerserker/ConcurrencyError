# output:
#  CLANG_FOUND
#  CLANG_VERSION
#  CLANG_C_COMPILER

function(isClangInstalled)
    find_program(CLANG_COMMAND NAMES "clang" PATHS ENV "PATH")
    if(CLANG_COMMAND)
        set(CLANG_FOUND 1 PARENT_SCOPE)
        execute_process(COMMAND ${CLANG_COMMAND} "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(REGEX MATCH "[0-9]+\.[0-9]+\.[0-9]+" CLANG_VERSION ${CLANG_VERSION_OUTPUT})
        set(CLANG_VERSION ${CLANG_VERSION} PARENT_SCOPE)
        set(CLANG_C_COMPILER ${CLANG_COMMAND} PARENT_SCOPE)
    else()
        set(CLANG_FOUND 0 PARENT_SCOPE)
    endif()
endfunction()

isClangInstalled()

if(${Clang_FIND_REQUIRED})
    if(${CLANG_FOUND})
        message(STATUS "Found Clang: ${CLANG_COMMAND} (found version \"${CLANG_VERSION}\")")
    else()
        message(FATAL_ERROR "Clang is not installed.") 
    endif()
endif()
