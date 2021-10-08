cmake_minimum_required(VERSION 3.17)
project(fts_sarahan)

set(CMAKE_CXX_STANDARD 20)

add_executable(fts_sarahan main.cpp server.h session.h helper.h server.cpp session.cpp)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.76.0 REQUIRED system coroutine)

if(Boost_FOUND)
    target_link_libraries(fts_sarahan Boost::system Boost::coroutine)
endif()