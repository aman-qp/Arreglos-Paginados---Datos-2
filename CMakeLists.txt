cmake_minimum_required(VERSION 3.28)
project(ArreglosPag)

set(CMAKE_CXX_STANDARD 17)

# Ejecutable para el generador
add_executable(generator main.cpp PagedArray.cpp PagedArray.h)

# Ejecutable para el sorter
add_executable(sorter sorter.cpp PagedArray.cpp PagedArray.h)