
#include "jogo.h"
#include <stdio.h>

void iniciarJogo(Jogo *jogo) {
    int inicial[TAMANHO][TAMANHO] = {
        {1, 0, 0, 4},
        {0, 4, 1, 0},
        {2, 0, 0, 3},
        {0, 3, 2, 0}
    };

    int resposta[TAMANHO][TAMANHO] = {
        {1, 2, 3, 4},
        {3, 4, 1, 2},
        {2, 1, 4, 3},
        {4, 3, 2, 1}
    };

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            jogo->tabuleiro[i][j] = inicial[i][j];
            jogo->solucao[i][j] = resposta[i][j];
            jogo->fixas[i][j] = inicial[i][j] != 0;
        }
    }

    jogo->linhaSelecionada = -1;
    jogo->colunaSelecionada = -1;
    jogo->concluido = false;
}

bool verificarVitoria(Jogo *jogo) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (jogo->tabuleiro[i][j] != jogo->solucao[i][j]) {
                return false;
            }
        }
    }

    return true;
}

void atualizarJogo(Jogo *jogo) {
    if (jogo->concluido) {
        return;
    }

    int tamanhoCelula = 100;
    int inicioX = 300;
    int inicioY = 200;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        if (mouse.x >= inicioX && mouse.x < inicioX + TAMANHO * tamanhoCelula &&
            mouse.y >= inicioY && mouse.y < inicioY + TAMANHO * tamanhoCelula) {

            int coluna = (int)(mouse.x - inicioX) / tamanhoCelula;
            int linha = (int)(mouse.y - inicioY) / tamanhoCelula;

            if (!jogo->fixas[linha][coluna]) {
                if (jogo->linhaSelecionada == linha &&
                    jogo->colunaSelecionada == coluna &&
                    jogo->tabuleiro[linha][coluna] != 0) {
                    jogo->tabuleiro[linha][coluna] = 0;
                }

                jogo->linhaSelecionada = linha;
                jogo->colunaSelecionada = coluna;
            }
        }
    }

    int linha = jogo->linhaSelecionada;
    int coluna = jogo->colunaSelecionada;

    if (linha != -1 && coluna != -1) {
        for (int numero = 1; numero <= TAMANHO; numero++) {
            if (IsKeyPressed(KEY_ONE + numero - 1) ||
                IsKeyPressed(KEY_KP_1 + numero - 1)) {

                jogo->tabuleiro[linha][coluna] = numero;
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE)) {
            jogo->tabuleiro[linha][coluna] = 0;
        }
    }

    if (verificarVitoria(jogo)) {
        jogo->concluido = true;
    }
}

void desenharJogo(Jogo *jogo) {
    int tamanhoCelula = 100;
    int inicioX = 300;
    int inicioY = 200;

    Color verde = {57, 255, 100, 255};
    Color fundoCelula = {15, 30, 20, 255};
    Color selecionada = {30, 85, 45, 255};

    DrawText("FASE 1 - ACESSO INICIAL", 310, 130, 28, verde);

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            Rectangle celula = {
                (float)(inicioX + j * tamanhoCelula),
                (float)(inicioY + i * tamanhoCelula),
                (float)tamanhoCelula,
                (float)tamanhoCelula
            };

            Color cor = fundoCelula;

            if (jogo->linhaSelecionada == i &&
                jogo->colunaSelecionada == j) {
                cor = selecionada;
            }

            DrawRectangleRec(celula, cor);
            DrawRectangleLinesEx(celula, 1, verde);

            if (jogo->tabuleiro[i][j] != 0) {
                char texto[4];

                snprintf(texto, sizeof(texto), "%d", jogo->tabuleiro[i][j]);

                Color corNumero = jogo->fixas[i][j] ? WHITE : verde;

                int largura = MeasureText(texto, 40);

                DrawText(
                    texto,
                    (int)(celula.x + (tamanhoCelula - largura) / 2),
                    (int)(celula.y + 30),
                    40,
                    corNumero
                );
            }
        }
    }

    for (int i = 0; i <= TAMANHO; i += 2) {
        DrawRectangle(
            inicioX,
            inicioY + i * tamanhoCelula - 2,
            TAMANHO * tamanhoCelula,
            4,
            verde
        );

        DrawRectangle(
            inicioX + i * tamanhoCelula - 2,
            inicioY,
            4,
            TAMANHO * tamanhoCelula,
            verde
        );
    }

    if (jogo->concluido) {
        DrawText("SISTEMA INVADIDO!", 340, 620, 30, verde);
    } else {
        DrawText("Selecione uma celula e use 1, 2, 3 ou 4", 280, 620, 20, GRAY);
    }
}
