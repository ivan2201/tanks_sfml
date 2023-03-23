cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

project(tanks)
add_executable(MyExecutable main.cpp mainmenu.cpp bullet.cpp eventcontroller.cpp field.cpp game.cpp resources.cpp tank.cpp fps.cpp windowmutex.cpp)
find_package(SFML COMPONENTS graphics system window audio REQUIRED)
if(NOT SFML_FOUND)
	message(SEND_ERROR "Failed to find SFML")
	return()
else()
	include_directories(${SFML_INCLUDE_DIR})
endif()
target_link_libraries(MyExecutable PUBLIC sfml-graphics sfml-system sfml-window sfml-audio)
