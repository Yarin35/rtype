cmake_minimum_required(VERSION 3.10)
project(RType)

set(CMAKE_CXX_STANDARD 17)

# Add include directory
include_directories(include)

# 🔄 SFML
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# 🔄 Common library for shared code (UDP, InputMessage)
add_library(RTypeCommon
    src/network/Protocol.cpp
    src/network/UDPClient.cpp
    src/network/UDPServer.cpp
)

# 🟢 Server executable
add_executable(RTypeServer
    src/server/ServerMain.cpp
    src/server/ServerLogic.cpp
)
target_link_libraries(RTypeServer RTypeCommon pthread)

# 🔵 Client executable
add_executable(RTypeClient 
    src/client/ClientMain.cpp
    src/network/UDPClient.cpp
)

target_link_libraries(RTypeClient 
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
)
