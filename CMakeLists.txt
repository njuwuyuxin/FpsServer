cmake_minimum_required (VERSION 2.6)

add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")

project (FpsServer)

include_directories(include)
add_subdirectory(src)

# 顺序不可修改，先link_directories 再 add_executable 最后 target_link_libraries
link_directories(/usr/local/lib)

#FILE(GLOB SOURCE_FILES "*.cpp" "*.h")
add_executable(FpsServer main.cpp)

target_link_libraries(FpsServer src)
target_link_libraries(FpsServer -lconfig++)