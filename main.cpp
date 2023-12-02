#include <iostream>
#include <vector>


const int ROWS = 6;
const int COLS = 7;

void imprimirTablero(const std::vector<std::vector<char>> &tablero) {
    for (int i = 0; i < ROWS; ++i) {
        std::cout << "|";
        for (int j = 0; j < COLS; ++j) {
            std::cout << tablero[i][j] << "|";
        }
        std::cout << std::endl;
    }
    std::cout << " 1 2 3 4 5 6 7" << std::endl;
}

bool columnaValida(int columna, const std::vector<std::vector<char>> &tablero) {
    return columna >= 1 && columna <= COLS && tablero[0][columna - 1] == ' ';
}

int obtenerFilaVacia(int columna, const std::vector<std::vector<char>> &tablero) {
    for (int i = ROWS - 1; i >= 0; --i) {
        if (tablero[i][columna - 1] == ' ') {
            return i;
        }
    }
    return -1; 
}

int evaluarTablero(const std::vector<std::vector<char>> &tablero, char jugador, char oponente) {
    
    int valorJugador = 0;
    int valorOponente = 0;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (tablero[i][j] == jugador) {
                valorJugador++;
            } else if (tablero[i][j] == oponente) {
                valorOponente++;
            }
        }
    }

    return valorJugador - valorOponente;
}

bool tableroLleno(const std::vector<std::vector<char>> &tablero) {
    for (int j = 0; j < COLS; ++j) {
        if (tablero[0][j] == ' ') {
            return false;  // Todavía hay al menos una columna vacía
        }
    }
    return true;  // Todas las columnas están llenas
}

int minimax(std::vector<std::vector<char>> &tablero, int profundidad, bool esMaximizador, char jugador, char oponente) {
    if (profundidad == 0 || tableroLleno(tablero)) {
        return evaluarTablero(tablero, jugador, oponente);
    }

    if (esMaximizador) {
        int mejorValor = INT_MIN;
        for (int columna = 1; columna <= COLS; ++columna) {
            if (columnaValida(columna, tablero)) {
                int fila = obtenerFilaVacia(columna, tablero);
                tablero[fila][columna - 1] = jugador;
                mejorValor = std::max(mejorValor, minimax(tablero, profundidad - 1, !esMaximizador, jugador, oponente));
                tablero[fila][columna - 1] = ' '; // Deshacer la jugada
            }
        }
        return mejorValor;
    } else {
        int mejorValor = INT_MAX;
        for (int columna = 1; columna <= COLS; ++columna) {
            if (columnaValida(columna, tablero)) {
                int fila = obtenerFilaVacia(columna, tablero);
                tablero[fila][columna - 1] = oponente;
                mejorValor = std::min(mejorValor, minimax(tablero, profundidad - 1, !esMaximizador, jugador, oponente));
                tablero[fila][columna - 1] = ' '; // Deshacer la jugada
            }
        }
        return mejorValor;
    }
}

int tomarDecisionIA(std::vector<std::vector<char>> &tablero, char jugador, char oponente, int dificultad) {
    int mejorValor = INT_MIN;
    int mejorColumna = -1;
    int profundidad;

    // Ajusta la profundidad según la dificultad
    switch (dificultad) {
        case 1:
            profundidad = 2;  // Dificultad fácil
            break;
        case 2:
            profundidad = 4;  // Dificultad intermedia
            break;
        case 3:
            profundidad = 6;  // Dificultad difícil
            break;
        default:
            profundidad = 3;  // dificultad si se ingresa valor no valido
            break;
    }

    for (int columna = 1; columna <= COLS; ++columna) {
        if (columnaValida(columna, tablero)) {
            int fila = obtenerFilaVacia(columna, tablero);
            tablero[fila][columna - 1] = jugador;
            int valorMinimax = minimax(tablero, profundidad, false, jugador, oponente);
            tablero[fila][columna - 1] = ' '; // Deshacer la jugada

            if (valorMinimax > mejorValor) {
                mejorValor = valorMinimax;
                mejorColumna = columna;
            }
        }
    }

    return mejorColumna;
}


