project(dukto)
cmake_minimum_required(VERSION 2.8.12)

OPTION(USE_QT5 "Build with qt5" OFF)
OPTION(USE_UPDATER "Add updater for application" OFF)
OPTION(USE_SINGLE_APP "Enable only single instance" OFF)

OPTION(USE_NOTIFY_NATIVE_TRAY "For notifications use System Tray's Native" OFF)

FIND_PACKAGE(PkgConfig)
if(PKG_CONFIG_FOUND)
    PKG_CHECK_MODULES(LIBNOTIFY libnotify)
    if(LIBNOTIFY_FOUND)
        OPTION(USE_NOTIFY_LIBNOTIFY "For notifications use libnotify" OFF)
    endif(LIBNOTIFY_FOUND)
endif(PKG_CONFIG_FOUND)

if(USE_UPDATER)
    add_definitions("-DUPDATER")
endif()
if(USE_SINGLE_APP)
    add_definitions("-DSINGLE_APP")
endif()

if(USE_NOTIFY_NATIVE_TRAY)
    add_definitions("-DNOTIFY_NATIVE_TRAY")
endif()
if(USE_NOTIFY_LIBNOTIFY)
    add_definitions("-DNOTIFY_LIBNOTIFY")
    include_directories(${LIBNOTIFY_INCLUDE_DIRS})
    list(APPEND DUKTO_LIBS ${LIBNOTIFY_LIBRARIES})
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(USE_QT5)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Network REQUIRED)
    find_package(Qt5Declarative REQUIRED)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
else()
    find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork QtDeclarative REQUIRED)
    include("${QT_USE_FILE}")
    add_definitions(${QT_DEFINITIONS})
    include_directories(${QT_MKSPECS_DIR}/default)
endif()

set(DUKTO_HDR
    buddylistitemmodel.h
    duktowindow.h
    ipaddressitemmodel.h
    peer.h
    platform.h
    recentlistitemmodel.h
    settings.h
)

set(DUKTO_MOC_HDR
    destinationbuddy.h
    duktoprotocol.h
    guibehind.h
    systemtray.h
    theme.h
    qmlapplicationviewer/qmlapplicationviewer.h
)

set(DUKTO_SRC
    main.cpp
    guibehind.cpp
    platform.cpp
    buddylistitemmodel.cpp
    duktoprotocol.cpp
    ipaddressitemmodel.cpp
    recentlistitemmodel.cpp
    settings.cpp
    destinationbuddy.cpp
    duktowindow.cpp
    theme.cpp
    systemtray.cpp
    qmlapplicationviewer/qmlapplicationviewer.cpp
)

set(DUKTO_RESOURCES
    qml.qrc
)

if(USE_UPDATER)
    list(APPEND DUKTO_SRC updateschecker.cpp)
    list(APPEND DUKTO_MOC_HDR updateschecker.h)
endif()

if(WIN32)
    list(APPEND DUKTO_SRC ecwin7.cpp)
    list(APPEND DUKTO_HDR ecwin7.h)
endif()

if(USE_QT5)
    qt5_add_resources(DUKTO_RESOURCES_RCC ${DUKTO_RESOURCES})
else()
    QT4_WRAP_CPP(DUKTO_MOC ${DUKTO_MOC_HDR})
    QT4_ADD_RESOURCES(DUKTO_RESOURCES_RCC ${DUKTO_RESOURCES})
endif()

include_directories(qmlapplicationviewer)

if(USE_SINGLE_APP)
    add_subdirectory(qtsingleapplication)
    include_directories(qtsingleapplication)
endif()

add_executable(${PROJECT_NAME}
                ${DUKTO_HDR}
                ${DUKTO_SRC}
                ${DUKTO_MOC}
                ${DUKTO_RESOURCES_RCC})
if(USE_QT5)
    qt5_use_modules(${PROJECT_NAME} Gui Widgets Network Declarative)
    set(QT_LIBRARIES "")
endif()

if(USE_SINGLE_APP)
    add_dependencies(${PROJECT_NAME} qtsingleapplication)
    link_directories("${CMAKE_CURRENT_BINARY_DIR}/qtsingleapplication")
    target_link_libraries(${PROJECT_NAME} qtsingleapplication)
endif()

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${DUKTO_LIBS})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} libWs2_32 libole32 libNetapi32)
endif()

if(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME}
            DESTINATION bin)
    install(FILES dukto.png
            DESTINATION share/pixmaps/)
    install(FILES dukto.desktop
            DESTINATION share/applications/)
endif()
