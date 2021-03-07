cmake_minimum_required(VERSION 3.10)

project(PiqueCryptoTestSuite)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../include)

add_executable(PiqueCryptoTestSuite testsuite.cpp)
target_link_libraries(PiqueCryptoTestSuite ${GTEST_LIBRARIES} pthread)
