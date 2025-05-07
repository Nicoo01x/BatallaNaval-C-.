#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cctype> // Para funciones como toupper y tolower
#include <stdexcept> // Para manejar excepciones como invalid_argument
#include <string> 
#include <set> // Para almacenar ataques realizados por la computadora

using namespace std;

// Constantes para el tamaño del tablero y los símbolos utilizados
const int TAMANO_TABLERO = 8; // Dimensiones del tablero (8x8)
const char CELDA_VACIA = '~'; // Representa una celda vacía
const char CELDA_BARCO = 'S'; // Representa una celda con un barco
const char CELDA_IMPACTO = 'X'; // Representa una celda impactada
const char CELDA_FALLO = 'O'; // Representa un ataque fallido

// Estructura para definir un barco con su tamaño y nombre
struct Barco {
    int tamano; // Tamaño del barco (número de celdas que ocupa)
    string nombre; // Nombre del barco
};

// Lista de barcos disponibles en el juego
vector<Barco> barcos = {
    {4, "Porta Aviones"},
    {3, "Submarino"},
    {3, "Crucero"},
    {2, "Lancha"},
    {1, "Kayak"}
};

// Alias para representar el tablero como una matriz bidimensional de caracteres
typedef vector<vector<char>> Tablero;

// Función para limpiar la pantalla usando secuencia ANSI
void limpiarPantalla() {
    cout << "\033[2J\033[H"; // Limpia la pantalla y mueve el cursor al inicio
}

// Prototipos de funciones
void inicializarTablero(Tablero& tablero); // Inicializa el tablero con celdas vacías
void mostrarTablero(const Tablero& tablero, bool ocultarBarcos); // Muestra el tablero en la consola
bool puedeColocarBarco(const Tablero& tablero, int r, int c, int tamano, bool horizontal); // Verifica si un barco puede colocarse
void colocarBarco(Tablero& tablero, int r, int c, int tamano, bool horizontal); // Coloca un barco en el tablero
void colocarBarcosJugador(Tablero& tablero); // Permite al jugador colocar sus barcos
bool colocarBarcoAleatorio(Tablero& tablero, int tamano); // Coloca un barco aleatoriamente
void colocarBarcosComputadora(Tablero& tablero); // Coloca los barcos de la computadora
bool todosBarcosHundidos(const Tablero& tablero); // Verifica si todos los barcos han sido hundidos
string turnoJugador(Tablero& tableroComputadora, Tablero& vistaComputadora); // Turno del jugador para atacar
string turnoComputadora(Tablero& tableroJugador, set<pair<int, int>>& ataquesRealizados); // Turno de la computadora para atacar

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Inicializa la semilla para números aleatorios

    // Tableros para el jugador y la computadora
    Tablero tableroJugador, tableroComputadora, vistaComputadora;
    inicializarTablero(tableroJugador); // Inicializa el tablero del jugador
    inicializarTablero(tableroComputadora); // Inicializa el tablero de la computadora
    inicializarTablero(vistaComputadora); // Inicializa la vista del jugador sobre el tablero de la computadora

    // Colocar barcos del jugador y de la computadora
    colocarBarcosJugador(tableroJugador);
    colocarBarcosComputadora(tableroComputadora);

    // Variables para almacenar ataques y mensajes
    set<pair<int, int>> ataquesComputadora; // Almacena las coordenadas de los ataques realizados por la computadora
    string mensajeAtaqueComputadora;  // Mensaje del último ataque de la computadora
    string mensajeTurnoJugador;       // Mensaje del último ataque del jugador

    cout << "\n¡Comienza el juego de Batalla Naval!\n";

    // Bucle principal del juego
    while (true) {
        limpiarPantalla(); // Limpia la pantalla antes de mostrar los tableros

        // Mostrar el tablero del jugador
        cout << "Tu Tablero:" << endl;
        mostrarTablero(tableroJugador, false); // Muestra todos los barcos y estados

        // Mostrar el tablero de la computadora (sin revelar barcos)
        cout << "\nTablero del Oponente (lo que tu ves):" << endl;
        mostrarTablero(vistaComputadora, true); // Oculta los barcos de la computadora

        // Mostrar mensajes de los últimos ataques
        if (!mensajeTurnoJugador.empty()) {
            cout << "\n(Jugador): " << mensajeTurnoJugador << endl;
        }
        if (!mensajeAtaqueComputadora.empty()) {
            cout << "\n(Computadora): " << mensajeAtaqueComputadora << endl;
        }
        cout << endl;

        // Turno del jugador
        mensajeTurnoJugador = turnoJugador(tableroComputadora, vistaComputadora);
        if (todosBarcosHundidos(tableroComputadora)) { // Verifica si el jugador ganó
            limpiarPantalla();
            cout << "Tu Tablero Final:" << endl;
            mostrarTablero(tableroJugador, false);
            cout << "\nTablero Final del Oponente:" << endl;
            mostrarTablero(tableroComputadora, false);
            cout << "\n¡Felicidades! Hundiste todos los barcos del oponente. ¡Ganaste!" << endl;
            break;
        }

        limpiarPantalla();

        // Turno de la computadora
        cout << "Turno de la computadora..." << endl;
        mensajeAtaqueComputadora = turnoComputadora(tableroJugador, ataquesComputadora); 
        if (todosBarcosHundidos(tableroJugador)) { // Verifica si la computadora ganó
            limpiarPantalla();
            cout << "Tablero Final:" << endl;
            mostrarTablero(tableroJugador, false);
            cout << "\nTablero Final del Oponente:" << endl;
            mostrarTablero(tableroComputadora, false);
            cout << "\n¡Todos tus barcos han sido hundidos. Perdiste!" << endl;
            break;
        }
    }

    return 0;
}

