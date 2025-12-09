cmake_minimum_required(VERSION 3.22)
project(DynamicArray LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(array INTERFACE)
target_include_directories(array INTERFACE ${CMAKE_SOURCE_DIR}/core)

add_executable(dynamic_array_example main.cpp)
target_link_libraries(dynamic_array_example PRIVATE array)

include(CTest)
if (BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)

    add_executable(array_tests tests/ArrayTests.cpp)
    target_link_libraries(array_tests
            PRIVATE
            array
            GTest::gtest
            GTest::gtest_main
    )

    add_test(NAME ArrayTests COMMAND array_tests)
endif()