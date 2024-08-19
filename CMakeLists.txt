# CMakeList.txt: cppserver 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("cppserver")
set(CMAKE_CXX_STANDARD 11)
# 搜索所有的源文件，加入SRCS变量中
aux_source_directory(./Log SRCS)

#本工程主程序文件 和 输出程序名称
#add_executable(softRenderer )
# 将源代码添加到此项目的可执行文件。
add_executable (cppserver "cppserver.cpp" "cppserver.h" ${SRCS})



