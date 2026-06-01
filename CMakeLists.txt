#/ Copyright (C) Advanced Micro Devices, Inc. All rights reserved.

cmake_minimum_required(VERSION 3.25)

project(DEVICE_INFO LANGUAGES CXX)

add_library(device_info STATIC)
add_library(AMD::device_info ALIAS device_info)
target_sources(device_info
    PRIVATE
        device_info.cpp
    PUBLIC
        FILE_SET public_headers
        TYPE "HEADERS"
        BASE_DIRS .
        FILES
            device_info.hpp
)

if (PROJECT_IS_TOP_LEVEL)
    # Used to verify that all headers in a target's PUBLIC and INTERFACE header sets can be included on their own.
    set_target_properties(device_info PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
endif()

target_compile_features(device_info PUBLIC cxx_std_20)

if (MSVC)
    target_compile_options(device_info PRIVATE
        # Reasonable warning level
        /W4
        # Compiler warnings as errors
        /WX
    )
else(${CMAKE_CXX_COMPILER_ID} MATCHES "(GNU|Clang)")
    target_compile_options(device_info PRIVATE
        -Wall
        -Wextra
        -Werror
    )
    if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_compile_options(device_info PRIVATE
            -Wconversion
            -Wimplicit-fallthrough
            -Wstring-conversion
        )
    endif()
endif()
