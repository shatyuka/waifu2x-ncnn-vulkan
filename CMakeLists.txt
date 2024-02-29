cmake_minimum_required(VERSION 3.9)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 11)

project(waifu2x-ncnn-vulkan)

add_subdirectory(src)
