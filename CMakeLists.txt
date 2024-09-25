cmake_minimum_required(VERSION 3.29)
project(C1 C)

set(CMAKE_C_STANDARD 11)

add_executable(C1 main.c
        server.c
        client.c
        encoder.c)
