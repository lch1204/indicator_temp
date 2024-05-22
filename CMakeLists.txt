cmake_minimum_required(VERSION 2.8.12)

project(indicator_temp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

add_executable(${PROJECT_NAME} "main.cpp" "temp.cpp" "temp.h")
find_package(Eigen3 REQUIRED)
target_link_libraries(indicator_temp Eigen3::Eigen)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
