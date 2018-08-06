cmake_minimum_required(VERSION 3.5)

project(tsl_array_hash_tests)

add_executable(tsl_array_hash_tests "main.cpp" 
                                    "array_bucket_test.cpp" 
                                    "array_map_tests.cpp" 
                                    "array_set_tests.cpp" 
                                    "policy_tests.cpp")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(tsl_array_hash_tests PRIVATE -std=c++11 -Werror -Wall -Wextra -Wold-style-cast -O3 -DTSL_DEBUG)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(tsl_array_hash_tests PRIVATE /bigobj /WX /W3 /DTSL_DEBUG)
endif()

# Boost::unit_test_framework
find_package(Boost 1.54.0 REQUIRED COMPONENTS unit_test_framework)
target_link_libraries(tsl_array_hash_tests PRIVATE Boost::unit_test_framework)   

# tsl::array_hash
add_subdirectory(../ ${CMAKE_CURRENT_BINARY_DIR}/tsl)
target_link_libraries(tsl_array_hash_tests PRIVATE tsl::array_hash)  
