cmake_minimum_required(VERSION 4.0)
project(vv)

set(CMAKE_CXX_STANDARD 20)

add_executable(vv main.cpp
        vvUtils.cpp
        vvUtils.h
        FileEntry.cpp
        FileEntry.h)

find_package(Curses REQUIRED)
target_link_libraries(vv PRIVATE ${CURSES_LIBRARIES})
target_include_directories(vv PRIVATE ${CURSES_INCLUDE_DIR})
