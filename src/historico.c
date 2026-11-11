
#include "historico.h"
#include <stdlib.h>

void adicionarJogada(Jogada **inicio, int linha, int coluna, int simbolo, int apagou) {
    Jogada *nova = malloc(sizeof(Jogada));

    if (nova == NULL) {
        return;
    }

    nova->linha = linha;
    nova->coluna = coluna;
    nova->simbolo = simbolo;
    nova->apagou = apagou;
    nova->proximo = *inicio;

    *inicio = nova;
}

void desenharHistorico(Jogada *inicio, Texture2D simbolos[]) {
    Color verde = {57, 255, 100, 255};
    Color verdeClaro = {120, 255, 160, 255};
    Color cinza = {165, 190, 175, 255};

    Rectangle painel = {35, 210, 220, 310};

    DrawRectangleRounded(
        painel,
        0.06f,
        8,
        (Color){12, 30, 21, 255}
    );

    DrawRectangleRoundedLinesEx(
        painel,
        0.06f,
        8,
        2,
        verde
    );

    DrawText("HISTORICO", 78, 230, 20, verde);
    DrawLine(50, 263, 240, 263, Fade(verde, 0.6f));

    if (inicio == NULL) {
        DrawText("Nenhuma jogada", 57, 310, 15, cinza);
        DrawText("registrada.", 57, 333, 15, cinza);
        return;
    }

    Jogada *atual = inicio;
    int contador = 0;

    while (atual != NULL && contador < 6) {
        int y = 282 + contador * 36;

        DrawText(
            TextFormat("L%d C%d", atual->linha + 1, atual->coluna + 1),
            53,
            y + 5,
            16,
            verdeClaro
        );

        if (atual->apagou) {
            DrawText("APAGOU", 140, y + 5, 14, ORANGE);
        } else {
            DrawText("=", 139, y + 5, 16, cinza);

            int indice = atual->simbolo - 1;

            if (indice >= 0 && indice < 9 && simbolos[indice].id != 0) {
                Rectangle origem = {
                    0,
                    0,
                    (float)simbolos[indice].width,
                    (float)simbolos[indice].height
                };

                Rectangle destino = {171, y + 2, 25, 25};

                DrawTexturePro(
                    simbolos[indice],
                    origem,
                    destino,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            }
        }

        atual = atual->proximo;
        contador++;
    }
}

void liberarHistorico(Jogada **inicio) {
    Jogada *atual = *inicio;

    while (atual != NULL) {
        Jogada *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    *inicio = NULL;
}
