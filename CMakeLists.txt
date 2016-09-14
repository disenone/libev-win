cmake_minimum_required(VERSION 2.6.2)
project(libev CXX C)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(internal_utils.cmake)
config_compiler_and_linker()

include_directories(${PROJECT_SOURCE_DIR})
set(LIBEV_SOURCES "ev.c;event.c")

# build libkcpev
add_library(libev_static STATIC ${LIBEV_SOURCES})
target_link_libraries(libev_static ws2_32)

add_library(libev SHARED ${LIBEV_SOURCES})
target_link_libraries(libev ws2_32)

add_executable(libev_test libev_test.cpp)
target_link_libraries(libev_test libev_static)

