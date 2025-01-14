cmake_minimum_required(VERSION 3.25)
project(aes-impl LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(EXECUTABLE_NAME "aes")

add_executable(${EXECUTABLE_NAME} "main.c" "galois.c" "utils.c")

target_include_directories(${EXECUTABLE_NAME} PRIVATE "./include")