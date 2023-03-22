cmake_minimum_required(VERSION 2.8)

project(tanks)
add_executable(MyExecutable "main.cpp")
include_directories("./lib/")
include_directories('')
target_link_libraries(MyExecutable msfl-graphics msfl-windows msfl-audio msfl-system)
