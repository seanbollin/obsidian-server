cmake_minimum_required(VERSION 3.5.1)
project(ObsidianServer)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake) #Clion, with conanbuildinfo.cmake in build folder
    include(${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake)
else()
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) # Not CLion
endif()
conan_basic_setup()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wextra -lstdc++")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(ObsidianServer_VERSION_MAJOR 0)
set(ObsidianServer_VERSION_MINOR 1)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
find_package(PythonInterp)

add_subdirectory(tests)

include_directories(includes/beast/include/)

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(SRCS server.cpp models/room.h models/room.cpp)

add_executable(obsidian-server ${SRCS})

target_link_libraries(obsidian-server ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()
add_test(NAME RoomTest COMMAND tests)

# Add a target that runs cpplint.py
#
# Parameters:
# - TARGET_NAME the name of the target to add
# - SOURCES_LIST a complete list of source and include files to check
function(add_style_check_target TARGET_NAME SOURCES_LIST)

    if(NOT PYTHONINTERP_FOUND)
        return()
    endif()

    list(REMOVE_DUPLICATES SOURCES_LIST)
    list(SORT SOURCES_LIST)

    add_custom_target(${TARGET_NAME}
            COMMAND "${CMAKE_COMMAND}" -E chdir
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${PYTHON_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/cpplint.py"
            "--counting=detailed"
            "--extensions=cpp,hpp,h"
            "--linelength=80"
            ${SOURCES_LIST}
            DEPENDS ${SOURCES_LIST}
            COMMENT "Linting ${TARGET_NAME}"
            VERBATIM)

endfunction()

add_style_check_target(cpplint "${SRCS}")