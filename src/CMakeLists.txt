add_executable(BookingServer main.cpp BookingService.cpp)

target_include_directories(BookingServer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party
)

# No extra link needed for httplib or json (header-only)
