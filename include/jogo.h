
#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include "historico.h"
#include <stdbool.h>

#define TAMANHO 9
#define TOTAL_SIMBOLOS 9
#define TOTAL_FASES 2

typedef struct {
    int tabuleiro[TAMANHO][TAMANHO];
    int solucao[TAMANHO][TAMANHO];
    bool fixas[TAMANHO][TAMANHO];
    int fase;
    int tamanho;
    int linhaSelecionada;
    int colunaSelecionada;
    int simboloSelecionado;
    bool concluido;
    int pontos;
    bool acertou[TAMANHO][TAMANHO];
    Texture2D simbolos[TOTAL_SIMBOLOS];
    Jogada *historico;
} Jogo;

void carregarSimbolos(Jogo *jogo);
void descarregarSimbolos(Jogo *jogo);
void iniciarJogo(Jogo *jogo);
void iniciarFase(Jogo *jogo, int fase);
void atualizarJogo(Jogo *jogo);
void desenharJogo(Jogo *jogo);
bool verificarConflito(Jogo *jogo, int linha, int coluna);
bool verificarVitoria(Jogo *jogo);

#endif
