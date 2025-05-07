# Documentación del Proyecto

## 🧩 Librerías (Includes)

- `#include <iostream>`  
  Permite usar `cout`, `cin`, `endl`, es decir, para mostrar información en pantalla o leer la entrada del usuario.

- `#include <vector>`  
  Habilita el uso de `vector`, que son listas dinámicas que pueden cambiar de tamaño.

- `#include <ctime>`  
  Se utiliza para obtener la hora del sistema y usarla como semilla para generar números aleatorios.

- `#include <cstdlib>`  
  Habilita las funciones `rand()` y `srand()`, que permiten generar números aleatorios.

- `#include <cctype>`  
  Contiene funciones para transformar letras a mayúsculas (`toupper`) o minúsculas (`tolower`).

- `#include <stdexcept>`  
  Se utiliza para manejar errores, como cuando se intenta convertir una cadena a número (por ejemplo, `stoi`).

- `#include <string>`  
  Permite el uso del tipo `string`, que representa cadenas de texto.

- `#include <set>`  
  Habilita el uso de `set`, que es una colección que no permite elementos duplicados.

## 🔧 Constantes y Tipos

```cpp
const int TAMANO_TABLERO = 8; // Define el tamaño del tablero como 8x8.
const char CELDA_VACIA = '~'; // Representa una celda vacía con el símbolo ~.
const char CELDA_BARCO = 'S'; // Indica que hay un barco en la celda con la letra S.
const char CELDA_IMPACTO = 'X'; // Marca un impacto en un barco con una X.
const char CELDA_FALLO = 'O'; // Indica un fallo en el ataque con una O.

struct Barco { 
    int tamano; 
    string nombre; 
}; // Define la estructura de un barco.

vector<Barco> barcos = {...}; // Lista que contiene todos los barcos del juego.

typedef vector<vector<char>> Tablero; // Alias que simplifica la escritura.
