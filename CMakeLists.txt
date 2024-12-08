cmake_minimum_required(VERSION 3.8)

project(stl_man)

find_package(SDL2 REQUIRED)

set(SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Parser.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include   
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} GL GLU)

