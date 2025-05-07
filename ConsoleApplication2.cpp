#include <iostream>
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
void colocarBarcosOrdenador(Tablero& tablero);
bool todosBarcosHundidos(const Tablero& tablero);
void turnoJugador(Tablero& tableroOrdenador, Tablero& vistaOrdenador);
void turnoOrdenador(Tablero& tableroJugador, set<pair<int, int>>& ataquesRealizados, string& mensajeAtaque);

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Semilla para aleatorios

    Tablero tableroJugador, tableroOrdenador, vistaOrdenador;
    inicializarTablero(tableroJugador);
    inicializarTablero(tableroOrdenador);
    inicializarTablero(vistaOrdenador);

    // Colocar barcos jugador y ordenador
    colocarBarcosJugador(tableroJugador);
    colocarBarcosOrdenador(tableroOrdenador);

    set<pair<int, int>> ataquesComputadora; // Para evitar ataques repetidos de la computadora
    string mensajeAtaqueComputadora = "";  // Para mostrar abajo del tablero del oponente el mensaje

    cout << "\n¡Comienza el juego de Batalla Naval!\n";

    while (true) {
        limpiarPantalla();

        // Mostrar tableros actualizados
        cout << "\nTu Tablero:" << endl;
        mostrarTablero(tableroJugador, false);  // Mostrar todos los barcos y estados

        cout << "\nTablero del Oponente (lo que tú ves):" << endl;
        mostrarTablero(vistaOrdenador, true); // Ocultar barcos del ordenador

        // Mostrar mensaje del último ataque de la computadora
        if (!mensajeAtaqueComputadora.empty()) {
            cout << "\nMensaje de la computadora: " << mensajeAtaqueComputadora << endl;
        }

        turnoJugador(tableroOrdenador, vistaOrdenador);
        if (todosBarcosHundidos(tableroOrdenador)) {
            limpiarPantalla();
            cout << "\nTu Tablero Final:" << endl;
            mostrarTablero(tableroJugador, false);
            cout << "\nTablero Final del Oponente:" << endl;
            mostrarTablero(tableroOrdenador, false);
            cout << "\n¡Felicidades! Hundiste todos los barcos del oponente. ¡Ganaste!" << endl;
            break;
        }

        limpiarPantalla();
        cout << "\nTurno de la computadora..." << endl;
        turnoOrdenador(tableroJugador, ataquesComputadora, mensajeAtaqueComputadora);
        if (todosBarcosHundidos(tableroJugador)) {
            limpiarPantalla();
            cout << "\nTablero Final:" << endl;
            mostrarTablero(tableroJugador, false);
            cout << "\nTablero Final del Oponente:" << endl;
            mostrarTablero(tableroOrdenador, false);
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
                cout << " " << CELDA_VACIA; // Oculta barcos para que no se vean
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

bool colocarBarcoAleatorio(Tablero& tablero, int tamano) {
    for (int intentos = 0; intentos < 100; ++intentos) { // Limitar intentos para evitar loops infinitos
        int fila = rand() % TAMANO_TABLERO;
        int columna = rand() % TAMANO_TABLERO;
        bool horizontal = rand() % 2 == 0;

        if (puedeColocarBarco(tablero, fila, columna, tamano, horizontal)) {
            colocarBarco(tablero, fila, columna, tamano, horizontal);
            return true; // Colocado exitosamente
        }
    }
    return false; // No se pudo colocar en 100 intentos (muy improbable)
}

void colocarBarcosJugador(Tablero& tablero) {
    for (auto& barco : barcos) {
        bool colocado = false;
        while (!colocado) {
            limpiarPantalla();
            cout << "Coloca tus barcos en el tablero." << endl;
            mostrarTablero(tablero, false);
            cout << "Coloca tu " << barco.nombre << " (tamano " << barco.tamano << ")." << endl;
            cout << "Ingresa la posicion (ej. A1) o 'A' para colocar aleatoriamente: ";
            string entrada;
            cin >> entrada;

            if (entrada.length() == 1 && (entrada[0] == 'A' || entrada[0] == 'a')) {
                // Colocación aleatoria
                if (colocarBarcoAleatorio(tablero, barco.tamano)) {
                    colocado = true;
                }
                else {
                    cout << "No se pudo colocar aleatoriamente el barco. Intenta manualmente." << endl;
                    // Seguir en el ciclo para permitir intentarlo manual
                }
                continue;
            }

            // Colocación manual
            if (entrada.length() < 2 || !isalpha(entrada[0]) || !isdigit(entrada[1])) {
                cout << "Entrada invalida. Intenta de nuevo." << endl;
                continue;
            }

            int fila = toupper(entrada[0]) - 'A';
            int columna;
            try {
                columna = stoi(entrada.substr(1)) - 1;
            }
            catch (invalid_argument&) {
                cout << "Entrada invalida. Intenta de nuevo." << endl;
                continue;
            }

            if (fila < 0 || fila >= TAMANO_TABLERO || columna < 0 || columna >= TAMANO_TABLERO) {
                cout << "Coordenadas fuera de rango. Intenta de nuevo." << endl;
                continue;
            }

            cout << "Horizontal (h) o vertical (v)? ";
            char dir;
            cin >> dir;
            bool horizontal = (tolower(dir) == 'h');

            if (!puedeColocarBarco(tablero, fila, columna, barco.tamano, horizontal)) {
                cout << "No se puede colocar el barco aqui. Intenta de nuevo." << endl;
                continue;
            }

            colocarBarco(tablero, fila, columna, barco.tamano, horizontal);
            colocado = true;
        }
    }
}

void colocarBarcosOrdenador(Tablero& tablero) {
    for (auto& barco : barcos) {
        bool colocado = false;
        while (!colocado) {
            int fila = rand() % TAMANO_TABLERO;
            int columna = rand() % TAMANO_TABLERO;
            bool horizontal = rand() % 2 == 0;
            if (puedeColocarBarco(tablero, fila, columna, barco.tamano, horizontal)) {
                colocarBarco(tablero, fila, columna, barco.tamano, horizontal);
                colocado = true;
            }
        }
    }
}

bool todosBarcosHundidos(const Tablero& tablero) {
    for (int r = 0; r < TAMANO_TABLERO; ++r) {
        for (int c = 0; c < TAMANO_TABLERO; ++c) {
            if (tablero[r][c] == CELDA_BARCO) return false;
        }
    }
    return true;
}

void turnoJugador(Tablero& tableroOrdenador, Tablero& vistaOrdenador) {
    while (true) {
        cout << "Ingresa el objetivo a atacar (ej. B3): ";
        string pos;
        cin >> pos;

        if (pos.length() < 2 || !isalpha(pos[0]) || !isdigit(pos[1])) {
            cout << "Entrada invalida. Intenta de nuevo." << endl;
            continue;
        }

        int fila = toupper(pos[0]) - 'A';
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

        if (vistaOrdenador[fila][columna] == CELDA_IMPACTO || vistaOrdenador[fila][columna] == CELDA_FALLO) {
            cout << "Ya atacaste esta posicion. Elige otra." << endl;
            continue;
        }

        cout << "Procesando ataque en " << char('A' + fila) << (columna + 1) << "..." << endl;

        if (tableroOrdenador[fila][columna] == CELDA_BARCO) {
            cout << "Barco impactado!" << endl;
            tableroOrdenador[fila][columna] = CELDA_IMPACTO;
            vistaOrdenador[fila][columna] = CELDA_IMPACTO;
        }
        else {
            cout << "Agua." << endl;
            vistaOrdenador[fila][columna] = CELDA_FALLO;
        }

        break;
    }
}

void turnoOrdenador(Tablero& tableroJugador, set<pair<int, int >> & ataquesRealizados, string& mensajeAtaque) {
    while (true) {
        int fila = rand() % TAMANO_TABLERO;
        int columna = rand() % TAMANO_TABLERO;

        if (ataquesRealizados.count({ fila, columna }) > 0) continue;

        ataquesRealizados.insert({ fila, columna });

        mensajeAtaque = "La computadora te ataco en ";
        mensajeAtaque += char('A' + fila);
        mensajeAtaque += to_string(columna + 1);
        mensajeAtaque += ". ";

        if (tableroJugador[fila][columna] == CELDA_BARCO) {
            mensajeAtaque += "¡La computadora impacto tu barco!";
            tableroJugador[fila][columna] = CELDA_IMPACTO;
        }
        else if (tableroJugador[fila][columna] == CELDA_VACIA) {
            mensajeAtaque += "La computadora fallo.";
            tableroJugador[fila][columna] = CELDA_FALLO;
        }
        else {
            continue;
        }

        break;
    }
}
