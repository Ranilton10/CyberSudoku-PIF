
#include "raylib.h"
#include "menu.h"
#include "jogo.h"
#include <stdbool.h>
#include <time.h>

#define LARGURA 1000
#define ALTURA 700

int main(void) {
    InitWindow(LARGURA, ALTURA, "Cyber Sudoku");

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Color fundo = {8, 12, 10, 255};
    Color verde = {57, 255, 100, 255};

    Tela telaAtual = MENU;
    bool sair = false;
    bool jogoIniciado = false;
    bool telaCheia = false;

    Jogador jogador = {0};
    Jogo jogo = {0};

    RenderTexture2D telaJogo = LoadRenderTexture(LARGURA, ALTURA);

    carregarSimbolos(&jogo);

    while (!WindowShouldClose() && !sair) {

        if (IsKeyPressed(KEY_F11)) {
            if (telaCheia) {
                ToggleFullscreen();
                SetWindowSize(LARGURA, ALTURA);
                telaCheia = false;
            } else {
                int monitor = GetCurrentMonitor();

                int larguraMonitor = GetMonitorWidth(monitor);
                int alturaMonitor = GetMonitorHeight(monitor);

                SetWindowSize(larguraMonitor, alturaMonitor);
                ToggleFullscreen();

                telaCheia = true;
            }
        }

        int larguraAtual = GetScreenWidth();
        int alturaAtual = GetScreenHeight();

        float escalaX = (float)larguraAtual / LARGURA;
        float escalaY = (float)alturaAtual / ALTURA;

        float escala = escalaX;

        if (escalaY < escalaX) {
            escala = escalaY;
        }

        float larguraDesenho = LARGURA * escala;
        float alturaDesenho = ALTURA * escala;

        float posicaoX = (larguraAtual - larguraDesenho) / 2;
        float posicaoY = (alturaAtual - alturaDesenho) / 2;

        Vector2 mouse = GetMousePosition();

        Vector2 mouseAdaptado = {
            (mouse.x - posicaoX) / escala,
            (mouse.y - posicaoY) / escala
        };

        SetMouseOffset(-posicaoX, -posicaoY);
        SetMouseScale(1.0f / escala, 1.0f / escala);

        if (telaAtual == IDENTIFICACAO) {
            atualizarIdentificacao(&jogador, &telaAtual);
        }

        if (telaAtual == PREPARACAO) {
            if (!jogoIniciado) {
                iniciarJogo(&jogo);
                jogoIniciado = true;
                time_t inicio = time(NULL);
            }

            if (!jogo.concluido) {
                if (IsKeyPressed(KEY_P)) {
                    telaAtual = PAUSA;
                    //Pausa Timer
                    atual += difftime(time(NULL),inicio);
                } else {
                    atualizarJogo(&jogo);
                    //Atualiza Timer
                    time_t atual = time(NULL);
                    int minutos = (int)difftime(atual, inicio) / 60;
                    int segundos = (int)difftime(atual, inicio) % 60;
                    char texto_timer[32];
                    sprintf(texto_timer, "Tempo: %02d:%02d",minutos, segundos);
                }
            }
        } else if (telaAtual == PAUSA) {
            if (IsKeyPressed(KEY_P)) {
                telaAtual = PREPARACAO;                
            }
        } else if (telaAtual == CONFIRMAR_SAIDA) {
            if (IsKeyPressed(KEY_P)) {
                telaAtual = PAUSA;
            }
        }

        BeginTextureMode(telaJogo);

        desenharFundo(fundo, verde);

        if (telaAtual == PREPARACAO) {
            desenharJogo(&jogo);

            if (jogo.concluido) {
                DrawRectangle(
                    0, 0, LARGURA, ALTURA,
                    (Color){0, 0, 0, 205}
                );

                Rectangle painel = {270, 225, 460, 270};

                DrawRectangleRounded(
                    painel,
                    0.08f,
                    8,
                    (Color){12, 30, 21, 255}
                );

                DrawRectangleRoundedLinesEx(
                    painel,
                    0.08f,
                    8,
                    2,
                    verde
                );

                const char *titulo;

                if (jogo.fase == TOTAL_FASES) {
                    titulo = "CAMPANHA CONCLUIDA!";
                } else {
                    titulo = "ACESSO LIBERADO!";
                }

                int larguraTitulo = MeasureText(titulo, 28);

                DrawText(
                    titulo,
                    (LARGURA - larguraTitulo) / 2,
                    265,
                    28,
                    verde
                );

                const char *mensagem;

                if (jogo.fase == TOTAL_FASES) {
                    mensagem = "Todos os sistemas foram invadidos.";
                } else {
                    mensagem = "Sistema invadido com sucesso.";
                }

                int larguraMensagem = MeasureText(mensagem, 19);

                DrawText(
                    mensagem,
                    (LARGURA - larguraMensagem) / 2,
                    330,
                    19,
                    RAYWHITE
                );

                Rectangle botao = {350, 395, 300, 60};

                const char *textoBotao;

                if (jogo.fase == TOTAL_FASES) {
                    textoBotao = "VOLTAR AO MENU";
                } else {
                    textoBotao = "PROXIMA FASE";
                }

                if (desenharBotao(textoBotao, botao, verde)) {
                    if (jogo.fase < TOTAL_FASES) {
                        iniciarFase(&jogo, jogo.fase + 1);
                    } else {
                        telaAtual = MENU;
                        jogoIniciado = false;
                        jogador.nome[0] = '\0';
                    }
                }
            }

        } else if (telaAtual == IDENTIFICACAO) {
            desenharTitulo(verde);
            desenharIdentificacao(&jogador, &telaAtual, verde);

        } else if (telaAtual == PAUSA) {
            desenharTitulo(verde);
            desenharPausa(&telaAtual, verde);

        } else if (telaAtual == CONFIRMAR_SAIDA) {
            desenharTitulo(verde);
            desenharConfirmacao(
                &telaAtual,
                &jogoIniciado,
                &jogador,
                verde
            );

        } else {
            desenharTitulo(verde);
            desenharMenu(&telaAtual, &sair, verde);
        }

        EndTextureMode();

        BeginDrawing();

        ClearBackground(BLACK);

        Rectangle origem = {
            0,
            0,
            LARGURA,
            -ALTURA
        };

        Rectangle destino = {
            posicaoX,
            posicaoY,
            larguraDesenho,
            alturaDesenho
        };

        DrawTexturePro(
            telaJogo.texture,
            origem,
            destino,
            (Vector2){0, 0},
            0,
            WHITE
        );

        EndDrawing();
    }

    descarregarSimbolos(&jogo);
    UnloadRenderTexture(telaJogo);

    CloseWindow();

    return 0;
}
