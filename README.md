# Device Info

Device Info is a tiny library that provides detailed information about the GPU hardware and its capabilities.

Allows users to query HW information purely offline. All information is stored in read only global memory.

Example: Quickly query the number of CUs on a specific GPU without going through a driver. Just need vendor, device, and revision ID.

## Build

- All you need is the source / header file.
- CMake support is provided / recommended.

## Requirements

- C++ 20
- CMake 3.25+

## NOTE: device_info library subject to breaking changes based on GPUOpen-Tools development needs

If you need an AMD library for querying HW information, specifications, etc.

Consider the following AMD libraries:
- [AMD GPU Services](https://gpuopen.com/amd-gpu-services-ags-library/)
- [AMD Device Library eXtra (ADLX)](https://gpuopen.com/adlx/)
