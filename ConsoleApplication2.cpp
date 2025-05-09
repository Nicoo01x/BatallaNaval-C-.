﻿#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cctype> // Para toupper y tolower
#include <stdexcept> // Para manejar excepciones de stoi
#include <string> 
#include <set>



using namespace std;

const int TAMANO_TABLERO = 8;
const char CELDA_VACIA = '~';
const char CELDA_BARCO = 'S';
const char CELDA_IMPACTO = 'X';
const char CELDA_FALLO = 'O';

struct Barco {
    int tamano;
    string nombre;
};

vector<Barco> barcos = {
    {4, "Porta Aviones"},
    {3, "Submarino"},
    {3, "Crucero"},
    {2, "Lancha"},
    {1, "Kayak"}
};



typedef vector<vector<char>> Tablero;

// Función para limpiar la pantalla usando secuencia ANSI
void limpiarPantalla() {
    cout << "\033[2J\033[H";
}

// Prototipos de funciones
void inicializarTablero(Tablero& tablero);
void mostrarTablero(const Tablero& tablero, bool ocultarBarcos);
bool puedeColocarBarco(const Tablero& tablero, int r, int c, int tamano, bool horizontal);
void colocarBarco(Tablero& tablero, int r, int c, int tamano, bool horizontal);
void colocarBarcosJugador(Tablero& tablero);
bool colocarBarcoAleatorio(Tablero& tablero, int tamano);
void colocarBarcosComputadora(Tablero& tablero);
bool todosBarcosHundidos(const Tablero& tablero);
string turnoJugador(Tablero& tableroComputadora, Tablero& vistaComputadora); // vistaComputadora es el tablero que ve el jugador, sin barcos
string turnoComputadora(Tablero& tableroJugador, set<pair<int, int>>& ataquesRealizados); // set para ataques realizados

