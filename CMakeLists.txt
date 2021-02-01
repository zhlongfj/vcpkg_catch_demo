cmake_minimum_required(VERSION 3.17)
set(CMAKE_TOOLCHAIN_FILE "/Users/hualongzhang/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(vcpkg_catch_demo)

set(CMAKE_CXX_STANDARD 20)

add_executable(vcpkg_catch_demo main.cpp)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(vcpkg_catch_demo PRIVATE Catch2::Catch2)