cmake_minimum_required(VERSION 3.5.1)
project(ObsidianServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -lstdc++")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(ObsidianServer_VERSION_MAJOR 0)
set(ObsidianServer_VERSION_MINOR 1)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)

include_directories(includes/beast/include/)

add_executable(obsidian-server server.cpp)

target_link_libraries(obsidian-server ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})