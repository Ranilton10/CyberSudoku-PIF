
#include "raylib.h"
#include "menu.h"
#include "jogo.h"
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
    bool jogoIniciado = false;

    Jogo jogo;

    while (!WindowShouldClose() && !sair) {
        if (telaAtual == PREPARACAO) {
            if (!jogoIniciado) {
                iniciarJogo(&jogo);
                jogoIniciado = true;
            }

            atualizarJogo(&jogo);

            if (IsKeyPressed(KEY_ESCAPE)) {
                telaAtual = MENU;
                jogoIniciado = false;
            }
        }

        BeginDrawing();

        desenharFundo(fundo, verde);

        if (telaAtual == PREPARACAO) {
            desenharJogo(&jogo);
        } else {
            desenharTitulo(verde);
            desenharMenu(&telaAtual, &sair, verde);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
