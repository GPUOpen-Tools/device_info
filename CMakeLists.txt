#/ Copyright (C) Advanced Micro Devices, Inc. All rights reserved.

cmake_minimum_required(VERSION 3.25)

project(DEVICE_INFO VERSION 1.1.0 LANGUAGES CXX)

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

target_compile_features(device_info PUBLIC cxx_std_20)

if (MSVC)
    target_compile_options(device_info PRIVATE /W4 /WX)
else()
    target_compile_options(device_info PRIVATE -Wall -Wextra -Werror)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(device_info PRIVATE
            -Wconversion
            -Wimplicit-fallthrough
            -Wstring-conversion
        )
    endif()
endif()

if (PROJECT_IS_TOP_LEVEL)
    set_target_properties(device_info PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS device_info
        EXPORT device_info-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT device_info-targets
        FILE device_info-targets.cmake
        NAMESPACE AMD::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/device_info
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/device_info-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/device_info-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/device_info
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/device_info-config-version.cmake
        # device_info follows semantic versioning, so we specify compatibility with the same major version.
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/device_info-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/device_info-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/device_info
    )
endif()
