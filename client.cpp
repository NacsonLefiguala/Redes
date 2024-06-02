#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <direccion IP> <puerto>" << endl;
        return 1;
    }

    const char* serverAddress = argv[1];
    int serverPort = atoi(argv[2]);

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cerr << "WSAStartup falló: " << iResult << endl;
        return 1;
    }

    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == INVALID_SOCKET) {
        cerr << "Error al crear el socket del cliente." << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort);
    serv_addr.sin_addr.s_addr = inet_addr(serverAddress);

    if (connect(client, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error al conectar con el servidor." << endl;
        closesocket(client);
        WSACleanup();
        return 1;
    }

    char buffer[1024];

    while (true) {
        int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidos > 0) {
            buffer[bytesRecibidos] = '\0';
            cout << buffer << endl;

            if (strcmp(buffer, "¡Gano Cliente!") == 0 || strcmp(buffer, "Empate") == 0 || strcmp(buffer, "¡Gano Servidor!") == 0) {
                break;
            } else if (strcmp(buffer, "Ingrese columna: ") == 0) {
                cout << "Ingrese columna: ";
                cin.getline(buffer, sizeof(buffer));
                send(client, buffer, strlen(buffer), 0);
            }
        } else {
            cerr << "Error al recibir datos del servidor." << endl;
            break;
        }
    }

    closesocket(client);
    WSACleanup();
    return 0;
}
