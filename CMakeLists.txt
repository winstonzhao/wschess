cmake_minimum_required (VERSION 3.0)

project (ws-chess)

add_subdirectory("libs/simple-web-sockets")
add_subdirectory("libs/json")
add_subdirectory("server")

add_executable(main main.cpp)

target_link_libraries(main server)