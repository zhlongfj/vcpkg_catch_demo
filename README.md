# vcpkg_catch_demo

# 安装vcpkg
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.sh

# 安装catch2
./vcpkg install catch2
#配置vcpkg所有工程可用
./vcpkg integrate install

# 项目配置
*假设项目名称是：`vcpkg_catch_demo`*

## 配置vcpkg工具链
### 通过clion GUI配置
在**Clion/Preferences/Build,Execution,Deployment/Cmake/CMake options**中填入以下信息：
`-DCMAKE_TOOLCHAIN_FILE=/Users/hualongzhang/vcpkg/scripts/buildsystems/vcpkg.cmake`，
如图：
![image.png](https://upload-images.jianshu.io/upload_images/1489066-61331f657c7ea447.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 通过cmakelists.txt配置
```
set(CMAKE_TOOLCHAIN_FILE "/Users/hualongzhang/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(vcpkg_catch_demo)
```
$\color{red}{注意：}$ 设置CMAKE_TOOLCHAIN_FILE要在project()命令之前

## 配置catch2
在项目的cmakelists.txt中加入以下内容
```
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(vcpkg_catch_demo PRIVATE Catch2::Catch2)
```
$\color{red}{注意：}$ 
* 要把以上两句放在`add_executable`后面，否则会出现以下错误：
```
CMake Error at CMakeLists.txt:8 (target_link_libraries):
  Cannot specify link libraries for target "vcpkg_catch_demo" which is not built by this
  project.
```
* 以下错误为没有配置好vcpkg工具链，如果已经严格按以上配置，但是还是出现该错误，则可以删除目录`cmake-build-debug`后，再右键点击*CMakelists.txt*，选择：*Reload Cmake Project*：
```
CMake Error at CMakeLists.txt:6 (find_package):
  Could not find a package configuration file provided by "Catch2" with any
  of the following names:

    Catch2Config.cmake
    catch2-config.cmake

  Add the installation prefix of "Catch2" to CMAKE_PREFIX_PATH or set
  "Catch2_DIR" to a directory containing one of the above files.  If "Catch2"
  provides a separate development package or SDK, be sure it has been
  installed.
```
# 通过cmakelists.txt配置的完整脚本**
```
cmake_minimum_required(VERSION 3.17)
set(CMAKE_TOOLCHAIN_FILE "/Users/hualongzhang/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(vcpkg_catch_demo)

set(CMAKE_CXX_STANDARD 20)

add_executable(vcpkg_catch_demo main.cpp)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(vcpkg_catch_demo PRIVATE Catch2::Catch2)
```

# vcpkg_catch_demo
[github](https://github.com/zhlongfj/vcpkg_catch_demo)
