cmake_minimum_required(VERSION 2.8.12)

set(SINGLEAPP-SOURCES
    qtsingleapplication.cpp
    qtlocalpeer.cpp
)

set(SINGLEAPP-MOC-HEADERS
    qtsingleapplication.h
    qtlocalpeer.h
)

if(USE_QT5)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Network REQUIRED)
    set(QT_LIBRARIES "")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
else()
    find_package(Qt4 COMPONENTS QtCore QtNetwork REQUIRED)
    include("${QT_USE_FILE}")
    add_definitions(${QT_DEFINITIONS})
    QT4_WRAP_CPP(SINGLEAPP-SOURCES-MOC ${SINGLEAPP-MOC-HEADERS})
    include_directories(${QT_MKSPECS_DIR}/default)
endif()

ADD_LIBRARY(qtsingleapplication STATIC
    ${SINGLEAPP-SOURCES}
    ${SINGLEAPP-SOURCES-MOC}
)

if(USE_QT5)
    qt5_use_modules(qtsingleapplication Network Widgets)
endif()
