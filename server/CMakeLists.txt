cmake_minimum_required (VERSION 3.0)

add_library(server server.hpp server.cpp)

target_include_directories(server INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(server simple-websocket-server)
target_link_libraries(server nlohmann_json)

