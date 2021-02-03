cmake_minimum_required(VERSION 3.17)

MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})
set(USER_DIR /Users/hualongzhang)
set(VCPKG ${USER_DIR}/vcpkg)
set(CMAKE_TOOLCHAIN_FILE "${VCPKG}/scripts/buildsystems/vcpkg.cmake")
set(Fruit_USE_STATIC_LIBS ON)
project(vcpkg_catch_demo)

set(CMAKE_CXX_STANDARD 20)

include_directories(${VCPKG}/installed/x64-osx/include)
link_directories(${VCPKG}/installed/x64-osx/lib)
link_libraries(
        ${Fruit_LIBRARIES}
)
add_executable(vcpkg_catch_demo main.cpp)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(vcpkg_catch_demo PRIVATE
        Catch2::Catch2
        fruit)
