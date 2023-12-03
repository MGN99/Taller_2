#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>  
#include <iomanip>


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
                registrarResultado(jugadorUsuario);  // Registra el resultado
                break;
            } else if (tableroLleno()) {
                imprimirTablero();
                cout << "¡Empate! El tablero está lleno." << endl;
                juegoTerminado = true;
                registrarResultado(' ');  // Registra el empate
                break;
            }

            // Turno de la IA
            int columnaIA;
            int filaIA = jugarTurnoIA(jugadorIA, columnaIA);
            if (hayGanador(jugadorIA, filaIA, columnaIA - 1)) {
                imprimirTablero();
                cout << "¡La IA ha ganado!" << endl;
                juegoTerminado = true;
                registrarResultado(jugadorIA);  // Registra el resultado
            } else if (tableroLleno()) {
                imprimirTablero();
                cout << "¡Empate! El tablero está lleno." << endl;
                juegoTerminado = true;
                registrarResultado(' ');  // Registra el empate
            }
        }
        partidasJugadas++;
        
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;
    }

    void registrarResultado(char ganador) {
        if (ganador == jugadorUsuario) {
            puntuacionJugador++;
        } else if (ganador == jugadorIA) {
            puntuacionIA++;
        }
    }

    void guardarResultadosCSV() {
        ofstream archivo("resultados.csv", ios::out | ios::app);  // Abre el archivo en modo de escritura y apendizaje

        // Verifica si el archivo se abrió correctamente
        if (archivo.is_open()) {
            // Si es la primera vez que se guarda, escribe la cabecera
            if (partidasJugadas == 1) {
                archivo << "Partidas,Jugador,Máquina" << endl;
            }

            // Escribe los resultados acumulativos
            archivo << partidasJugadas << "," << puntuacionJugador << "," << puntuacionIA << endl;

            archivo.close();  // Cierra el archivo
            cout << "Resultados guardados en 'resultados.csv'" << endl;
        } else {
            cout << "Error al abrir el archivo 'resultados.csv'" << endl;
        }
    }
    void reiniciar() {
        tablero = vector<vector<char>>(ROWS, vector<char>(COLS, ' '));
    }

    void menu() {
    int opcion;
    do {
        Conecta4 juego;  // Reinicia el juego antes de cada partida
        cout << "Menú:" << endl;
        cout << "1. Jugar" << endl;
        cout << "2. Cargar Partida" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                juego.jugar();
                juego.guardarResultadosCSV();
                break;
            case 2:
                // Agrega aquí la lógica para cargar la partida
                break;
            case 3:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }

        if (opcion != 3) {
            int seguirJugando;
            cout << "¿Desea seguir jugando? (1: Si, 0: No): ";

            // Validación para asegurarse de que se ingrese un 0 o un 1
            while (true) {
                cin >> seguirJugando;

                if (cin.fail() || (seguirJugando != 0 && seguirJugando != 1)) {
                    cin.clear();  
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  
                    cout << "Por favor, ingrese 0 o 1: ";
                } else {
                    break;
                }
            }

            opcion = seguirJugando;
        }

    } while (opcion == 1);
}

    

private:
    const static int ROWS = 6;
    const static int COLS = 7;
    char jugadorUsuario = 'X';
    char jugadorIA = 'O';
    int puntuacionJugador = 0;
    int puntuacionIA = 0;
    int partidasJugadas = 0;

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
                tablero[fila][columna - 1] = ' '; 

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