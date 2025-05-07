🧩 LIBRERÍAS (INCLUDES)
#include <iostream>
Sirve para usar cout, cin, endl, o sea: para mostrar cosas en pantalla o leer lo que pone el usuario.

#include <vector>
Te deja usar vector, que son como listas dinámicas (pueden cambiar de tamaño).

#include <ctime>
Sirve para obtener la hora del sistema y usarla como semilla para los números aleatorios.

#include <cstdlib>
Habilita rand() y srand(), o sea, funciones para tirar números al azar.

#include <cctype>
Tiene funciones para transformar letras a mayúsculas (toupper) o minúsculas (tolower).

#include <stdexcept>
Es para manejar errores, como cuando metés algo que no se puede convertir a número (stoi).

#include <string>
Habilita el uso del tipo string, o sea, cadenas de texto.

#include <set>
Permite usar set, que es como una lista pero que no repite elementos.

🔧 CONSTANTES Y TIPOS
const int TAMANO_TABLERO = 8;
El tablero es de 8x8.

const char CELDA_VACIA = '~';
Una celda sin nada tiene esta onda: ~.

const char CELDA_BARCO = 'S';
Letra que marca que ahí hay un barco.

const char CELDA_IMPACTO = 'X';
Cuando le pegás a un barco, se marca con una X.

const char CELDA_FALLO = 'O';
Si le pifiaste, aparece una O.

struct Barco { int tamano; string nombre; };
Un barco tiene un tamaño (cuántas celdas ocupa) y un nombre (como "Submarino").

vector<Barco> barcos = {...};
Lista con todos los barcos del juego (fragata, destructor, etc.).

typedef vector<vector<char>> Tablero;
Alias para no escribir tanto: un "Tablero" es una matriz de caracteres.

⚙️ FUNCIONES QUE HACEN EL LABURO
void limpiarPantalla()
Limpia la consola con escape codes ANSI. En sistemas tipo Linux/Mac, borra todo lo anterior.

void inicializarTablero(Tablero& tablero)
Crea un tablero vacío y lo llena con ~.

void mostrarTablero(const Tablero& tablero, bool ocultarBarcos)
Imprime el tablero en pantalla.

Si ocultarBarcos es true, no se muestran los barcos enemigos (para que no hagas trampa 😅).

bool puedeColocarBarco(...)
Fija si se puede poner un barco en cierta posición sin pasarse ni chocar con otro.

void colocarBarco(...)
Coloca efectivamente el barco en el tablero, marcando cada parte con una S.

bool colocarBarcoAleatorio(...)
Intenta meter un barco en una posición random del tablero.

Si en 100 intentos no puede, devuelve false.

void colocarBarcosJugador(Tablero& tablero)
Le pregunta al jugador si quiere colocar barcos manual o automáticamente.

Si es manual, te deja ponerlos tipo "A1", y si es válido, lo mete.

void colocarBarcosOrdenador(Tablero& tablero)
Coloca los barcos del oponente de forma aleatoria. Todo automático.

bool todosBarcosHundidos(const Tablero& tablero)
Revisa si todavía queda alguna S en el tablero.

Si no queda ninguna, todos los barcos están hundidos.

void turnoJugador(...)
Te pide que ingreses una coordenada para atacar.

Si acertás, se marca con X; si no, con O.

Guarda el resultado en la "vista" del jugador (así ves qué pegaste y qué no).

void turnoOrdenador(...)
El enemigo tira ataques random sin repetir coordenadas.

Guarda los ataques que ya hizo para no repetir y avisa si fue acierto o no.

🛠 FUNCIONES AUXILIARES Y COMANDOS QUE SE USAN
toupper() y tolower()
Transforman letras. Ejemplo: 'a' → 'A'.

stoi()
Convierte un texto a número. Si ponés algo raro, tira error.

rand()
Devuelve un número aleatorio.

srand()
Le mete una semilla al generador aleatorio (con time(NULL), por ejemplo).

cin, cout
Para leer y mostrar cosas al usuario.

'A' + i
Convierte números en letras: 0 → 'A', 1 → 'B', etc.

set<pair<int, int>>
Guarda coordenadas únicas para que el enemigo no ataque dos veces el mismo lugar.

continue
Salta al próximo paso del while o for.

break
Corta el ciclo.

try / catch
Bloques que manejan errores cuando algo puede salir mal (como una entrada inválida).
