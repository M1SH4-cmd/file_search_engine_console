cmake_minimum_required(VERSION 3.30)
project(file_search_drive_console_v)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(lib/nlohmann_json)

set(HEADERS
        include/search_drive.h
)

set(SOURCES
        src/main.cpp
        src/search_drive.cpp
        include/json_parser.h
        src/json_parser.cpp)



add_executable(file_search_drive_console_v
        ${HEADERS}
        ${SOURCES})

target_include_directories(file_search_drive_console_v PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(file_search_drive_console_v PRIVATE
    nlohmann_json::nlohmann_json

)