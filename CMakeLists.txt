cmake_minimum_required(VERSION 3.5)
include(GNUInstallDirs)


project(tsl-array-hash VERSION 0.7.1)

add_library(array_hash INTERFACE)
# Use tsl::array_hash as target, more consistent with other libraries conventions (Boost, Qt, ...)
add_library(tsl::array_hash ALIAS array_hash)

target_include_directories(array_hash INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

list(APPEND headers "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_growth_policy.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_hash.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_map.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_set.h")
target_sources(array_hash INTERFACE "$<BUILD_INTERFACE:${headers}>")




include(CMakePackageConfigHelpers)

## Install include directory
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/tsl"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")



## Create and install tsl-array-hashConfig.cmake
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/tsl-array-hashConfig.cmake.in"
                                "${CMAKE_CURRENT_BINARY_DIR}/tsl-array-hashConfig.cmake"
                                INSTALL_DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/tsl-array-hash")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/tsl-array-hashConfig.cmake"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/tsl-array-hash")



## Create and install tsl-array-hashTargets.cmake
install(TARGETS array_hash
        EXPORT tsl-array-hashTargets)

install(EXPORT tsl-array-hashTargets
        NAMESPACE tsl::
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/tsl-array-hash")



## Create and install tsl-array-hashConfigVersion.cmake
# tsl-array-hash is header-only and does not depend on the architecture.
# Remove CMAKE_SIZEOF_VOID_P from tsl-array-hashConfigVersion.cmake so that a
# tsl-array-hashConfig.cmake generated for a 64 bit target can be used for 32 bit
# targets and vice versa.
set(CMAKE_SIZEOF_VOID_P_BACKUP ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/tsl-array-hashConfigVersion.cmake"
                                COMPATIBILITY SameMajorVersion)
set(CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P_BACKUP})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/tsl-array-hashConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/tsl-array-hash")
