cmake_minimum_required(VERSION 3.16)
project(MyCpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_PREFIX_PATH  "${CMAKE_PREFIX_PATH};c:/Qt/6.2.4/msvc2019_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable( MyCpp
                WIN32
                main.cpp
                mainwindow.cpp)


target_include_directories(MyCpp PUBLIC
                           "${PROJECT_BINARY_DIR}")

target_link_libraries(MyCpp Qt6::Core Qt6::Widgets Qt6::Gui user32.lib gdi32.lib comdlg32.lib advapi32.lib) 

