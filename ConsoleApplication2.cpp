#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cctype> // Para toupper y tolower
#include <stdexcept> // Para manejar excepciones de stoi
#include <string> // Para usar std::stoi

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

// Prototipos de funciones
void inicializarTablero(Tablero& tablero);
void mostrarTablero(const Tablero& tablero, bool ocultarBarcos);
bool puedeColocarBarco(const Tablero& tablero, int r, int c, int tamano, bool horizontal);
void colocarBarco(Tablero& tablero, int r, int c, int tamano, bool horizontal);
void colocarBarcosJugador(Tablero& tablero);
void colocarBarcosOrdenador(Tablero& tablero);
bool todosBarcosHundidos(const Tablero& tablero);
void turnoJugador(Tablero& tableroOrdenador, Tablero& vistaOrdenador);
void turnoOrdenador(Tablero& tableroJugador);

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Solucion a C4244

    Tablero tableroJugador, tableroOrdenador, vistaOrdenador;
    inicializarTablero(tableroJugador);
    inicializarTablero(tableroOrdenador);
    inicializarTablero(vistaOrdenador);

    cout << "Bienvenido a Batalla Naval!" << endl;
    colocarBarcosJugador(tableroJugador);
    colocarBarcosOrdenador(tableroOrdenador);

    while (true) {
        cout << "\nTu Tablero:" << endl;
        mostrarTablero(tableroJugador, false);

        cout << "\nTablero del Ordenador:" << endl;
        mostrarTablero(vistaOrdenador, true);

        turnoJugador(tableroOrdenador, vistaOrdenador);
        if (todosBarcosHundidos(tableroOrdenador)) {
            cout << "Felicidades! Hundiste todos los barcos del oponente. Ganaste!" << endl;
            break;
        }

        cout << "\nTurno de la computadora..." << endl;
        turnoOrdenador(tableroJugador);
        if (todosBarcosHundidos(tableroJugador)) {
            cout << "Todos tus barcos han sido hundidos. Perdiste!" << endl;
            break;
        }
    }

    cout << "\nTableros Finales:" << endl;
    cout << "Tu Tablero:" << endl;
    mostrarTablero(tableroJugador, false);
    cout << "Tablero del Ordenador:" << endl;
    mostrarTablero(tableroOrdenador, false);

    return 0;
}

// Definiciones de funciones
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
                cout << " " << CELDA_VACIA;
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

void colocarBarcosJugador(Tablero& tablero) {
    cout << "Coloca tus barcos en el tablero." << endl;
    for (auto& barco : barcos) {
        bool colocado = false;
        while (!colocado) {
            mostrarTablero(tablero, false);
            cout << "Coloca tu " << barco.nombre << " (tamano " << barco.tamano << ")." << endl;
            cout << "Ingresa la posicion (ej. A1): ";
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
        mostrarTablero(vistaOrdenador, false);
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

void turnoOrdenador(Tablero& tableroJugador) {
    while (true) {
        int fila = rand() % TAMANO_TABLERO;
        int columna = rand() % TAMANO_TABLERO;

        if (tableroJugador[fila][columna] == CELDA_IMPACTO || tableroJugador[fila][columna] == CELDA_FALLO) {
            continue;
        }

        cout << "La computadora te atacó en " << char('A' + fila) << (columna + 1) << "." << endl;

        if (tableroJugador[fila][columna] == CELDA_BARCO) {
            cout << "La computadora impactó tu barco!" << endl;
            tableroJugador[fila][columna] = CELDA_IMPACTO;
        }
        else {
            cout << "La computadora falló." << endl;
            tableroJugador[fila][columna] = CELDA_FALLO;
        }

        break;
    }
}
