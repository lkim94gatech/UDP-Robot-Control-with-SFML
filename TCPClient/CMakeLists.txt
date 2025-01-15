cmake_minimum_required(VERSION 3.10)
 
project(Client VERSION 1.0)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
 
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/udpClient.cpp)
 
add_executable(Client ${SOURCES})
 
include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)
 
link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)
 
target_link_libraries(Client PUBLIC sfml-graphics sfml-system sfml-window sfml-network)
 
set_target_properties(
    Client PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)