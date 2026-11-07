
#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    MENU,
    PREPARACAO,
    RANKING,
    INSTRUCOES
} Tela;

bool desenharBotao(const char *texto, Rectangle botao, Color verde);
void desenharFundo(Color fundo, Color verde);
void desenharTitulo(Color verde);
void desenharMenu(Tela *telaAtual, bool *sair, Color verde);

#endif
