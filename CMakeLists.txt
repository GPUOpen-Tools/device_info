## Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

cmake_minimum_required(VERSION 3.25)

project(DEVICE_INFO LANGUAGES CXX)

add_library(device_info STATIC)
add_library(AMD::device_info ALIAS device_info)
target_sources(device_info
    PRIVATE
        DeviceInfo.cpp
        DeviceInfoUtils.cpp
    PUBLIC
        FILE_SET public_headers
        TYPE "HEADERS"
        BASE_DIRS .
        FILES
            DeviceInfo.h
            DeviceInfoUtils.h
)

target_compile_features(device_info PUBLIC cxx_std_17)
