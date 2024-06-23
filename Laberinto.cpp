#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>

using namespace std;

class Laberinto {
public:
    Laberinto(int ancho, int alto);
    void generarLaberinto();
    void imprimirLaberinto();
    bool resolverLaberinto(int x, int y);
    static void solicitarTamanoTablero(int &filas, int &columnas);

private:
    int ancho, alto;
    vector<vector<string>> laberinto;
    default_random_engine rng;

    void inicializarLaberinto();
    void tallarCamino(int x, int y);
    bool estaDentroDeLosLimites(int x, int y);
    vector<pair<int, int>> obtenerVecinos(int x, int y);
    bool resolverLaberintoUtil(vector<vector<string>>& laberinto, int x, int y);
};

Laberinto::Laberinto(int ancho, int alto) : ancho(ancho), alto(alto), rng(random_device()()) {
    laberinto.resize(alto, vector<string>(ancho, "█"));
}

void Laberinto::generarLaberinto() {
    inicializarLaberinto();
    tallarCamino(1, 1);
    laberinto[1][1] = "E"; // Entrada
    laberinto[alto-2][ancho-2] = "S"; // Salida
}

void Laberinto::inicializarLaberinto() {
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            laberinto[y][x] = "█";
        }
    }
}

void Laberinto::tallarCamino(int x, int y) {
    stack<pair<int, int>> pilaCamino;
    pilaCamino.push({x, y});
    laberinto[y][x] = " ";

    while (!pilaCamino.empty()) {
        auto [cx, cy] = pilaCamino.top();
        vector<pair<int, int>> vecinos;

        // Recoger vecinos válidos
        if (estaDentroDeLosLimites(cx + 2, cy) && laberinto[cy][cx + 2] == "█") vecinos.push_back({cx + 2, cy});
        if (estaDentroDeLosLimites(cx - 2, cy) && laberinto[cy][cx - 2] == "█") vecinos.push_back({cx - 2, cy});
        if (estaDentroDeLosLimites(cx, cy + 2) && laberinto[cy + 2][cx] == "█") vecinos.push_back({cx, cy + 2});
        if (estaDentroDeLosLimites(cx, cy - 2) && laberinto[cy - 2][cx] == "█") vecinos.push_back({cx, cy - 2});

        if (!vecinos.empty()) {
            // Elegir un vecino al azar
            shuffle(vecinos.begin(), vecinos.end(), rng);
            auto [nx, ny] = vecinos.back();

            // Eliminar muro entre la celda actual y el vecino elegido
            laberinto[(cy + ny) / 2][(cx + nx) / 2] = " ";
            // Marcar la celda vecina como parte del camino
            laberinto[ny][nx] = " ";
            // Empujar la celda vecina a la pila
            pilaCamino.push({nx, ny});
        } else {
            pilaCamino.pop();
        }
    }
}

bool Laberinto::estaDentroDeLosLimites(int x, int y) {
    return x > 0 && x < ancho - 1 && y > 0 && y < alto - 1;
}

void Laberinto::imprimirLaberinto() {
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            cout << laberinto[y][x];
        }
        cout << endl;
    }
}

bool Laberinto::resolverLaberinto(int x, int y) {
    return resolverLaberintoUtil(laberinto, x, y);
}

bool Laberinto::resolverLaberintoUtil(vector<vector<string>>& laberinto, int x, int y) {
    // Si llegamos a la salida
    if (laberinto[x][y] == "S") {
        return true;
    }

    // Si la celda actual es un espacio libre ( ) o entrada (E)
    if (laberinto[x][y] == " " || laberinto[x][y] == "E") {
        // Marcar la celda como parte del camino
        laberinto[x][y] = "*";

        // Movimientos posibles (arriba, derecha, abajo, izquierda)
        int dx[4] = {-1, 0, 1, 0};
        int dy[4] = {0, 1, 0, -1};

        // Intentar cada movimiento
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Verificar si el movimiento es válido
            if (nx >= 0 && nx < alto && ny >= 0 && ny < ancho && (laberinto[nx][ny] == " " || laberinto[nx][ny] == "S")) {
                if (resolverLaberintoUtil(laberinto, nx, ny)) {
                    return true;
                }
            }
        }

        // Si no se encontró una salida, desmarcar la celda (backtracking)
        if (laberinto[x][y] != "E") {
            laberinto[x][y] = " ";
        }
    }

    return false;
}

void Laberinto::solicitarTamanoTablero(int &filas, int &columnas) {
    cout << "Ingrese el número de filas del tablero: ";
    cin >> filas;
    cout << "Ingrese el número de columnas del tablero: ";
    cin >> columnas;
}

int main() {
    srand(time(0));

    int filas, columnas;
    Laberinto::solicitarTamanoTablero(filas, columnas);

    // Asegurarse de que el tamaño sea impar
    if (filas % 2 == 0) filas++;
    if (columnas % 2 == 0) columnas++;

    Laberinto laberinto(filas, columnas);
    laberinto.generarLaberinto();
    laberinto.imprimirLaberinto();

    cout << "Resolviendo el laberinto..." << endl;
    if (laberinto.resolverLaberinto(1, 1)) {
        cout << "Laberinto resuelto:" << endl;
        laberinto.imprimirLaberinto();
    } else {
        cout << "No se encontró solución para el laberinto." << endl;
    }

    return 0;
}
