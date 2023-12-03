# Taller_2

Ruta de compilacion:"g++ -o main main.cpp"
para ejecutar:"./main"

El programa lo dividi en dos clases.

Clase Main: se encarga solo de llamar a un objeto de tipo Conecta4

Clase Conecta4: Se encarga de crear la matriz que simula el juego Conecta4 e implementa toda la logica del juego como por ejemplo:

-Revisar si la columna esta llena en donde se quiere poner una ficha.
-Evaluar el tablero en cada jugada y hacer comprobaciones en direccion lateral y diagonal
-Verificar ganador IA o humano
-verificar empate
-Implementar minimax Poda Alfa beta
-logica de la jugada del humano
-logica de la jugada de la IA.
-logica de guardado de puntajes de Humano e IA en archivo csv

Conecta4 con minimax Poda Alfa-Beta
Tiempo de ejecucion total del programa en modo Fácil: Tiempo de ejecución: 3830 ms (4 movimientos)
Tiempo de ejecucion total del programa en modo Intermedio: Tiempo de ejecución: 8723 ms (5 movimientos)
Tiempo de ejecucion total del programa en modo Difícil: Tiempo de ejecución: 33632 ms (12 movimientos)

Conecta4 sin minimax Poda Alfa-Beta
Tiempo de ejecución total del programa en modo Fácil: Tiempo de ejecución: 11320 ms (6 movimientos)
Tiempo de ejecución total del programa en modo Intermedio: Tiempo de ejecución: 15732 ms (7 movimientos)
Tiempo de ejecución total del programa en modo Difícil: Tiempo de ejecución: 120000 ms aprox (14 movimientos)

para efectos de evaluación dejé el codigo con el método implementado.

La implementación de la poda alfa-beta en el código mejora significativamente la eficiencia del algoritmo Minimax al reducir la cantidad de nodos evaluados durante la búsqueda para evitar explorar ramas que no afectarán el resultado final, permitiendo una búsqueda más rápida y eficiente.

El archivo CSV registra la cantidad de partidas totales jugadas/ cantidad de partidas ganadas por humano/ cantidad de partidas ganadas por IA
El archivo CSV llamado movimientos registra los movimientos realizados en una partida

