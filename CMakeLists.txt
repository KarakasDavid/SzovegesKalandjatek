cmake_minimum_required(VERSION 3.25)
project(yay C)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/1.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/2.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/3.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/4.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/5.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/6.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/7.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/8.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/9.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/10.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/11.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(yay
        almodu_adat.h
        almodul_adat.c
        almodul_csata.c
        almodul_csata.h
        almodul_kiiras.c
        almodul_kiiras.h
        almodul_menuk.c
        almodul_menuk.h
        debugmalloc.h
        main.c)
