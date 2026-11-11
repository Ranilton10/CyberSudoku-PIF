
#include "raylib.h"
#include "menu.h"
#include "jogo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LARGURA 1000
#define ALTURA 700

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA, ALTURA, "Cyber Sudoku");

    InitAudioDevice();

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Color fundo = {8, 12, 10, 255};
    Color verde = {57, 255, 100, 255};

    Tela telaAtual = MENU;
    bool sair = false;
    bool jogoIniciado = false;
    bool telaCheia = false;

    bool emTransicao = false;
    float tempoTransicao = 0.0f;
    int proximaFase = 1;

    float tempoTotal = 0.0f;
    int pontuacaoTotal = 0;

    Music musica = LoadMusicStream("assets/sounds/loaben.mp3");
    bool musicaCarregada = IsMusicValid(musica);

    float volumeAtual = 0.6f;
    float volumeDesejado = 0.6f;
    float velocidadeVolume = 0.8f;

    if (musicaCarregada) {
        musica.looping = true;
        SetMusicVolume(musica, volumeAtual);
        PlayMusicStream(musica);
    }

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

        SetMouseOffset(-posicaoX, -posicaoY);
        SetMouseScale(1.0f / escala, 1.0f / escala);

        if (telaAtual == IDENTIFICACAO) {
            atualizarIdentificacao(&jogador, &telaAtual);
        }

        if (telaAtual == PREPARACAO && !jogoIniciado) {
            jogoIniciado = true;
            tempoTotal = 0.0f;
            pontuacaoTotal = 0;
            proximaFase = 1;
            tempoTransicao = 0.0f;
            emTransicao = true;
        }

        if (emTransicao) {
            tempoTransicao += GetFrameTime();

            if (tempoTransicao >= 2.0f) {
                iniciarFase(&jogo, proximaFase);
                emTransicao = false;
                tempoTransicao = 0.0f;
            }
        }

        if (telaAtual == PREPARACAO && !emTransicao) {
            if (!jogo.concluido) {
                if (IsKeyPressed(KEY_P)) {
                    telaAtual = PAUSA;
                } else {
                    atualizarJogo(&jogo);

                    if (!jogo.concluido) {
                        tempoTotal += GetFrameTime();
                    }
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

        if (emTransicao) {
            volumeDesejado = 0.2f;
        } else if (telaAtual == PAUSA ||
                   telaAtual == CONFIRMAR_SAIDA) {
            volumeDesejado = 0.3f;
        } else if (telaAtual == PREPARACAO && jogo.concluido) {
            volumeDesejado = 0.35f;
        } else {
            volumeDesejado = 0.6f;
        }

        float diferenca = volumeDesejado - volumeAtual;
        float passo = velocidadeVolume * GetFrameTime();

        if (diferenca > passo) {
            volumeAtual += passo;
        } else if (diferenca < -passo) {
            volumeAtual -= passo;
        } else {
            volumeAtual = volumeDesejado;
        }

        if (musicaCarregada) {
            UpdateMusicStream(musica);
            SetMusicVolume(musica, volumeAtual);
        }

        int tempoSegundos = (int)tempoTotal;
        int minutos = tempoSegundos / 60;
        int segundos = tempoSegundos % 60;

        BeginTextureMode(telaJogo);

        desenharFundo(fundo, verde);

        if (emTransicao) {
            const char *nomeFase;

            if (proximaFase == 1) {
                nomeFase = "ACESSO INICIAL";
            } else {
                nomeFase = "QUEBRA DE FIREWALL";
            }

            const char *cabecalho = "CYBER SYSTEM // INICIALIZANDO";

            DrawText(
                cabecalho,
                (LARGURA - MeasureText(cabecalho, 19)) / 2,
                225,
                19,
                verde
            );

            const char *numeroFase =
                TextFormat("FASE %02d", proximaFase);

            DrawText(
                numeroFase,
                (LARGURA - MeasureText(numeroFase, 52)) / 2,
                285,
                52,
                verde
            );

            DrawText(
                nomeFase,
                (LARGURA - MeasureText(nomeFase, 25)) / 2,
                365,
                25,
                RAYWHITE
            );

            Rectangle barraFundo = {300, 435, 400, 12};

            DrawRectangleRounded(
                barraFundo,
                0.5f,
                8,
                (Color){20, 55, 32, 255}
            );

            float progresso = tempoTransicao / 2.0f;

            if (progresso > 1.0f) {
                progresso = 1.0f;
            }

            if (progresso > 0.0f) {
                Rectangle barraProgresso = {
                    300,
                    435,
                    400 * progresso,
                    12
                };

                DrawRectangleRounded(
                    barraProgresso,
                    0.5f,
                    8,
                    verde
                );
            }

            const char *mensagem = "CARREGANDO SISTEMA...";

            DrawText(
                mensagem,
                (LARGURA - MeasureText(mensagem, 18)) / 2,
                470,
                18,
                verde
            );

        } else if (telaAtual == PREPARACAO) {
            desenharJogo(&jogo);

            DrawText(
                TextFormat("TEMPO: %02d:%02d", minutos, segundos),
                770, 75, 20, verde
            );

            if (jogo.concluido) {
                DrawRectangle(
                    0, 0, LARGURA, ALTURA,
                    (Color){0, 0, 0, 215}
                );

                if (jogo.fase == TOTAL_FASES) {
                    Rectangle painel = {240, 135, 520, 440};

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

                    const char *titulo = "INVASAO CONCLUIDA!";

                    DrawText(
                        titulo,
                        (LARGURA - MeasureText(titulo, 29)) / 2,
                        170,
                        29,
                        verde
                    );

                    DrawText(
                        jogador.nome,
                        (LARGURA - MeasureText(jogador.nome, 21)) / 2,
                        220,
                        21,
                        RAYWHITE
                    );

                    const char *subtitulo = "RELATORIO FINAL";

                    DrawText(
                        subtitulo,
                        (LARGURA - MeasureText(subtitulo, 21)) / 2,
                        260,
                        21,
                        verde
                    );

                    DrawLine(
                        275, 300, 725, 300,
                        Fade(verde, 0.5f)
                    );

                    DrawText(
                        "TEMPO TOTAL",
                        295, 330, 18, RAYWHITE
                    );

                    DrawText(
                        TextFormat("%02d:%02d", minutos, segundos),
                        565, 330, 23, verde
                    );

                    DrawText(
                        "PONTUACAO",
                        295, 385, 18, RAYWHITE
                    );

                    DrawText(
                        TextFormat("%d", pontuacaoTotal),
                        565, 385, 23, verde
                    );

                    const char *fases = "FASES CONCLUIDAS: 2 / 2";

                    DrawText(
                        fases,
                        (LARGURA - MeasureText(fases, 19)) / 2,
                        440,
                        19,
                        verde
                    );

                    Rectangle botao = {350, 490, 300, 55};

                    if (desenharBotao("VOLTAR AO MENU", botao, verde)) {
                        telaAtual = MENU;
                        jogoIniciado = false;
                        jogador.nome[0] = '\0';
                    }

                } else {
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

                    const char *titulo = "ACESSO LIBERADO!";

                    DrawText(
                        titulo,
                        (LARGURA - MeasureText(titulo, 28)) / 2,
                        265,
                        28,
                        verde
                    );

                    const char *mensagem =
                        "Sistema invadido com sucesso.";

                    DrawText(
                        mensagem,
                        (LARGURA - MeasureText(mensagem, 19)) / 2,
                        330,
                        19,
                        RAYWHITE
                    );

                    Rectangle botao = {350, 395, 300, 60};

                    if (desenharBotao("PROXIMA FASE", botao, verde)) {
                        proximaFase = jogo.fase + 1;
                        tempoTransicao = 0.0f;
                        emTransicao = true;
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

    liberarHistorico(&jogo.historico);
    descarregarSimbolos(&jogo);
    UnloadRenderTexture(telaJogo);

    if (musicaCarregada) {
        StopMusicStream(musica);
        UnloadMusicStream(musica);
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
