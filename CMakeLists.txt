cmake_minimum_required(VERSION 3.15)
project(MovieBooking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add common include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/cpp-httplib)
include_directories(${CMAKE_SOURCE_DIR}/third_party/json)

# Main executable
add_executable(movie_booking
    src/main.cpp
    src/BookingService.cpp
    # Add other .cpp files here
)
