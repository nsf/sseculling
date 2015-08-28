project(SSECULLING)
cmake_minimum_required(VERSION 2.8)

if (NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
endif()

set(PROJECT_INCLUDES ${CMAKE_SOURCE_DIR})

function(add_source_subdir NAME)
	file(GLOB SOURCES "${NAME}/*.cpp" "${NAME}/*.h")
	list(APPEND PROJECT_SOURCES ${SOURCES})
	set(PROJECT_SOURCES "${PROJECT_SOURCES}" PARENT_SCOPE)
endfunction()

add_source_subdir(Core)
add_source_subdir(Math)
add_source_subdir(.)
include_directories(${PROJECT_INCLUDES})
add_executable(sseculling ${PROJECT_SOURCES})
