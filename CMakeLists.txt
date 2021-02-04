cmake_minimum_required(VERSION 3.17)

string(SUBSTRING ${CMAKE_CURRENT_SOURCE_DIR} 1 100 TEMP)
string(REPLACE "/" ";" SOURCE_DIR_LIST ${TEMP})
list(GET SOURCE_DIR_LIST 1 USER_NAME)
set(USER_DIR /Users/)
string(APPEND USER_DIR ${USER_NAME})

set(VCPKG ${USER_DIR}/vcpkg)
set(CMAKE_TOOLCHAIN_FILE "${VCPKG}/scripts/buildsystems/vcpkg.cmake")

project(vcpkg_catch_demo)

set(CMAKE_CXX_STANDARD 20)

link_directories(${VCPKG}/installed/x64-osx/lib)
link_libraries(
        ${Fruit_LIBRARIES}
)
add_executable(vcpkg_catch_demo main.cpp)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(vcpkg_catch_demo PRIVATE
        Catch2::Catch2
        fruit)
