cmake_minimum_required(VERSION 3.20)
project("fluid_example")

# set cxx standard
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_STANDARD 11)

find_package (Threads REQUIRED)

if (CMAKE_BUILD_TYPE EQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-O0")
    endif()
elseif(CMAKE_BUILD_TYPE EQUAL "Release") 
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-O3")
    endif()
endif()

set(BUILD_SHARED_LIBS FALSE) # build using the static libraries

add_subdirectory(libs/SFML)

add_executable(fluid_example
               main.cpp
               FluidHelperFunctions.cpp
               FluidSim.cpp
               DrawRect.cpp)

target_link_libraries(fluid_example
                      sfml-window
                      sfml-graphics
                      Threads::Threads)

target_include_directories(fluid_example PUBLIC
                           $<TARGET_PROPERTY:sfml-window,INTERFACE_INCLUDE_DIRECTORIES>
                           $<TARGET_PROPERTY:sfml-graphics,INTERFACE_INCLUDE_DIRECTORIES>)
