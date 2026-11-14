
#include "menu.h"
#include "ranking.h"
#include <string.h>

#define LARGURA 1000
#define ALTURA 700

static const Color VERDE_CLARO = {110, 255, 155, 255};
static const Color FUNDO_PAINEL = {12, 24, 20, 255};
static const Color FUNDO_BOTAO = {19, 43, 32, 255};
static const Color CINZA_TEXTO = {170, 190, 180, 255};

static Ranking rankingTela;

static void textoCentralizado(const char *texto, int y, int tamanho, Color cor) {
    int largura = MeasureText(texto, tamanho);
    DrawText(texto, (LARGURA - largura) / 2, y, tamanho, cor);
}


static void desenharPainel(Rectangle area, Color verde) {
    DrawRectangleRounded(area, 0.035f, 8, FUNDO_PAINEL);
    DrawRectangleRoundedLinesEx(area, 0.035f, 8, 1.5f, Fade(verde, 0.55f));

    DrawRectangle(
        (int)area.x + 20,
        (int)area.y,
        65,
        3,
        verde
    );
}

static void desenharSecao(const char *titulo, const char *linha1, const char *linha2, int y, Color verde) {
    DrawRectangle(175, y + 3, 4, 23, verde);
    DrawText(titulo, 192, y, 22, VERDE_CLARO);
    DrawText(linha1, 192, y + 36, 18, RAYWHITE);

    if (linha2 != NULL) {
        DrawText(linha2, 192, y + 61, 18, CINZA_TEXTO);
    }
}

bool desenharBotao(const char *texto, Rectangle botao, Color verde) {
    Vector2 mouse = GetMousePosition();
    bool mouseSobre = CheckCollisionPointRec(mouse, botao);

    Color corFundo = mouseSobre ? (Color){32, 100, 61, 255} : FUNDO_BOTAO;
    Color corBorda = mouseSobre ? VERDE_CLARO : Fade(verde, 0.65f);

    DrawRectangleRounded(botao, 0.13f, 8, corFundo);
    DrawRectangleRoundedLinesEx(botao, 0.13f, 8, 2, corBorda);

    int fonte = 22;
    int larguraTexto = MeasureText(texto, fonte);

    DrawText(
        texto,
        (int)(botao.x + (botao.width - larguraTexto) / 2),
        (int)(botao.y + (botao.height - fonte) / 2),
        fonte,
        mouseSobre ? WHITE : VERDE_CLARO
    );

    return mouseSobre && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void desenharFundo(Color fundo, Color verde) {
    ClearBackground(fundo);

    for (int x = 0; x < LARGURA; x += 50) {
        DrawLine(x, 0, x, ALTURA, Fade(verde, 0.055f));
    }

    for (int y = 0; y < ALTURA; y += 50) {
        DrawLine(0, y, LARGURA, y, Fade(verde, 0.055f));
    }

    DrawRectangle(0, 0, LARGURA, 5, verde);
    DrawRectangle(0, ALTURA - 5, LARGURA, 5, Fade(verde, 0.65f));

    DrawText("CYBER SYSTEM // ONLINE", 28, 25, 15, Fade(verde, 0.65f));
    
}

void desenharTitulo(Color verde) {
    textoCentralizado("CYBER SUDOKU", 75, 48, verde);
    textoCentralizado("SISTEMA DE INVASAO", 135, 19, CINZA_TEXTO);

    DrawLine(360, 175, 640, 175, Fade(verde, 0.55f));
}

void desenharMenu(Tela *telaAtual, bool *sair, Color verde) {
    if (*telaAtual == MENU) {
        Rectangle painel = {295, 210, 410, 390};
        desenharPainel(painel, verde);

        textoCentralizado("TERMINAL DE ACESSO", 235, 23, VERDE_CLARO);
        textoCentralizado("Escolha uma operacao", 271, 17, CINZA_TEXTO);

        const char *opcoes[] = {
            "INICIAR INVASAO",
            "COMO JOGAR",
            "RANKING",
            "SAIR"
        };

        for (int i = 0; i < 4; i++) {
            Rectangle botao = {
                345,
                (float)(315 + i * 66),
                310,
                50
            };

            if (desenharBotao(opcoes[i], botao, verde)) {
                if (i == 0) {
                    *telaAtual = IDENTIFICACAO;
                } else if (i == 1) {
                    *telaAtual = INSTRUCOES;
                } else if (i == 2) {
                    ranking_carregar(&rankingTela);
                    *telaAtual = RANKING;
                } else {
                    *sair = true;
                }
            }
        }

        textoCentralizado("PRESSIONE OS BOTOES PARA NAVEGAR", 632, 16, CINZA_TEXTO);
    }

    else if (*telaAtual == INSTRUCOES) {
        Rectangle painel = {120, 195, 760, 425};
        desenharPainel(painel, verde);

        textoCentralizado("MANUAL DE INVASAO", 215, 27, verde);

        desenharSecao(
            "OBJETIVO",
            "Preencha todas as celulas sem repetir simbolos.",
            "Cada linha, coluna e bloco deve ter simbolos diferentes.",
            260,
            verde
        );

        desenharSecao(
            "CONTROLES",
            "Escolha um simbolo no painel e clique em uma celula.",
            "Use as teclas numericas ou BACKSPACE para editar.",
            355,
            verde
        );

        desenharSecao(
            "CAMPANHA",
            "Fase 1: tabuleiro 4x4. Fases 2 e 3: tabuleiro 9x9.",
            "Complete as tres fases para registrar sua pontuacao.",
            450,
            verde
        );

        DrawText("P = PAUSAR / RETOMAR", 192, 558, 19, VERDE_CLARO);

        Rectangle voltar = {390, 635, 220, 45};

        if (desenharBotao("VOLTAR", voltar, verde)) {
            *telaAtual = MENU;
        }
    }

    else if (*telaAtual == RANKING) {
        Rectangle painel = {260, 220, 480, 355};
        desenharPainel(painel, verde);

        textoCentralizado("RANKING DE HACKERS", 255, 27, verde);
        textoCentralizado("REGISTROS DE INVASAO", 300, 17, CINZA_TEXTO);

        if (rankingTela.quantidade == 0) {
            textoCentralizado("Nenhuma campanha concluida.", 395, 21, RAYWHITE);
            textoCentralizado("Os melhores hackers aparecerao aqui.", 430, 17, CINZA_TEXTO);
        }

        for (int i = 0; i < rankingTela.quantidade; i++) {
            int y = 330 + i * 23;
            Color cor = (i == 0) ? GOLD : RAYWHITE;

            DrawText(TextFormat("%02d", i + 1), 300, y, 18, VERDE_CLARO);
            DrawText(rankingTela.entradas[i].nome, 350, y, 18, cor);

            const char *pontos = TextFormat("%d", rankingTela.entradas[i].pontos);
            DrawText(pontos, 700 - MeasureText(pontos, 18), y, 18, cor);
        }

        Rectangle voltar = {390, 610, 220, 50};

        if (desenharBotao("VOLTAR", voltar, verde)) {
            *telaAtual = MENU;
        }
    }
}

void atualizarIdentificacao(Jogador *jogador, Tela *telaAtual) {
    int tamanho = (int)strlen(jogador->nome);
    int tecla = GetCharPressed();

    while (tecla > 0) {
        if (tecla >= 32 && tecla <= 126 && tamanho < MAX_NOME) {
            jogador->nome[tamanho] = (char)tecla;
            tamanho++;
            jogador->nome[tamanho] = '\0';
        }

        tecla = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && tamanho > 0) {
        jogador->nome[tamanho - 1] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER) && strlen(jogador->nome) > 0) {
        *telaAtual = PREPARACAO;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        jogador->nome[0] = '\0';
        *telaAtual = MENU;
    }
}

