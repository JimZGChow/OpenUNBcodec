cmake_minimum_required(VERSION 3.14)

project(OpenUNBCodec LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(OpenUNBCodecLib SHARED
    src/OpenUNBEncoderLL.cpp include/OpenUNBEncoderLL.h
    src/OpenUNBDecoderLL.cpp include/OpenUNBDecoderLL.h
    src/OpenUNBCodecHL.cpp include/OpenUNBCodecHL.h
)

include_directories(include/)

add_executable (OpenUNBCodecTest "tests/OpenUNBCodecTest.cpp")
target_link_libraries (OpenUNBCodecTest LINK_PUBLIC OpenUNBCodecLib)

install(DIRECTORY include/ DESTINATION /usr/include/OpenUNB)
install(TARGETS OpenUNBCodecLib DESTINATION /usr/lib)
