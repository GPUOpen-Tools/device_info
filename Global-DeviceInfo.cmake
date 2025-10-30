## Copyright (C) 2018-2025 Advanced Micro Devices, Inc.  All rights reserved. ##

cmake_minimum_required(VERSION 3.10)

message(WARNING "Global-DeviceInfo.cmake is deprecated use CMakeLists.txt instead.")

## Define Device Info directory
set(DEVICE_INFO ${CMAKE_CURRENT_LIST_DIR})

## Include directory
set(ADDITIONAL_INCLUDE_DIRECTORIES ${ADDITIONAL_INCLUDE_DIRECTORIES}
                                   ${DEVICE_INFO})

## Define device info sources
set(DEVICE_INFO_SRC
    ${CMAKE_CURRENT_LIST_DIR}/DeviceInfoUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DeviceInfo.cpp)

set(DEVICE_INFO_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/DeviceInfo.h
    ${CMAKE_CURRENT_LIST_DIR}/DeviceInfoUtils.h)
