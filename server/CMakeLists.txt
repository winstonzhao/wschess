cmake_minimum_required (VERSION 3.0)

add_library(
  server
  server.hpp
  server.cpp
  communication/identification.hpp
  communication/identification.cpp
  communication/error.hpp
  communication/error.cpp
  communication/generic_response.hpp
  communication/generic_response.cpp
  communication/messages.hpp
  game/player.hpp
  game/player.cpp
  game/room.hpp
  game/room.cpp
  )

target_include_directories(server INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(server simple-websocket-server)
target_link_libraries(server nlohmann_json)

