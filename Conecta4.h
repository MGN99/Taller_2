#include <iostream>
#include <vector>
#include <chrono>


using namespace std;

class Conecta4 {
public:
    Conecta4() : tablero(ROWS, vector<char>(COLS, ' ')) {}

    void jugar() {
        char jugadorUsuario = 'X';
        char jugadorIA = 'O';
        bool juegoTerminado = false;
        int dificultad;

        cout << "Seleccione la dificultad (1: Facil, 2: Intermedia, 3: Dificil): ";
        cin >> dificultad;

        auto start_time = std::chrono::high_resolution_clock::now();

        while (!juegoTerminado) {
            imprimirTablero();

            // Turno del jugador humano
            int columnaUsuario;
            int filaUsuario = jugarTurnoUsuario(jugadorUsuario, columnaUsuario);
            if (hayGanador(jugadorUsuario, filaUsuario, columnaUsuario - 1)) {
                imprimirTablero();
                cout << "¡Jugador " << jugadorUsuario << " ha ganado!" << endl;
                juegoTerminado = true;
                break;
            } else if (tableroLleno()) {
                imprimirTablero();
                cout << "¡Empate! El tablero esta lleno." << endl;
                juegoTerminado = true;
                break;
            }

            // Turno de la IA
            int columnaIA;
            int filaIA = jugarTurnoIA(jugadorIA, columnaIA);
            if (hayGanador(jugadorIA, filaIA, columnaIA - 1)) {
                imprimirTablero();
                cout << "¡La IA ha ganado!" << endl;
                juegoTerminado = true;
            } else if (tableroLleno()) {
                imprimirTablero();
                cout << "¡Empate! El tablero esta lleno." << endl;
                juegoTerminado = true;
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;
    }

private:
    const static int ROWS = 6;
    const static int COLS = 7;

    vector<vector<char>> tablero;

    void imprimirTablero() const {
        for (int i = 0; i < ROWS; ++i) {
            cout << "|";
            for (int j = 0; j < COLS; ++j) {
                cout << tablero[i][j] << "|";
            }
            cout << endl;
        }
        cout << " 1 2 3 4 5 6 7" << endl;
    }

    bool columnaValida(int columna) const {
        return columna >= 1 && columna <= COLS && tablero[0][columna - 1] == ' ';
    }

    int obtenerFilaVacia(int columna) const {
        for (int i = ROWS - 1; i >= 0; --i) {
            if (tablero[i][columna - 1] == ' ') {
                return i;
            }
        }
        return -1;
    }

    int evaluarTablero(char jugador, char oponente) const {
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

    bool tableroLleno() const {
        for (int j = 0; j < COLS; ++j) {
            if (tablero[0][j] == ' ') {
                return false;  // Todavía hay al menos una columna vacía
            }
        }
        return true;  // Todas las columnas están llenas
    }

    int minimax(vector<vector<char>> &tablero, int profundidad, bool esMaximizador, char jugador, char oponente)  {
        if (profundidad == 0 || tableroLleno()) {
            return evaluarTablero(jugador, oponente);
        }

        if (esMaximizador) {
            int mejorValor = INT_MIN;
            for (int columna = 1; columna <= COLS; ++columna) {
                if (columnaValida(columna)) {
                    int fila = obtenerFilaVacia(columna);
                    tablero[fila][columna - 1] = jugador;
                    mejorValor = max(mejorValor, minimax(tablero, profundidad - 1, !esMaximizador, jugador, oponente));
                    tablero[fila][columna - 1] = ' '; // Deshacer la jugada
                }
            }
            return mejorValor;
        } else {
            int mejorValor = INT_MAX;
            for (int columna = 1; columna <= COLS; ++columna) {
                if (columnaValida(columna)) {
                    int fila = obtenerFilaVacia(columna);
                    tablero[fila][columna - 1] = oponente;
                    mejorValor = min(mejorValor, minimax(tablero, profundidad - 1, !esMaximizador, jugador, oponente));
                    tablero[fila][columna - 1] = ' '; // Deshacer la jugada
                }
            }
            return mejorValor;
        }
    }

    int tomarDecisionIA(char jugador, char oponente, int dificultad)  {
        int mejorValor = INT_MIN;
        int mejorColumna = -1;
        int profundidad;

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
            if (columnaValida(columna)) {
                int fila = obtenerFilaVacia(columna);
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

    bool hayGanador(char jugador, int fila, int columna)  {
        int contador = 0;

        // Verificar en la fila
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

    int jugarTurnoUsuario(char jugador, int &columnaUsuario)  {
        int columna;
        do {
            cout << "Jugador " << jugador << ", elige una columna (1-7): ";
            cin >> columna;
        } while (!columnaValida(columna));

        columnaUsuario = columna;  // Actualiza la variable de columnaUsuario

        int fila = obtenerFilaVacia(columna);
        tablero[fila][columna - 1] = jugador;

        return fila;
    }


    int jugarTurnoIA(char jugador, int &columnaIA)  {
        int columna;
        do {
            columna = rand() % COLS + 1;  // Selecciona una columna al azar
        } while (!columnaValida(columna));

        columnaIA = columna;  // Actualiza la variable de columnaIA

        int fila = obtenerFilaVacia(columna);
        tablero[fila][columna - 1] = jugador;

        return fila;
    }
};