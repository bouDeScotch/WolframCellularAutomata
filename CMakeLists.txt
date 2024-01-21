cmake_minimum_required(VERSION 3.27)
project(WolframCellularAutomata)

set(CMAKE_CXX_STANDARD 17)

# Add the path to the SFML include directory and the SFML library directory
set(SFML_ROOT "Libs/SFML/include")
include_directories(${SFML_ROOT}/include)
link_directories(${SFML_ROOT}/lib)

# Locate the SFML library
# set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(WolframCellularAutomata main.cpp)

# Link the SFML libraries to your executable
target_link_libraries(WolframCellularAutomata sfml-graphics sfml-audio)