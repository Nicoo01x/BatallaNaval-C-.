#include <iostream> // Biblioteca para entrada y salida estándar
#include <vector> // Biblioteca para usar vectores dinámicos
#include <ctime> // Biblioteca para trabajar con tiempo (semilla para aleatorios)
#include <cstdlib> // Biblioteca para funciones como rand()
#include <cctype> // Biblioteca para funciones como toupper() y tolower()
#include <stdexcept> // Biblioteca para manejar excepciones como invalid_argument
#include <string> // Biblioteca para trabajar con cadenas de texto
#include <set> // Biblioteca para usar conjuntos (set) y evitar duplicados

using namespace std; // Permite usar nombres estándar sin prefijo std::

const int TAMANO_TABLERO = 8; // Define el tamaño del tablero (8x8)
const char CELDA_VACIA = '~'; // Representa una celda vacía en el tablero
const char CELDA_BARCO = 'S'; // Representa una celda ocupada por un barco
const char CELDA_IMPACTO = 'X'; // Representa una celda impactada por un ataque
const char CELDA_FALLO = 'O'; // Representa un ataque fallido

// Estructura que define un barco con su tamaño y nombre
struct Barco {
    int tamano; // Tamaño del barco (número de celdas que ocupa)
    string nombre; // Nombre del barco (ej. "Submarino")
};

// Lista de barcos disponibles en el juego
vector<Barco> barcos = {
    {4, "Porta Aviones"}, // Barco de tamaño 4
    {3, "Submarino"}, // Barco de tamaño 3
    {3, "Crucero"}, // Barco de tamaño 3
    {2, "Lancha"}, // Barco de tamaño 2
    {1, "Kayak"} // Barco de tamaño 1
};

// Alias para representar un tablero como una matriz bidimensional de caracteres
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
    inicializarTablero(tableroJugador); // Inicializa el tablero del jugador con celdas vacías
    inicializarTablero(tableroComputadora); // Inicializa el tablero de la computadora con celdas vacías
    inicializarTablero(vistaComputadora); // Inicializa la vista del jugador sobre el tablero de la computadora

    // Colocar barcos del jugador y de la computadora
    colocarBarcosJugador(tableroJugador); // Permite al jugador colocar sus barcos
    colocarBarcosComputadora(tableroComputadora); // Coloca los barcos de la computadora aleatoriamente

    // Variables para almacenar ataques y mensajes
    set<pair<int, int>> ataquesComputadora; // Almacena las coordenadas de los ataques realizados por la computadora
    string mensajeAtaqueComputadora;  // Mensaje del último ataque de la computadora
    string mensajeTurnoJugador;       // Mensaje del último ataque del jugador

    cout << "\n¡Comienza el juego de Batalla Naval!\n"; // Mensaje inicial del juego

    // Bucle principal del juego
    while (true) {
        limpiarPantalla(); // Limpia la pantalla antes de mostrar los tableros

        // Mostrar el tablero del jugador
        cout << "Tu Tablero:" << endl;
        mostrarTablero(tableroJugador, false); // Muestra todos los barcos y estados del jugador

        // Mostrar el tablero de la computadora (sin revelar barcos)
        cout << "\nTablero del Oponente (lo que tu ves):" << endl;
        mostrarTablero(vistaComputadora, true); // Oculta los barcos de la computadora

        // Mostrar mensajes de los últimos ataques
        if (!mensajeTurnoJugador.empty()) {
            cout << "\n(Jugador): " << mensajeTurnoJugador << endl; // Muestra el mensaje del último ataque del jugador
        }
        if (!mensajeAtaqueComputadora.empty()) {
            cout << "\n(Computadora): " << mensajeAtaqueComputadora << endl; // Muestra el mensaje del último ataque de la computadora
        }
        cout << endl;

        // Turno del jugador
        mensajeTurnoJugador = turnoJugador(tableroComputadora, vistaComputadora); // Ejecuta el turno del jugador
        if (todosBarcosHundidos(tableroComputadora)) { // Verifica si el jugador ganó
            limpiarPantalla(); // Limpia la pantalla antes de mostrar los resultados finales
            cout << "Tu Tablero Final:" << endl;
            mostrarTablero(tableroJugador, false); // Muestra el tablero final del jugador
            cout << "\nTablero Final del Oponente:" << endl;
            mostrarTablero(tableroComputadora, false); // Muestra el tablero final de la computadora
            cout << "\n¡Felicidades! Hundiste todos los barcos del oponente. ¡Ganaste!" << endl;
            break; // Termina el juego
        }

        limpiarPantalla(); // Limpia la pantalla antes del turno de la computadora

        // Turno de la computadora
        cout << "Turno de la computadora..." << endl;
        mensajeAtaqueComputadora = turnoComputadora(tableroJugador, ataquesComputadora); // Ejecuta el turno de la computadora
        if (todosBarcosHundidos(tableroJugador)) { // Verifica si la computadora ganó
            limpiarPantalla(); // Limpia la pantalla antes de mostrar los resultados finales
            cout << "Tablero Final:" << endl;
            mostrarTablero(tableroJugador, false); // Muestra el tablero final del jugador
            cout << "\nTablero Final del Oponente:" << endl;
            mostrarTablero(tableroComputadora, false); // Muestra el tablero final de la computadora
            cout << "\n¡Todos tus barcos han sido hundidos. Perdiste!" << endl;
            break; // Termina el juego
        }
    }

    return 0; // Finaliza el programa
}

