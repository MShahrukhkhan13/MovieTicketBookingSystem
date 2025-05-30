cmake_minimum_required(VERSION 3.16)

project(MovieBookingApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appMovieBookingApp
    main.cpp
)

qt_add_qml_module(appMovieBookingApp
    URI MovieBookingApp
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES ApiClient.h
        SOURCES ApiClient.cpp
)

if(WIN32)
    set_target_properties(appMovieBookingApp PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

target_link_libraries(appMovieBookingApp
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appMovieBookingApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
