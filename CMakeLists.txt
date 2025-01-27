cmake_minimum_required(VERSION 3.20)
project(project)

set(CMAKE_CXX_STANDARD 17)

add_executable(project
        main.cpp
        parser.cpp
        parser.h
        filter_processing.cpp
        filter_processing.h
        filters.h
        filters.cpp
        BMP.h
        BMP.cpp
        )