bool hayGanador(const std::vector<std::vector<char>> &tablero, char jugador, int fila, int columna) {
    // Verificar en la fila
    int contador = 0;
    for (int j = 0; j < COLS; ++j) {
        if (tablero[fila][j] == jugador) {
            ++contador;
            if (contador == 4) return true;
        } else {
            contador = 0;
        }
    }

    // Verificar en la columna
    contador = 0;
    for (int i = 0; i < ROWS; ++i) {
        if (tablero[i][columna] == jugador) {
            ++contador;
            if (contador == 4) return true;
        } else {
            contador = 0;
        }
    }

    // Verificar en la diagonal principal
    contador = 0;
    int i = fila, j = columna;
    while (i > 0 && j > 0) {
        --i;
        --j;
    }
    while (i < ROWS && j < COLS) {
        if (tablero[i][j] == jugador) {
            ++contador;
            if (contador == 4) return true;
        } else {
            contador = 0;
        }
        ++i;
        ++j;
    }

    // Verificar en la diagonal secundaria
    contador = 0;
    i = fila;
    j = columna;
    while (i > 0 && j < COLS - 1) {
        --i;
        ++j;
    }
    while (i < ROWS && j >= 0) {
        if (tablero[i][j] == jugador) {
            ++contador;
            if (contador == 4) return true;
        } else {
            contador = 0;
        }
        ++i;
        --j;
    }

    return false;
}



int jugarTurnoUsuario(std::vector<std::vector<char>> &tablero, char jugador, int &columnaUsuario) {
    int columna;
    do {
        std::cout << "Jugador " << jugador << ", elige una columna (1-7): ";
        std::cin >> columna;
    } while (!columnaValida(columna, tablero));

    columnaUsuario = columna;  // Actualiza la variable de columnaUsuario

    int fila = obtenerFilaVacia(columna, tablero);
    tablero[fila][columna - 1] = jugador;

    return fila;
}

int jugarTurnoIA(std::vector<std::vector<char>> &tablero, char jugador, int &columnaIA) {
    int columna;
    do {
        columna = rand() % COLS + 1;  // Selecciona una columna al azar
    } while (!columnaValida(columna, tablero));

    columnaIA = columna;  // Actualiza la variable de columnaIA

    int fila = obtenerFilaVacia(columna, tablero);
    tablero[fila][columna - 1] = jugador;

    return fila;
}


int main() {
    std::vector<std::vector<char>> tablero(ROWS, std::vector<char>(COLS, ' '));

    char jugadorUsuario = 'X';
    char jugadorIA = 'O';
    bool juegoTerminado = false;
    int dificultad;

    std::cout << "Seleccione la dificultad (1: Facil, 2: Intermedia, 3: Dificil): ";
    std::cin >> dificultad;

    

    while (!juegoTerminado) {
        imprimirTablero(tablero);

        // Turno del jugador humano
        int columnaUsuario;
        int filaUsuario = jugarTurnoUsuario(tablero, jugadorUsuario, columnaUsuario);
        if (hayGanador(tablero, jugadorUsuario, filaUsuario, columnaUsuario - 1)) {
            imprimirTablero(tablero);
            std::cout << "¡Jugador " << jugadorUsuario << " ha ganado!" << std::endl;
            juegoTerminado = true;
            break;
        } else if (tableroLleno(tablero)) {
            imprimirTablero(tablero);
            std::cout << "¡Empate! El tablero está lleno." << std::endl;
            juegoTerminado = true;
            break;
        }

        // Turno de la IA
        int columnaIA;
        int filaIA = jugarTurnoIA(tablero, jugadorIA, columnaIA);
        if (hayGanador(tablero, jugadorIA, filaIA, columnaIA - 1)) {
            imprimirTablero(tablero);
            std::cout << "¡La IA ha ganado!" << std::endl;
            juegoTerminado = true;
        } else if (tableroLleno(tablero)) {
            imprimirTablero(tablero);
            std::cout << "¡Empate! El tablero está lleno." << std::endl;
            juegoTerminado = true;
        }
    }

    return 0;
}