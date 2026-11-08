
#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include <stdbool.h>

#define TAMANHO 4

typedef struct {
    int tabuleiro[TAMANHO][TAMANHO];
    int solucao[TAMANHO][TAMANHO];
    bool fixas[TAMANHO][TAMANHO];
    int linhaSelecionada;
    int colunaSelecionada;
    bool concluido;
} Jogo;

void iniciarJogo(Jogo *jogo);
void atualizarJogo(Jogo *jogo);
void desenharJogo(Jogo *jogo);
bool verificarVitoria(Jogo *jogo);

#endif