int main() {


    cout << "Bienvenido a la Batalla Naval" << endl;
    cout << "Creado por @Nicolas Cabanillas y @Sebastian Revuelta" << endl;
    cout << "Presione cualquier tecla para iniciar el juego" << endl;
	cin.get(); // Esta linea espera a que el usuario presione una tecla
	limpiarPantalla();

    srand(time(0)); // esta linea inicializa la semilla para generar numeros aleatorios

    Tablero tableroJugador, tableroComputadora, vistaComputadora;
    inicializarTablero(tableroJugador);
    inicializarTablero(tableroComputadora);
    inicializarTablero(vistaComputadora);

    // Colocar barcos jugador y computadora
    colocarBarcosJugador(tableroJugador);
    colocarBarcosComputadora(tableroComputadora);  

    set<pair<int, int>> ataquesComputadora; // Para ataques ya realizados
    string mensajeAtaqueComputadora;  // Mensaje para mostrar debajo tablero Oponente
    string mensajeTurnoJugador;       // Mensaje para mostrar debajo tablero Computadora
    // Bucle principal del juego
    while (true) { 
        limpiarPantalla();
        
        // Mostrar tableros
        cout << "Tu Tablero:" << endl;
        mostrarTablero(tableroJugador, false);  // mostrar barcos y estados
        
        cout << "\nTablero del Oponente (lo que tu ves):" << endl;
        mostrarTablero(vistaComputadora, true); // ocultar barcos

        // Mostrar mensajes debajo de tableros
        if (!mensajeTurnoJugador.empty()) {
            cout << "\n(Jugador): " << mensajeTurnoJugador << endl;
        }
        if (!mensajeAtaqueComputadora.empty()) {
            cout << "\n(Computadora): " << mensajeAtaqueComputadora << endl;
        }
        cout << endl;

        // Turno jugador
        mensajeTurnoJugador = turnoJugador(tableroComputadora, vistaComputadora);
        if (todosBarcosHundidos(tableroComputadora)) {
            limpiarPantalla();
            cout << "Tu Tablero Final:" << endl;
            mostrarTablero(tableroJugador, false);
            cout << "\nTablero Final del Oponente:" << endl;
            mostrarTablero(tableroComputadora, false);
            cout << "\n¡Felicidades! Hundiste todos los barcos del oponente. ¡Ganaste!" << endl;
            break;
        }

        limpiarPantalla();

        cout << "Turno de la computadora..." << endl;
        mensajeAtaqueComputadora = turnoComputadora(tableroJugador, ataquesComputadora);
        if (todosBarcosHundidos(tableroJugador)) {
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

void inicializarTablero(Tablero& tablero) {
    tablero.resize(TAMANO_TABLERO, vector<char>(TAMANO_TABLERO, CELDA_VACIA));
}

void mostrarTablero(const Tablero& tablero, bool ocultarBarcos) {
    cout << "  ";
    for (int c = 0; c < TAMANO_TABLERO; ++c)
        cout << " " << c + 1;
    cout << endl;

    for (int r = 0; r < TAMANO_TABLERO; ++r) {
        cout << char('A' + r) << " ";
        for (int c = 0; c < TAMANO_TABLERO; ++c) {
            char celda = tablero[r][c];
            if (ocultarBarcos && celda == CELDA_BARCO)
                cout << " " << CELDA_VACIA; // Oculta barcos
            else
                cout << " " << celda;
        }
        cout << endl;
    }
}

bool puedeColocarBarco(const Tablero& tablero, int r, int c, int tamano, bool horizontal) {
    if (horizontal) {
        if (c + tamano > TAMANO_TABLERO) return false;
        for (int i = 0; i < tamano; ++i) {
            if (tablero[r][c + i] != CELDA_VACIA) return false;
        }
    }
    else {
        if (r + tamano > TAMANO_TABLERO) return false;
        for (int i = 0; i < tamano; ++i) {
            if (tablero[r + i][c] != CELDA_VACIA) return false;
        }
    }
    return true;
}

void colocarBarco(Tablero& tablero, int r, int c, int tamano, bool horizontal) {
    if (horizontal) {
        for (int i = 0; i < tamano; ++i) {
            tablero[r][c + i] = CELDA_BARCO;
        }
    }
    else {
        for (int i = 0; i < tamano; ++i) {
            tablero[r + i][c] = CELDA_BARCO;
        }
    }
}

bool colocarBarcoAleatorio(Tablero& tablero, int tamano) { // Esta funcion coloca un barco aleatoriamente en el tablero
    for (int intentos = 0; intentos < 100; ++intentos) {
        int fila = rand() % TAMANO_TABLERO;
        int columna = rand() % TAMANO_TABLERO;
        bool horizontal = rand() % 2 == 0;

		if (puedeColocarBarco(tablero, fila, columna, tamano, horizontal)) { // puedeColocarBarco verifica si se puede colocar el barco en la posicion dada
            colocarBarco(tablero, fila, columna, tamano, horizontal);
            return true;
        }
    }
    return false;
}

void colocarBarcosJugador(Tablero& tablero) {
	for (auto& barco : barcos) { // esta linea recorre el vector barcos y obtiene cada barco
        bool colocado = false;
        while (!colocado) {
            limpiarPantalla();
            cout << "Coloca tus barcos en el tablero." << endl;
            mostrarTablero(tablero, false);
            cout << "\nColoca tu " << barco.nombre << " (tamano " << barco.tamano << ")." << endl;
            cout << "Ingresa la posicion (ejemplo: A1) o 'A' para colocar aleatoriamente: ";
            string entrada;
            cin >> entrada;

            if (entrada.length() == 1 && (entrada[0] == 'A' || entrada[0] == 'a')) {
                if (colocarBarcoAleatorio(tablero, barco.tamano)) {
                    colocado = true;
                }
                else {
                    cout << "No se pudo colocar aleatoriamente el barco. Intenta manualmente." << endl;
                }
                continue;
            }

			if (entrada.length() < 2 || !isalpha(entrada[0]) || !isdigit(entrada[1])) { //length verifica la longitud de la cadena
                cout << "Entrada invalida. Intenta de nuevo." << endl;
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // esta linea ignora el resto de la entrada
                continue;
            }

            int fila = toupper(entrada[0]) - 'A';
            int columna;
            try {
                columna = stoi(entrada.substr(1)) - 1;
            }
            catch (invalid_argument&) {
                cout << "Entrada invalida. Intenta de nuevo." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (fila < 0 || fila >= TAMANO_TABLERO || columna < 0 || columna >= TAMANO_TABLERO) {
                cout << "Coordenadas fuera de rango. Intenta de nuevo." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                continue;
            }

            cout << "Horizontal (h) o vertical (v)? ";
            char dir;
            cin >> dir;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
			bool horizontal = (tolower(dir) == 'h'); // tolower convierte a minuscula

            if (!puedeColocarBarco(tablero, fila, columna, barco.tamano, horizontal)) {
                cout << "No se puede colocar el barco aqui. Intenta de nuevo." << endl;
                continue;
            }

            colocarBarco(tablero, fila, columna, barco.tamano, horizontal);
            colocado = true;
        }
    }
}

void colocarBarcosComputadora(Tablero& tablero) {
    for (auto& barco : barcos) {
        bool colocado = false;
		while (!colocado) { // ! es una negacion, si colocado es falso entra al bucle
            int fila = rand() % TAMANO_TABLERO;
			int columna = rand() % TAMANO_TABLERO; // rand genera un numero aleatorio
            bool horizontal = rand() % 2 == 0;
            if (puedeColocarBarco(tablero, fila, columna, barco.tamano, horizontal)) {
                colocarBarco(tablero, fila, columna, barco.tamano, horizontal);
                colocado = true;
            }
        }
    }
}

bool todosBarcosHundidos(const Tablero & tablero) { // Esta funcion verifica si todos los barcos han sido hundidos
    for (int r = 0; r < TAMANO_TABLERO; ++r) {
        for (int c = 0; c < TAMANO_TABLERO; ++c) {
            if (tablero[r][c] == CELDA_BARCO) return false;
        }
    }
    return true;
}

string turnoJugador(Tablero& tableroComputadora, Tablero& vistaComputadora) {
    while (true) {
        cout << "Ingresa el objetivo a atacar (ej. B3): ";
        string pos;
        cin >> pos;

		if (pos.length() < 2 || !isalpha(pos[0]) || !isdigit(pos[1])) { // isdigit verifica si el caracter es un digito y isalpha verifica si es una letra
            cout << "Entrada invalida. Intenta de nuevo." << endl;
            continue;
        }

		int fila = toupper(pos[0]) - 'A'; //toupper convierte a mayuscula
        int columna;
        try {
			columna = stoi(pos.substr(1)) - 1; // stoi convierte a entero string a int y substr(1) obtiene la parte de la cadena que contiene el numero
        }
        catch (invalid_argument&) {
            cout << "Entrada invalida. Intenta de nuevo." << endl;
            continue;
        }

		if (fila < 0 || fila >= TAMANO_TABLERO || columna < 0 || columna >= TAMANO_TABLERO) { // esta linea verifica si la fila y columna estan dentro del rango
            cout << "Coordenadas fuera de rango. Intenta de nuevo." << endl;
            continue;
        }

		if (vistaComputadora[fila][columna] == CELDA_IMPACTO || vistaComputadora[fila][columna] == CELDA_FALLO) { // esta linea verifica si la celda ya fue atacada
            cout << "Ya atacaste esta posicion. Elige otra." << endl;
            continue;
        }

        string mensaje = "Procesando ataque en ";
        mensaje += char('A' + fila);
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

string turnoComputadora(Tablero& tableroJugador, set<pair<int, int >>& ataquesRealizados) { //set pair<int, int> es un contenedor que almacena pares de enteros y no permite duplicados
    while (true) {
        int fila = rand() % TAMANO_TABLERO;
        int columna = rand() % TAMANO_TABLERO;

        if (ataquesRealizados.count({ fila, columna }) > 0) continue;

        ataquesRealizados.insert({ fila, columna });

        string mensaje = "La computadora te ataco en ";
        mensaje += char('A' + fila);
        mensaje += to_string(columna + 1);
		mensaje += ". "; // += se usa para sumar una valor a la variable y remplazarlo por el nuevo valor

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

        return mensaje;
    }
}

// Autor: Nicolás Cabanillas y Sebastian Revuelta.
// Universidad Blas Pascal.