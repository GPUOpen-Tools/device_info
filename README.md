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

If you need a stable AMD library for querying HW information, specifications, etc., consider the options below.

Consider the following AMD libraries:
- [AMD Device Library eXtra (ADLX)](https://gpuopen.com/adlx/)
    - ADLX queries the drivers directly at runtime and is the recommended general-purpose library for querying HW information, display settings, and performance data.
- [AMD GPU Services (AGS)](https://gpuopen.com/amd-gpu-services-ags-library/)
    - AGS is gaming-focused and recommended for developers who need runtime HW queries with forward compatibility for new hardware in that context.
