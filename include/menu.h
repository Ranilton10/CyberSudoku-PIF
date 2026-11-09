
#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_NOME 20

typedef enum {
    MENU,
    IDENTIFICACAO,
    PREPARACAO,
    PAUSA,
    CONFIRMAR_SAIDA,
    RANKING,
    INSTRUCOES
} Tela;

typedef struct {
    char nome[MAX_NOME + 1];
} Jogador;

bool desenharBotao(const char *texto, Rectangle botao, Color verde);
void desenharFundo(Color fundo, Color verde);
void desenharTitulo(Color verde);
void desenharMenu(Tela *telaAtual, bool *sair, Color verde);
void atualizarIdentificacao(Jogador *jogador, Tela *telaAtual);
void desenharIdentificacao(Jogador *jogador, Tela *telaAtual, Color verde);
void desenharPausa(Tela *telaAtual, Color verde);
void desenharConfirmacao(Tela *telaAtual, bool *jogoIniciado, Jogador *jogador, Color verde);

#endif
