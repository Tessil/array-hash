cmake_minimum_required(VERSION 3.1)

project(tsl_array_hash)

add_library(tsl_array_hash INTERFACE)
# Use tsl::array_hash as target, more consistent with other libraries conventions (Boost, Qt, ...)
add_library(tsl::array_hash ALIAS tsl_array_hash)

target_include_directories(tsl_array_hash INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(tsl_array_hash INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_growth_policy.h"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_hash.h"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_map.h"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_set.h")


if(${CMAKE_VERSION} VERSION_GREATER "3.7") 
    # Only available since version 3.8
    target_compile_features(tsl_array_hash INTERFACE cxx_std_11)
endif()
