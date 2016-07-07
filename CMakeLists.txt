cmake_minimum_required(VERSION 3.5)
project(cloudtrail_parse)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

find_package(RapidJSON)
#link_libraries(rapidjson)

add_executable(cloudtrail_parse ${SOURCE_FILES})