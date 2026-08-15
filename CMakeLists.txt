cmake_minimum_required(VERSION 3.15)
project(atlas CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

#find_package(SQLite3 REQUIRED)

add_executable(atlas
    src/main.cpp
   )

#target_include_directories(atlas PRIVATE src ${SQLite3_INCLUDE_DIRS})
#target_link_libraries(atlas PRIVATE ${SQLite3_LIBRARIES})

# On Arch, SQLite3_LIBRARIES may resolve fine via find_package.
# If find_package(SQLite3) fails on your system, fall back to:
#   target_link_libraries(atlas PRIVATE sqlite3)
