cmake_minimum_required(VERSION 3.15)
project(signing_only_wallet)

set(CMAKE_CXX_STANDARD 14)

include_directories(uBitcoin/src uBitcoin/src/utility uBitcoin/src/utility/trezor)

add_executable(signing_only_wallet main.cpp)