// Inicializa el tablero con celdas vacías
void inicializarTablero(Tablero& tablero) {
    tablero.resize(TAMANO_TABLERO, vector<char>(TAMANO_TABLERO, CELDA_VACIA)); // cambia el tablero y lo llena con celdas vacías
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
    return true; // Retorna true si el barco puede colocarse
}

// Coloca un barco en el tablero
void colocarBarco(Tablero& tablero, int r, int c, int tamano, bool horizontal) {
    if (horizontal) {
        for (int i = 0; i < tamano; ++i) {
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
string turnoJugador(Tablero& tableroComputadora, Tablero& vistaComputadora) {
    while (true) {
        cout << "Ingresa el objetivo a atacar (ej. B3): ";
        string pos;
        cin >> pos; // Lee la posición ingresada por el jugador

        if (pos.length() < 2 || !isalpha(pos[0]) || !isdigit(pos[1])) { // Verifica si la entrada es válida
            cout << "Entrada invalida. Intenta de nuevo." << endl;
            continue; // Pide al jugador que intente de nuevo
        }

        int fila = toupper(pos[0]) - 'A'; // Convierte la letra a mayúscula y la convierte en índice de fila
        int columna;
        try {
            columna = stoi(pos.substr(1)) - 1; // Convierte el número ingresado en índice de columna
        }
        catch (invalid_argument&) {
            cout << "Entrada invalida. Intenta de nuevo." << endl;
            continue; // Pide al jugador que intente de nuevo
        }

        if (fila < 0 || fila >= TAMANO_TABLERO || columna < 0 || columna >= TAMANO_TABLERO) { // Verifica si las coordenadas están dentro del rango
            cout << "Coordenadas fuera de rango. Intenta de nuevo." << endl;
            continue; // Pide al jugador que intente de nuevo
        }

        if (vistaComputadora[fila][columna] == CELDA_IMPACTO || vistaComputadora[fila][columna] == CELDA_FALLO) { // Verifica si la celda ya fue atacada
            cout << "Ya atacaste esta posicion. Elige otra." << endl;
            continue; // Pide al jugador que intente de nuevo
        }

        string mensaje = "Procesando ataque en ";
        mensaje += char('A' + fila); // Agrega la letra de la fila al mensaje
        mensaje += to_string(columna + 1); // Agrega el número de la columna al mensaje
        mensaje += "... ";

        if (tableroComputadora[fila][columna] == CELDA_BARCO) { // Verifica si el ataque impactó un barco
            tableroComputadora[fila][columna] = CELDA_IMPACTO; // Marca la celda como impactada
            vistaComputadora[fila][columna] = CELDA_IMPACTO; // Actualiza la vista del jugador
            mensaje += "Barco impactado!"; // Agrega el mensaje de impacto
        }
        else { // Si el ataque no impactó un barco
            vistaComputadora[fila][columna] = CELDA_FALLO; // Marca la celda como fallo
            mensaje += "Agua."; // Agrega el mensaje de fallo
        }

        return mensaje; // Retorna el mensaje del resultado del ataque
    }
}

// Turno de la computadora para atacar
string turnoComputadora(Tablero& tableroJugador, set<pair<int, int >>& ataquesRealizados) {
    while (true) {
        int fila = rand() % TAMANO_TABLERO; // Genera una fila aleatoria
        int columna = rand() % TAMANO_TABLERO; // Genera una columna aleatoria

        if (ataquesRealizados.count({ fila, columna }) > 0) continue; // Verifica si la celda ya fue atacada

        ataquesRealizados.insert({ fila, columna }); // Agrega la celda a la lista de ataques realizados

        string mensaje = "La computadora te ataco en ";
        mensaje += char('A' + fila); // Agrega la letra de la fila al mensaje
        mensaje += to_string(columna + 1); // Agrega el número de la columna al mensaje
        mensaje += ". ";

        if (tableroJugador[fila][columna] == CELDA_BARCO) { // Verifica si el ataque impactó un barco
            mensaje += "¡La computadora impacto tu barco!"; // Agrega el mensaje de impacto
            tableroJugador[fila][columna] = CELDA_IMPACTO; // Marca la celda como impactada
        }
        else if (tableroJugador[fila][columna] == CELDA_VACIA) { // Verifica si el ataque falló
            mensaje += "La computadora fallo."; // Agrega el mensaje de fallo
            tableroJugador[fila][columna] = CELDA_FALLO; // Marca la celda como fallo
        }
        else {
            continue; // Si la celda ya fue atacada, genera nuevas coordenadas
        }

        return mensaje; // Retorna el mensaje del resultado del ataque
    }
}

