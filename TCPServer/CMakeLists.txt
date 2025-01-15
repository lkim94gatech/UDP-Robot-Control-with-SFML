cmake_minimum_required(VERSION 3.10)
 
project(Server VERSION 1.0)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
 
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/udpServer.cpp)
 
add_executable(Server ${SOURCES})
 
include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)
 
link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)
 
target_link_libraries(Server PUBLIC sfml-graphics sfml-system sfml-window sfml-network)
 
set_target_properties(
    Server PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)