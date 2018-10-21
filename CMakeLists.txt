cmake_minimum_required(VERSION 3.1)

project(tsl_array_hash VERSION 0.5.1)

add_library(array_hash INTERFACE)
# Use tsl::array_hash as target, more consistent with other libraries conventions (Boost, Qt, ...)
add_library(tsl::array_hash ALIAS array_hash)

target_include_directories(array_hash INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

target_sources(array_hash INTERFACE
               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_growth_policy.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_hash.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_map.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/include/tsl/array_set.h>)


# Only compatible with CMake version >= 3.8
if(${CMAKE_VERSION} VERSION_GREATER "3.7")
    target_compile_features(array_hash INTERFACE cxx_std_11)
endif()




# Installation (only compatible with CMake version >= 3.3)
if(${CMAKE_VERSION} VERSION_GREATER "3.2")
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    ## Install include directory
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/tsl
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})



    ## Create and install tsl_array_hashConfig.cmake
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tsl_array_hashConfig.cmake.in
                                  ${CMAKE_CURRENT_BINARY_DIR}/tsl_array_hashConfig.cmake
                                  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tsl_array_hash)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tsl_array_hashConfig.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tsl_array_hash)

            

    ## Create and install tsl_array_hashTargets.cmake
    install(TARGETS array_hash
            EXPORT tsl_array_hashTargets)
            
    install(EXPORT tsl_array_hashTargets
            NAMESPACE tsl::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tsl_array_hash)

            

    ## Create and install tsl_array_hashConfigVersion.cmake
    # tsl_array_hash is header-only and does not depend on the architecture.
    # Remove CMAKE_SIZEOF_VOID_P from tsl_array_hashConfigVersion.cmake so that a
    # tsl_array_hashConfig.cmake generated for a 64 bit target can be used for 32 bit
    # targets and vice versa.
    set(TSL_ARRAY_HASH_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
    unset(CMAKE_SIZEOF_VOID_P)
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/tsl_array_hashConfigVersion.cmake
                                    COMPATIBILITY SameMajorVersion)
    set(CMAKE_SIZEOF_VOID_P ${TSL_ARRAY_HASH_CMAKE_SIZEOF_VOID_P})

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tsl_array_hashConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tsl_array_hash)
endif()
