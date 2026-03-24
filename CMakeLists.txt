cmake_minimum_required(VERSION 3.10)
project(exe1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

file(GLOB SOURCES src/*.cpp)

set(Boost_USE_MULTITHREADED ON)
set(Boost_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS log)
find_package(ZLIB REQUIRED)

add_executable(exe1 ${SOURCES})

target_link_libraries(exe1 PUBLIC
    ${Boost_LIBRARIES}
    ZLIB::ZLIB
)

target_include_directories(exe1 PUBLIC
    "${PROJECT_BINARY_DIR}"
)

target_compile_options(exe1 PUBLIC -Wall -Wextra -Werror)

target_compile_definitions(exe1 PRIVATE
    $<$<CONFIG:Release>:RELEASE_BUILD>
)