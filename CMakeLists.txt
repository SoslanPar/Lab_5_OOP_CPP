cmake_minimum_required(VERSION 3.10)
project(Lab_5 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Main executable (demo)
if(EXISTS ${CMAKE_SOURCE_DIR}/src/main.cpp)
    add_executable(main_app src/main.cpp)
    target_include_directories(main_app PRIVATE ${CMAKE_SOURCE_DIR}/include)
endif()

# GoogleTest-based unit tests
enable_testing()
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)

# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(tests
    test/test_list.cpp
    test/test_memory_resource.cpp
)

target_link_libraries(tests gtest_main)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(tests)
