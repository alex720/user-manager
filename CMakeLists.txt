cmake_minimum_required(VERSION 3.8)

project(ts3-user-manager)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

add_library(ts3-user-manager SHARED
        src/plugin.cpp
        src/sqlwrapper.cpp
        src/userwidget.cpp
        src/User-Manager.ui
        src/configDialog.h
        src/ConfigData.cpp
        src/configDialog.cpp
        src/help_functions.cpp
        src/UserWidget.ui

        src/ConfigData.h
        #src/GeneratedFiles/ui_User-Manager.h
        #src/GeneratedFiles/ui_UserWidget.h
        src/help_functions.h
        src/plugin.h
        src/ServerObject.h
        src/sqlwrapper.h
        src/User.h
        src/userwidget.hpp
)

target_link_libraries(ts3-user-manager PRIVATE Qt5::Core Qt5::Widgets)

#qt_finalize_executable(ts3-user-manager)

#add_executable(ts3-user-manager
#        src/plugin.cpp
#        src/sqlwrapper.cpp
#        src/userwidget.cpp
#        ui/User-Manager.ui
#        src/configDialog.h
#        src/ConfigData.cpp
#        src/configDialog.cpp
#        src/help_functions.cpp
#        ui/UserWidget.ui
#
#        src/ConfigData.h
#        src/GeneratedFiles/ui_User-Manager.h
#        src/GeneratedFiles/ui_UserWidget.h
#        src/help_functions.h
#        src/plugin.h
#        src/ServerObject.h
#        src/sqlwrapper.h
#        src/User.h
#        src/userwidget.hpp
#)

target_include_directories(ts3-user-manager PRIVATE src vendor/ts3client-pluginsdk-api26/)

set_property(TARGET ts3-user-manager PROPERTY CXX_STANDARD 17)
