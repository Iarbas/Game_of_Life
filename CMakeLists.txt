cmake_minimum_required (VERSION 3.11)
project (Game_of_Life)

# ------------------------------------------------- Project Settings --------------------------------------------------
# Get the absolute path to the parent directory.
get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

message(STATUS "Installation path of the binaries, libs and headers: " ${PARENT_DIR})

# Define the locations all necessary files.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PARENT_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PARENT_DIR}/build/lib)
set(CMAKE_HEADERS_OUTPUT_DIRECTORY ${PARENT_DIR}/build/lib/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PARENT_DIR}/build/bin)

# C++ options.
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_REQUIRED_FLAGS -std=c++17 -lstdc++fs)

# The version number.
set (GAME_VERSION_MAJOR 1)
set (GAME_VERSION_MINOR 0)

# Standard directory for the config files.
set (CONFIG_DIRECTORY ${PARENT_DIR}/build/configs)

# Standard directory for the etc files.
set (ETC_DIRECTORY ${PARENT_DIR}/build/etc)

# Create the additional folders to organize the configs and etc in a better way.
add_custom_target(create_folders)

add_custom_command(
        TARGET create_folders PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_HEADERS_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CONFIG_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ETC_DIRECTORY})

# This command and the FindSFML.cmake file in cmake_modules is needed to find SFML.
# message(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(OpenGL)
find_package(SFML 2 REQUIRED COMPONENTS graphics window system )
find_package (PythonLibs REQUIRED)

# Configure a header file to pass some of the CMake settings to the source code
configure_file ( src/cmake_config.h.in ${CMAKE_HEADERS_OUTPUT_DIRECTORY}/cmake_config.h ) 

# ----------------------------------------------- Program Configuration -----------------------------------------------
include_directories(include
                    ${CMAKE_HEADERS_OUTPUT_DIRECTORY}
                    ${PYTHON_INCLUDE_DIRS})

# %%%%%%% Executable %%%%%%%
add_executable(${PROJECT_NAME} src/main.cpp src/visual2d.cpp src/lifeCycle.cpp src/python_wrapper.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME}
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${PYTHON_LIBRARIES}
    Config_Reader
    )

add_dependencies( ${PROJECT_NAME} copy_config_etc Config_Reader)

# %%%%%%% Library %%%%%%%
# Create a static library.
ADD_LIBRARY( Config_Reader STATIC src/readConfig.cpp )
target_link_libraries( Config_Reader stdc++fs )
add_dependencies( Config_Reader copy_headers_reader )

# ----------------------------------------------------- Aftermath -----------------------------------------------------
# %%%%%%% Custom Commands %%%%%%%
# Copy the headers in the projects own file managment. 
add_custom_target( copy_headers_reader )
add_dependencies( copy_headers_reader create_folders )

add_custom_command(
        TARGET copy_headers_reader PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy include/readConfig.hpp ${CMAKE_HEADERS_OUTPUT_DIRECTORY}/)
        
# Copy the config and etc files files in the projects own file managment. 
add_custom_target(copy_config_etc)
add_dependencies( copy_config_etc create_folders )

add_custom_command(
        TARGET copy_config_etc PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_LIST_DIR}/config/sim.config ${CONFIG_DIRECTORY}/sim.config
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_LIST_DIR}/etc/sansation.ttf ${ETC_DIRECTORY}/sansation.ttf)

