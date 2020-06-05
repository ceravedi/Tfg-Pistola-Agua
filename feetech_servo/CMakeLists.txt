##---------------------------------------------------------------------------------------------------------------------
##  
##---------------------------------------------------------------------------------------------------------------------
##  Copyright 2020 Manuel Pérez Jiménez (a.k.a. manuoso) manuperezj@gmail.com
##---------------------------------------------------------------------------------------------------------------------
##  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
##  and associated documentation files (the "Software"), to deal in the Software without restriction,
##  including without limitation the rights to use, copy, modify, merge, publish, distribute,
##  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
##  furnished to do so, subject to the following conditions:
##
##  The above copyright notice and this permission notice shall be included in all copies or substantial
##  portions of the Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
##  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
##  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
##  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
##  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##---------------------------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
project(feetech_servo VERSION 0.1 LANGUAGES C CXX)

#########################################
###### Loading 3rd party libraries ######
#########################################

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
       FORCE )
endif()

#########################################
## Load Eigen
find_package(Eigen3 REQUIRED)
if(${Eigen3_FOUND})
	MESSAGE( STATUS "Eigen FOUND")
endif()

#########################################
## Load serial
find_package(serial REQUIRED)
if(${serial_FOUND})
	MESSAGE( STATUS "Serial FOUND")
endif()

#########################################
######			Library 		   ######
#########################################

# Collect all sources
file(GLOB_RECURSE FEETECH_SERVO_SOURCE_FILES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE FEETECH_SERVO_HEADER_FILES "include/*.h" "include/*.inl")

add_library(${PROJECT_NAME} SHARED ${FEETECH_SERVO_HEADER_FILES} ${FEETECH_SERVO_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

#########################################

target_include_directories(${PROJECT_NAME} PUBLIC "/usr/include/eigen3")

target_include_directories(${PROJECT_NAME} PUBLIC ${serial_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${serial_LIBRARIES})

#########################################
######			Examples 		   ######
#########################################

add_subdirectory(examples)
