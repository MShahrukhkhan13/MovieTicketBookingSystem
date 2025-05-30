cmake_minimum_required(VERSION 3.14)
project(BookingService_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BookingService_test
    BookingService_test.cpp
    ../src/BookingService.cpp       
)

target_include_directories(BookingService_test PRIVATE
    ${PROJECT_SOURCE_DIR}/../include
    ${PROJECT_SOURCE_DIR}/../third_party/boost_1_88_0
)
