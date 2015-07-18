cmake_minimum_required (VERSION 3.2)
project (FOO)
include (FindLuaJIT.cmake)

add_library (luaw lua.cc)
target_include_directories (luaw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library (objlib lib.cc)
add_executable (demo main.cc obj.cc)
target_link_libraries (demo LINK_PUBLIC luaw objlib)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
