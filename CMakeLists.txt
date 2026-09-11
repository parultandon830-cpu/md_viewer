cmake_minimum_required(VERSION 3.16)
project(MDReader LANGUAGES CXX)

Require C++17

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

Enable automatic Qt code generators (MOC handles Q_OBJECT macros)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

Find Qt 6 (or Qt 5 fallback) with required modules

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets PrintSupport)

Create the Windows GUI executable

Note: "WIN32" prevents a black console window from opening behind the app

add_executable(MDReader WIN32
main.cpp
)

Link Qt libraries

target_link_libraries(MDReader PRIVATE
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Gui
Qt${QT_VERSION_MAJOR}::Widgets
Qt${QT_VERSION_MAJOR}::PrintSupport
)
