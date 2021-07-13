cmake_minimum_required(VERSION 3.19)
project(DAC7573 C)

set(CMAKE_C_STANDARD 99)

# add dependencies
include(cpm/CPM.cmake)
CPMAddPackage("gh:JeroenVandezande/LowLevelCInterfaces@1.0.6")

add_library(DAC7573 DAC7573.c DAC7573.h)
target_include_directories (DAC7573 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(DAC7573 PUBLIC LowLevelCInterfaces m)