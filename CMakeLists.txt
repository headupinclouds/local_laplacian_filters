# Copyright (c) 2015, David Hirvonen
# All rights reserved.

cmake_minimum_required(VERSION 3.0)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.14.12.tar.gz"
  SHA1 "2d444140db0d53393b5490604006bb4f4896a444"
  LOCAL
)
  
project(LLF VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include (CheckCXXSymbolExists)
CHECK_CXX_SYMBOL_EXISTS ( log2 "cmath" HAVE_STD_LOG2 )
CHECK_CXX_SYMBOL_EXISTS ( ceil "cmath" HAVE_STD_CEIL )
add_definitions(-DHAVE_STD_LOG2=${HAVE_STD_LOG2})
add_definitions(-DHAVE_STD_CEIL=${HAVE_STD_CEIL})

set(is_android ${ANDROID}) # syntax compatibility
string(COMPARE EQUAL "${CMAKE_OSX_SYSROOT}" "iphoneos" is_ios)
string(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "Linux" is_linux)
string(COMPARE EQUAL "${CMAKE_GENERATOR}" "Xcode" is_xcode)

if(is_xcode)
  option(CVSTEER_DISABLE_DSYM "Disable generation of dSYM information" ON)
  if(CVSTEER_DISABLE_DSYM)
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf")
  endif()
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "Configuration types")
  set(CMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE NO)
endif()

## #################################################################
## Dependencies - OpenCV
## #################################################################

hunter_add_package(OpenCV)
find_package(OpenCV REQUIRED)

## #################################################################
## Library
## #################################################################

include_directories(.)

#add_subdirectory(cvsteer)
#add_subdirectory (test)

add_library(llf
  gaussian_pyramid.cpp
  gaussian_pyramid.h
  laplacian_pyramid.cpp
  laplacian_pyramid.h
  local_laplacian.h
  opencv_utils.cpp
  opencv_utils.h
  remapping_function.cpp
  remapping_function.h
  )

target_link_libraries(llf ${OpenCV_LIBS})

## #################################################################
## Executable
## #################################################################

add_executable(locallaplacian main.cpp)
target_link_libraries(locallaplacian llf ${OpenCV_LIBS})
install(TARGETS locallaplacian DESTINATION bin)

## #################################################################
## Install
## #################################################################

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Note: use 'targets_export_name'
configure_file("cmake/Config.cmake.in" "${project_config}" @ONLY)

install(
    TARGETS llf
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}/${PROJECT_NAME}"
)

install(
    FILES
    gaussian_pyramid.h # TODO: min subset?
    laplacian_pyramid.h
    local_laplacian.h
    opencv_utils.h
    remapping_function.h
    DESTINATION "${include_install_dir}/${PROJECT_NAME}"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)
