# Copyright 2022 Jevgenij Christoforov. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.16)

if (${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build")
endif ()

if (TARGET vox2vfm)
	return()
endif ()

project(vox2vfm
	VERSION 0.0.1
	DESCRIPTION "Utility to convert MagicaVoxel models to Voxfield models"
	HOMEPAGE_URL "https://github.com/Animeska34/vox2vfm"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(VOXFIELD_CORE_BUILD_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(libraries/voxfield-core)

configure_file(
	cmake/defines.h.in
	vox2vfm/defines.h)

add_executable(vox2vfm
	source/main.cpp
	source/vox2vfm.cpp)
target_link_libraries(vox2vfm PRIVATE
	voxfield-core-static)
target_include_directories(vox2vfm PRIVATE
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libraries/opengametools/src)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set_target_properties(vox2vfm PROPERTIES
		OUTPUT_NAME "_vox2vfm")
endif ()

if (${CMAKE_BUILD_TYPE} MATCHES "Release" AND
	(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR
	${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
	add_custom_command(TARGET vox2vfm POST_BUILD
		COMMAND strip "$<TARGET_FILE:vox2vfm>"
		VERBATIM)
endif ()