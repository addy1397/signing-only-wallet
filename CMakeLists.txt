cmake_minimum_required(VERSION 3.15)
project(signing_only_wallet)

set(CMAKE_CXX_STANDARD 14)


include_directories(uBitcoin/src)

file(GLOB_RECURSE CPPSOURCES RELATIVE ${CMAKE_SOURCE_DIR} "uBitcoin/src/*.cpp")
file(GLOB_RECURSE CSOURCES RELATIVE ${CMAKE_SOURCE_DIR} "uBitcoin/src/utility/*.c")

add_executable(signing_only_wallet main.cpp ${CPPSOURCES} ${CSOURCES})

