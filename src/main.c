
#include "raylib.h"
#include "menu.h"
#include <stdbool.h>

#define LARGURA 1000
#define ALTURA 700

int main(void) {
    InitWindow(LARGURA, ALTURA, "Cyber Sudoku");
    SetTargetFPS(60);

    Color fundo = {8, 12, 10, 255};
    Color verde = {57, 255, 100, 255};

    Tela telaAtual = MENU;
    bool sair = false;

    while (!WindowShouldClose() && !sair) {
        BeginDrawing();

        desenharFundo(fundo, verde);
        desenharTitulo(verde);
        desenharMenu(&telaAtual, &sair, verde);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
