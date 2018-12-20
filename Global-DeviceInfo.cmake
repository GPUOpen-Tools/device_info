## Copyright (c) 2018 Advanced Micro Devices, Inc. All rights reserved.
cmake_minimum_required(VERSION 3.5.1)

## Define Device Info directory
set(DEVICE_INFO ${COMMON_DIR}/Src/DeviceInfo)

## Include directory
set(ADDITIONAL_INCLUDE_DIRECTORIES ${ADDITIONAL_INCLUDE_DIRECTORIES}
                                   ${DEVICE_INFO})

## Define device info sources
set(DEVICE_INFO_SRC
    DeviceInfoUtils.cpp)

set(DEVICE_INFO_HEADERS
    DeviceInfo.h
    DeviceInfoUtils.h)
