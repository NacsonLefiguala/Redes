# 4 en Línea con Sockets TCP en C++

Este proyecto implementa un juego de 4 en línea utilizando sockets TCP en C++. El juego sigue una arquitectura cliente-servidor y permite a dos jugadores (uno en el servidor y otro en el cliente) jugar en tiempo real a través de la red.

## Requisitos

- Windows con MinGW instalado
- CMake
- Git
- CLion (opcional, pero recomendado)

## Estructura del Proyecto

- `client.cpp`: Implementación del cliente que se conecta al servidor para jugar.
- `server.cpp`: Implementación del servidor que maneja el juego y la conexión con el cliente.
- `CMakeLists.txt`: Archivo de configuración de CMake para compilar el proyecto.
- `.idea/`: Configuraciones específicas de CLion.
- `cmake-build-debug/`: Carpeta de salida para los archivos compilados.

## Instalación y Ejecución

### Clonar el Repositorio

```bash
git clone https://github.com/NacsonLefiguala/Redes.git
cd Redes

Compilar el Proyecto

Asegúrate de tener CMake y MinGW configurados en tu sistema.

bash

mkdir build
cd build
cmake ..
make

Ejecución del Servidor

Para ejecutar el servidor, usa el siguiente comando desde la carpeta build:

bash

./Server.exe <puerto>

Por ejemplo:

bash

./Server.exe 8080

Ejecución del Cliente

Para ejecutar el cliente, usa el siguiente comando desde la carpeta build:

bash

./Client.exe <dirección IP> <puerto>

Por ejemplo:

bash

./Client.exe 127.0.0.1 8080

Uso

    El servidor se ejecutará esperando la conexión de un cliente.
    Una vez que el cliente se conecta, el juego puede comenzar.
    Los jugadores ingresarán el número de columna donde desean colocar su ficha.
    El servidor y el cliente se turnarán para jugar hasta que uno de los jugadores gane o el tablero esté lleno (empate).
