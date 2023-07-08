# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (CamTest)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
set(CMAKE_CXX_STANDARD 11)



#添加元文件路径
#判断编译条件CPU是arm还是X86  
#cmake传参$      cmake -DTARGET_CPU:STRING=x86
if(TARGET_CPU STREQUAL "arm")
#添加ARM所需资源
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/LIBMODBUS/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/LIBMODBUS/lib/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/cJSON/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/cJSON/lib/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/Galaxy_camera/inc)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/Galaxy_camera/lib/armv8)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/opencv3/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/ARM64/opencv3/lib)

else()
#添加X86所需资源
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/X86/LIBMODBUS/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/env/X86/LIBMODBUS/lib/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../env/X86/cJSON/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../env/X86/cJSON/lib/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../env/X86/Galaxy_camera/inc)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../env/X86/Galaxy_camera/lib/x86_64)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../env/X86/opencv4/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../env/X86/opencv4/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/Base)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/Cam)
endif()

# 添加 子目录


# 指定生成目标 
add_executable(TestBrodcast TestBroadcast.cpp)
add_executable(TestCam TestCam.cpp)
# 添加链接库
target_link_libraries(TestBrodcast  LibCam Base  -lcjson -lgxiapi -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_flann -lpthread)
target_link_libraries(TestCam  LibCam Base  -lcjson -lgxiapi -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_flann -lpthread)
