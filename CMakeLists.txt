cmake_minimum_required(VERSION 3.17)
project(simple-tcp-chat)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
  src/main.cpp
  src/chat/ChatClient.cpp
  src/chat/ChatServer.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(simple-tcp-chat ${SOURCE_FILES})

target_include_directories(simple-tcp-chat PRIVATE /usr/local/include ${CMAKE_CURRENT_SOURCE_DIR}/include)