void desenharIdentificacao(Jogador *jogador, Tela *telaAtual, Color verde) {
    Rectangle painel = {260, 220, 480, 375};
    desenharPainel(painel, verde);

    textoCentralizado("IDENTIFICACAO", 250, 30, verde);
    textoCentralizado("Informe seu codinome de hacker", 300, 19, CINZA_TEXTO);

    Rectangle campo = {310, 365, 380, 65};

    DrawRectangleRounded(campo, 0.12f, 8, (Color){8, 18, 14, 255});
    DrawRectangleRoundedLinesEx(campo, 0.12f, 8, 2, verde);

    int tamanho = (int)strlen(jogador->nome);
    int larguraTexto = MeasureText(jogador->nome, 25);

    DrawText(jogador->nome, 330, 385, 25, RAYWHITE);

    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawRectangle(333 + larguraTexto, 383, 2, 30, verde);
    }

    DrawText(
        TextFormat("%d/%d", tamanho, MAX_NOME),
        625, 445, 17, CINZA_TEXTO
    );

    textoCentralizado("ENTER PARA INICIAR A INVASAO", 480, 18, VERDE_CLARO);

    Rectangle voltar = {385, 530, 230, 48};

    if (desenharBotao("VOLTAR", voltar, verde)) {
        jogador->nome[0] = '\0';
        *telaAtual = MENU;
    }
}

void desenharPausa(Tela *telaAtual, Color verde) {
    Rectangle painel = {285, 220, 430, 350};
    desenharPainel(painel, verde);

    textoCentralizado("SISTEMA PAUSADO", 250, 30, verde);
    textoCentralizado("Sua partida esta protegida.", 305, 19, CINZA_TEXTO);

    Rectangle retomar = {340, 365, 320, 55};
    Rectangle sair = {340, 445, 320, 55};

    if (desenharBotao("RETOMAR PARTIDA", retomar, verde)) {
        *telaAtual = PREPARACAO;
    }

    if (desenharBotao("SAIR DA PARTIDA", sair, verde)) {
        *telaAtual = CONFIRMAR_SAIDA;
    }

    textoCentralizado("PRESSIONE P PARA RETOMAR", 535, 17, CINZA_TEXTO);
}

void desenharConfirmacao(Tela *telaAtual, bool *jogoIniciado, Jogador *jogador, Color verde) {
    Rectangle painel = {275, 220, 450, 350};
    desenharPainel(painel, verde);

    textoCentralizado("ENCERRAR INVASAO?", 255, 29, verde);
    textoCentralizado("Todo o progresso sera perdido.", 315, 19, CINZA_TEXTO);

    Rectangle confirmar = {340, 375, 320, 55};
    Rectangle cancelar = {340, 455, 320, 55};

    if (desenharBotao("SIM, SAIR", confirmar, verde)) {
        *telaAtual = MENU;
        *jogoIniciado = false;
        jogador->nome[0] = '\0';
    }

    if (desenharBotao("CANCELAR", cancelar, verde)) {
        *telaAtual = PAUSA;
    }
}
