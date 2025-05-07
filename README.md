
# Batalla Naval - Explicación de Funciones y Comandos del Código

Este repositorio contiene el código completo del juego **Batalla Naval** en C++.  
A continuación se explica detalladamente cada función y cada comando utilizado en el proyecto.

---

## 📦 Estructuras y Definiciones

```cpp
const int TAMANO_TABLERO = 8;
```
Define el tamaño del tablero: 8x8.

```cpp
const char CELDA_VACIA = '~', CELDA_BARCO = 'S', CELDA_IMPACTO = 'X', CELDA_FALLO = 'O';
```
Caracteres que representan los distintos estados de una celda del tablero.

```cpp
struct Barco {
    int tamano;
    string nombre;
};
```
Estructura que representa un barco con su tamaño y nombre.

```cpp
typedef vector<vector<char>> Tablero;
```
Alias para usar `Tablero` en vez de escribir `vector<vector<char>>`.

---

## 🔧 Funciones

### `void limpiarPantalla()`
Borra la consola usando escape ANSI para que se vea más prolijo.

### `void inicializarTablero(Tablero& tablero)`
Rellena el tablero con `CELDA_VACIA` (~) en todas sus posiciones.

### `void mostrarTablero(const Tablero& tablero, bool ocultarBarcos)`
Imprime el tablero por consola. Si `ocultarBarcos` es `true`, oculta los barcos del oponente.

### `bool puedeColocarBarco(...)`
Verifica si un barco puede ser colocado en una posición determinada, sin salirse ni superponerse.

### `void colocarBarco(...)`
Coloca un barco en el tablero, una vez validada la posición.

### `bool colocarBarcoAleatorio(...)`
Intenta colocar un barco aleatoriamente. Reintenta hasta 100 veces como máximo.

### `void colocarBarcosJugador(Tablero& tablero)`
Permite al usuario colocar sus barcos manual o aleatoriamente.

### `void colocarBarcosOrdenador(Tablero& tablero)`
Coloca los barcos del ordenador en posiciones aleatorias válidas.

### `bool todosBarcosHundidos(const Tablero& tablero)`
Verifica si quedan barcos (`'S'`) en el tablero. Si no hay, el jugador pierde.

### `void turnoJugador(...)`
El jugador elige una posición a atacar. Se actualiza el tablero con impacto (`'X'`) o fallo (`'O'`).

### `void turnoOrdenador(...)`
La computadora ataca una posición aleatoria que no haya usado antes. Registra y muestra el resultado.

---

## 🧠 Comandos y Librerías Usadas

```cpp
#include <iostream>     // Entrada y salida de datos (cin, cout)
#include <vector>       // Uso de vectores (tableros y estructuras)
#include <ctime>        // Para inicializar la semilla de aleatoriedad
#include <cstdlib>      // rand(), srand()
#include <cctype>       // toupper(), tolower(), isalpha(), isdigit()
#include <stdexcept>    // Manejo de errores con stoi()
#include <string>       // Manejo de texto
#include <set>          // Almacena ataques ya hechos por la compu (sin repetidos)
```

- `rand()` y `srand()`: Generación de números aleatorios.
- `toupper()`, `tolower()`: Conversión de letras.
- `stoi()`: Convierte string a int.
- `set<pair<int, int>>`: Guarda pares únicos de coordenadas atacadas.

---

## ⚠️ Derechos Reservados

© Nicolas Cabanillas y Sebastián Revuelta.  
Todos los derechos reservados.  
Prohibida la modificación sin autorización de los autores.

---
