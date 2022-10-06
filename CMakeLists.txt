cmake_minimum_required(VERSION 3.18.2)

project(kmean VERSION 1.0)

# set everything up for c++ 17 features
set(CMAKE_CXX_STANDARD 17)

# test that filesystem header actually is there and works

add_executable(kmean main/main.cpp kmean/graph.hpp)
set_target_properties(kmean PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/exec")
#------------------------------------------------------------------------------
# default release build
#------------------------------------------------------------------------------

# set compilation flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release ")
  set(
	  CMAKE_BUILD_TYPE Release
    CACHE
    STRING "Choose the type of build."
    FORCE
  )
  # Set the possible values of build type for cmake-gui
  set_property(
    CACHE
    CMAKE_BUILD_TYPE
    PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
  )

endif()


message("PROJECT_NAME:" ${PROJECT_NAME})
message("PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})
message("CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})

# Turn on the verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

# Library-specific variable
#set(GAP_3RD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rd-party)


# -----------------------------------------------------------------------------
# must-have package include
# -----------------------------------------------------------------------------

# Enable test
include(CTest)

# -----------------------------------------------------------------------------
# include sub cmake list
# -----------------------------------------------------------------------------

# add sub directories
#add_subdirectory(kmean)
#add_subdirectory(main)
#add_subdirectory(unittests)

