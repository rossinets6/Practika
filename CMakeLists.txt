cmake_minimum_required(VERSION 3.16)

project(BorisMaherskaya VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appBorisMaherskaya
    main.cpp
)

qt_add_qml_module(appBorisMaherskaya
    URI BorisMaherskaya
    VERSION 1.0
    QML_FILES
        Main.qml
        Login.qml
        Guest.qml
        dopfile.qml
    SOURCES
        database.h
        database.cpp
)

set_target_properties(appBorisMaherskaya PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBorisMaherskaya
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::Quick Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS appBorisMaherskaya
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
