cmake_minimum_required(VERSION 3.10)

project(libgoki)

include_directories(../libvcol/src/bstrlib)

set(LIBGOKI_C_FILES src/libgoki.c)

add_library(libgoki ${LIBGOKI_C_FILES})

