cmake_minimum_required(VERSION 3.15...3.31)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  GSMProj
  VERSION 1.0
  LANGUAGES CXX)


include(CTest)

add_subdirectory(src)
add_subdirectory(tests)
