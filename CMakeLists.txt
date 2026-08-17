cmake_minimum_required(VERSION 3.15)
project(atlas CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(atlas
    src/main.cpp
)

target_include_directories(atlas PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(atlas PRIVATE
    unofficial::sqlite3::sqlite3
)
