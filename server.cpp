#include <iostream>
#include <winsock2.h>
#include <ctime>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class Server {
private:
    static constexpr char Disponible = '_';
    static int fila;
    static int columna;

    static int calcularFila(char m[6][7], int columna) {
        for (int i = 5; i >= 0; i--) {
            if (m[i][columna] == Disponible) {
                return i;
            }
        }
        return -1; // Columna llena
    }

    static void inicializarTablero(char m[6][7]) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                m[i][j] = '_';
            }
        }
    }

    static void mostrarTablero(char m[6][7]) {
        cout << endl;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                cout << "[" << m[i][j] << "]";
            }
            cout << endl;
        }
    }

    static bool verificarGanador(char m[6][7], char signo) {
        int i, j, cont;

        // Horizontales
        for (i = 0; i < 6; i++) {
            cont = 1;
            for (j = 0; j < 5; j++) {
                if (m[i][j] == signo && m[i][j + 1] == signo) {
                    cont++;
                } else {
                    cont = 1;
                }
                if (cont == 4) return true;
            }
        }

        // Verticales
        for (j = 0; j < 7; j++) {
            cont = 1;
            for (i = 0; i < 5; i++) {
                if (m[i][j] == signo && m[i + 1][j] == signo) {
                    cont++;
                } else {
                    cont = 1;
                }
                if (cont == 4) return true;
            }
        }

        // Diagonales
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                if (m[i][j] == signo && m[i + 1][j + 1] == signo && m[i + 2][j + 2] == signo && m[i + 3][j + 3] == signo) {
                    return true;
                }
            }
        }

        // Diagonales /
        for (i = 0; i < 3; i++) {
            for (j = 3; j < 7; j++) {
                if (m[i][j] == signo && m[i + 1][j - 1] == signo && m[i + 2][j - 2] == signo && m[i + 3][j - 3] == signo) {
                    return true;
                }
            }
        }

        return false;
    }

    static bool tableroCompleto(char m[6][7]) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                if (m[i][j] == Disponible) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    static void main(int argc, char* argv[]) {
        if (argc != 2) {
            cerr << "Uso: " << argv[0] << " <puerto>" << endl;
            return;
        }

        int puerto = atoi(argv[1]);

        WSADATA WSAData;
        SOCKET client;
        SOCKADDR_IN serverAddr, clientAddr;
        char buffer[1024];

        // Inicializar Winsock
        if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0) {
            cerr << "Error al inicializar Winsock." << endl;
            return;
        }

        // Crear el socket del servidor
        SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == INVALID_SOCKET) {
            cerr << "Error al crear el socket del servidor." << endl;
            WSACleanup();
            return;
        }

        // Configurar la dirección del servidor
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(puerto);

        // Vincular el socket a la dirección y el puerto
        if (bind(server, reinterpret_cast<SOCKADDR *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "Error al vincular el socket al puerto." << endl;
            closesocket(server);
            WSACleanup();
            return;
        }

        // Obtener la IP del servidor
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
            cerr << "Error al obtener el nombre del host." << endl;
            closesocket(server);
            WSACleanup();
            return;
        }

        struct hostent* host = gethostbyname(hostname);
        char* ip = inet_ntoa(*reinterpret_cast<struct in_addr*>(host->h_addr_list[0]));

        cout << "Servidor escuchando en IP: " << ip << " puerto: " << puerto << endl;

        // Escuchar nuevas conexiones
        listen(server, 1);

        cout << "Esperando conexion con el cliente..." << endl;
        int clientAddrSize = sizeof(clientAddr);
        if ((client = accept(server, reinterpret_cast<SOCKADDR *>(&clientAddr), &clientAddrSize)) != INVALID_SOCKET) {
            cout << "Cliente conectado" << endl;
        }

        char tablero[6][7];
        inicializarTablero(tablero);

        // Elegir aleatoriamente quién comienza
        srand(time(0));  // Inicializar la semilla de números aleatorios
        bool turnoJugador = rand() % 2 == 0;

        while (true) {
            if (turnoJugador) {
                cout << "--- TURNO JUGADOR ---" << endl;
                mostrarTablero(tablero);
                do {
                    send(client, "Ingrese columna: ", sizeof("Ingrese columna: "), 0);
                    int bytesRecibidos = recv(client, buffer, sizeof(buffer) - 1, 0);
                    buffer[bytesRecibidos] = '\0';
                    sscanf_s(buffer, "%d", &columna);
                    fila = calcularFila(tablero, columna);
                    if (columna < 0 || columna > 6) {
                        send(client, "Columna invalida. Ingrese una columna entre 0 y 6.", sizeof("Columna invalida. Ingrese una columna entre 0 y 6."), 0);
                        continue;
                    }
                    if (fila == -1) {
                        send(client, "Columna llena. Ingrese otra columna.", sizeof("Columna llena. Ingrese otra columna."), 0);
                        continue;
                    }
                    if (Disponible == tablero[fila][columna]) {
                        tablero[fila][columna] = 'C';
                    } else {
                        send(client, "Casilla Ocupada", sizeof("Casilla Ocupada"), 0);
                        continue;
                    }
                } while (Disponible == tablero[fila][columna]);

                if (verificarGanador(tablero, 'C')) {
                    cout << "¡Gano Cliente!" << endl;
                    mostrarTablero(tablero);
                    send(client, "¡Gano Cliente!", sizeof("¡Gano Cliente!"), 0);
                    break;
                }

                if (tableroCompleto(tablero)) {
                    mostrarTablero(tablero);
                    send(client, "Empate", sizeof("Empate"), 0);
                    cout << "Empate" << endl;
                    break;
                }

                turnoJugador = false;
            } else {
                cout << "--- TURNO SERVIDOR ---" << endl;
                mostrarTablero(tablero);
                do {
                    cout << "Ingrese columna: ";
                    cin >> columna;
                    fila = calcularFila(tablero, columna);
                    if (columna < 0 || columna > 6) {
                        cout << "Columna inválida. Ingrese una columna entre 0 y 6." << endl;
                        continue;
                    }
                    if (fila == -1) {
                        cout << "Columna llena. Ingrese otra columna." << endl;
                        continue;
                    }
                    if (Disponible == tablero[fila][columna]) {
                        tablero[fila][columna] = 'S';
                    } else {
                        cout << "Casilla Ocupada" << endl;
                        continue;
                    }
                } while (Disponible == tablero[fila][columna]);

                if (verificarGanador(tablero, 'S')) {
                    cout << "¡Ganaste!" << endl;
                    mostrarTablero(tablero);
                    send(client, "¡Gano Servidor!", sizeof("¡Gano Servidor!"), 0);
                    break;
                }

                if (tableroCompleto(tablero)) {
                    mostrarTablero(tablero);
                    send(client, "Empate", sizeof("Empate"), 0);
                    cout << "Empate" << endl;
                    break;
                }

                turnoJugador = true;
            }
        }

        closesocket(client);
        closesocket(server);
        WSACleanup();
    }
};

int Server::fila = 0;
int Server::columna = 0;

int main(int argc, char* argv[]) {
    Server::main(argc, argv);
    return 0;
}
