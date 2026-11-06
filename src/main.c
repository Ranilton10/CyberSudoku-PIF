#include "raylib.h"

int main(void) {
    InitWindow(1000, 700, "Cyber Sudoku");
    SetTargetFPS(60);

    Color verdeNeon = {57, 255, 100, 255};
    Color fundo = {8, 12, 10, 255};

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(fundo);

        DrawText("CYBER SUDOKU", 310, 250, 45, verdeNeon);
        DrawText("SISTEMA INICIALIZADO", 365, 320, 20, GREEN);
        DrawText("Aguardando conexao...", 385, 360, 18, GRAY);

        DrawRectangle(250, 410, 500, 2, verdeNeon);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