// Inicializa el tablero con celdas vacías
void inicializarTablero(Tablero& tablero) {
    tablero.resize(TAMANO_TABLERO, vector<char>(TAMANO_TABLERO, CELDA_VACIA));
}

// Muestra el tablero en la consola
void mostrarTablero(const Tablero& tablero, bool ocultarBarcos) {
    cout << "  ";
    for (int c = 0; c < TAMANO_TABLERO; ++c)
        cout << " " << c + 1; // Imprime los números de las columnas
    cout << endl;

    for (int r = 0; r < TAMANO_TABLERO; ++r) {
        cout << char('A' + r) << " "; // Imprime las letras de las filas
        for (int c = 0; c < TAMANO_TABLERO; ++c) {
            char celda = tablero[r][c];
            if (ocultarBarcos && celda == CELDA_BARCO)
                cout << " " << CELDA_VACIA; // Oculta los barcos
            else
                cout << " " << celda; // Muestra el contenido de la celda
        }
        cout << endl;
    }
}

// Verifica si un barco puede colocarse en una posición específica
bool puedeColocarBarco(const Tablero& tablero, int r, int c, int tamano, bool horizontal) {
    if (horizontal) {
        if (c + tamano > TAMANO_TABLERO) return false; // Verifica si el barco cabe horizontalmente
        for (int i = 0; i < tamano; ++i) {
            if (tablero[r][c + i] != CELDA_VACIA) return false; // Verifica que las celdas estén vacías
        }
    }
    else {
        if (r + tamano > TAMANO_TABLERO) return false; // Verifica si el barco cabe verticalmente
        for (int i = 0; i < tamano; ++i) {
            if (tablero[r + i][c] != CELDA_VACIA) return false; // Verifica que las celdas estén vacías
        }
    }
    return true;
}

// Coloca un barco en el tablero
void colocarBarco(Tablero& tablero, int r, int c, int tamano, bool horizontal) {
    if (horizontal) {
		for (int i = 0; i < tamano; ++i) { // R es para las filas y C es para las columnas
            tablero[r][c + i] = CELDA_BARCO; // Marca las celdas ocupadas por el barco
        }
    }
    else {
        for (int i = 0; i < tamano; ++i) {
            tablero[r + i][c] = CELDA_BARCO; // Marca las celdas ocupadas por el barco
        }
    }
}

// Turno del jugador para atacar
string turnoJugador(Tablero& tableroComputadora, Tablero& vistaComputadora) { // Turno del jugador para atacar y la linea dice que los que cambie en TableroComputadora se reflejan en vistaComputadora
    while (true) {
        cout << "Ingresa el objetivo a atacar (ej. B3): ";
        string pos;
        cin >> pos;
        // pos se utiliza para almacenar la posición ingresada por el jugador

		if (pos.length() < 2 || !isalpha(pos[0]) || !isdigit(pos[1])) { // Verifica si la entrada es válida
            cout << "Entrada invalida. Intenta de nuevo." << endl;
            continue;
        }

		int fila = toupper(pos[0]) - 'A'; // toupper se usa para convertir la letra a mayúscula
        int columna;
        try {
            columna = stoi(pos.substr(1)) - 1;
        }
        catch (invalid_argument&) {
            cout << "Entrada invalida. Intenta de nuevo." << endl;
            continue;
        }

        if (fila < 0 || fila >= TAMANO_TABLERO || columna < 0 || columna >= TAMANO_TABLERO) {
            cout << "Coordenadas fuera de rango. Intenta de nuevo." << endl;
            continue;
        }

        if (vistaComputadora[fila][columna] == CELDA_IMPACTO || vistaComputadora[fila][columna] == CELDA_FALLO) {
            cout << "Ya atacaste esta posicion. Elige otra." << endl; 
            continue;
        }

        string mensaje = "Procesando ataque en ";
		mensaje += char('A' + fila); // += se usa para sumar una valor a la variable y luego guardarlo
        mensaje += to_string(columna + 1);
        mensaje += "... ";

        if (tableroComputadora[fila][columna] == CELDA_BARCO) {
            tableroComputadora[fila][columna] = CELDA_IMPACTO;
            vistaComputadora[fila][columna] = CELDA_IMPACTO;
            mensaje += "Barco impactado!";
        }
        else {
            vistaComputadora[fila][columna] = CELDA_FALLO;
            mensaje += "Agua.";
        }

        return mensaje;
    }
}

// Turno de la computadora para atacar
string turnoComputadora(Tablero& tableroJugador, set<pair<int, int >>& ataquesRealizados) {
    while (true) {
        int fila = rand() % TAMANO_TABLERO;
        int columna = rand() % TAMANO_TABLERO;

        if (ataquesRealizados.count({ fila, columna }) > 0) continue;

        ataquesRealizados.insert({ fila, columna });

        string mensaje = "La computadora te ataco en ";
        mensaje += char('A' + fila);
        mensaje += to_string(columna + 1);
        mensaje += ". ";

        if (tableroJugador[fila][columna] == CELDA_BARCO) {
            mensaje += "¡La computadora impacto tu barco!";
            tableroJugador[fila][columna] = CELDA_IMPACTO;
        }
        else if (tableroJugador[fila][columna] == CELDA_VACIA) {
            mensaje += "La computadora fallo.";
            tableroJugador[fila][columna] = CELDA_FALLO;
        }
        else {
            continue;
        }

		return mensaje; // Vuelve a el mensaje del ataque de la computadora
    }
}
