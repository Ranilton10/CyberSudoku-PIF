
#include "raylib.h"
#include <stdbool.h>

#define LARGURA 1000
#define ALTURA 700

typedef enum {
    MENU,
    PREPARACAO,
    RANKING,
    INSTRUCOES
} Tela;

bool desenharBotao(const char *texto, Rectangle botao, Color verde) {
    Vector2 mouse = GetMousePosition();

    bool mouseSobre = CheckCollisionPointRec(mouse, botao);

    Color fundoBotao = {18, 55, 32, 255};

    if (mouseSobre) {
        DrawRectangleRec(botao, verde);
    } else {
        DrawRectangleRec(botao, fundoBotao);
    }

    DrawRectangleLinesEx(botao, 2, verde);

    int tamanhoFonte = 24;
    int larguraTexto = MeasureText(texto, tamanhoFonte);

    DrawText(
        texto,
        (int)(botao.x + (botao.width - larguraTexto) / 2),
        (int)(botao.y + (botao.height - tamanhoFonte) / 2),
        tamanhoFonte,
        mouseSobre ? BLACK : verde
    );

    return mouseSobre && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void desenharFundo(Color fundo, Color verde) {
    ClearBackground(fundo);

    Color linha = Fade(verde, 0.12f);

    for (int i = 0; i < 20; i++) {
        float x = (float)(i * 55);

        DrawLineV(
            (Vector2){x, 0},
            (Vector2){x, ALTURA},
            linha
        );
    }

    for (int i = 0; i < 15; i++) {
        float y = (float)(i * 50);

        DrawLineV(
            (Vector2){0, y},
            (Vector2){LARGURA, y},
            linha
        );
    }
}

void desenharTitulo(Color verde) {
    const char *titulo = "CYBER SUDOKU";

    int larguraTexto = MeasureText(titulo, 45);

    DrawText(
        titulo,
        (LARGURA - larguraTexto) / 2,
        90,
        45,
        verde
    );

    const char *subtitulo = "SISTEMA DE INVASAO";

    larguraTexto = MeasureText(subtitulo, 20);

    DrawText(
        subtitulo,
        (LARGURA - larguraTexto) / 2,
        150,
        20,
        GRAY
    );
}

int main(void) {
    InitWindow(LARGURA, ALTURA, "Cyber Sudoku");
    SetTargetFPS(60);

    Color fundo = {8, 12, 10, 255};
    Color verde = {57, 255, 100, 255};

    Tela telaAtual = MENU;

    bool sair = false;

    const char *opcoes[] = {
        "JOGAR",
        "COMO JOGAR",
        "RANKING",
        "SAIR"
    };

    while (!WindowShouldClose() && !sair) {
        BeginDrawing();

        desenharFundo(fundo, verde);
        desenharTitulo(verde);

        if (telaAtual == MENU) {
            for (int i = 0; i < 4; i++) {
                Rectangle botao = {
                    350,
                    (float)(240 + i * 85),
                    300,
                    60
                };

                if (desenharBotao(opcoes[i], botao, verde)) {
                    if (i == 0) {
                        telaAtual = PREPARACAO;
                    } else if (i == 1) {
                        telaAtual = INSTRUCOES;
                    } else if (i == 2) {
                        telaAtual = RANKING;
                    } else if (i == 3) {
                        sair = true;
                    }
                }
            }
        }

        else if (telaAtual == PREPARACAO) {
            DrawText(
                "FASE 1 - ACESSO INICIAL",
                300, 280, 28, verde
            );

            DrawText(
                "Preparando sistema de seguranca...",
                290, 340, 20, GRAY
            );

            DrawText(
                "Tabuleiro 4x4 em desenvolvimento",
                310, 390, 20, WHITE
            );
        }

        else if (telaAtual == RANKING) {
            DrawText(
                "RANKING DE HACKERS",
                330, 280, 30, verde
            );

            DrawText(
                "Nenhuma partida registrada.",
                330, 340, 20, GRAY
            );
        }

        else if (telaAtual == INSTRUCOES) {
            DrawText(
                "COMO JOGAR",
                370, 220, 30, verde
            );

            DrawText(
                "Complete linhas, colunas e blocos",
                280, 290, 20, WHITE
            );

            DrawText(
                "sem repetir os simbolos.",
                340, 325, 20, WHITE
            );

            DrawText(
                "FASE 1: Sudoku 4x4 - Treinamento",
                300, 385, 20, verde
            );

            DrawText(
                "FASE 2: Sudoku 9x9 - Firewall",
                300, 420, 20, verde
            );

            DrawText(
                "FASE 3: Sudoku 9x9 - Sistema Central",
                300, 455, 20, verde
            );
        }

        if (telaAtual != MENU) {
            Rectangle voltar = {400, 550, 200, 50};

            if (desenharBotao("VOLTAR", voltar, verde)) {
                telaAtual = MENU;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
