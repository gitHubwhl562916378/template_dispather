cmake_minimum_required(VERSION 3.0.0)
project(FunctorDispather VERSION 0.1.0)

add_compile_options(-std=c++17)
FILE(GLOB SRC_FILES "*.cpp")
FILE(GLOB HEADER_FILES "*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
