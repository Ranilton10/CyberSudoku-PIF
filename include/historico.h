
#ifndef HISTORICO_H
#define HISTORICO_H

#include "raylib.h"

typedef struct Jogada {
    int linha;
    int coluna;
    int simbolo;
    int apagou;
    struct Jogada *proximo;
} Jogada;

void adicionarJogada(Jogada **inicio, int linha, int coluna, int simbolo, int apagou);
void desenharHistorico(Jogada *inicio, Texture2D simbolos[]);
void liberarHistorico(Jogada **inicio);

#endif
