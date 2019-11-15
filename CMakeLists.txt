cmake_minimum_required(VERSION 3.15)
project(signing_only_wallet)

set(CMAKE_CXX_STANDARD 14)
set(BOOST_ROOT "/usr/include/boost")

find_package(Boost 1.65.1)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()

add_executable(signing_only_wallet main.cpp)

