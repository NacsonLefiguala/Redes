cmake_minimum_required(VERSION 3.28)
project(RedesTarea)

set(CMAKE_CXX_STANDARD 17)

# Añadir ejecutable para el servidor
add_executable(Server server.cpp)
target_link_libraries(Server ws2_32)

# Añadir ejecutable para el cliente
add_executable(Client client.cpp)
target_link_libraries(Client ws2_32)
