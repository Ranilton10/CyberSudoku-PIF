
#include "jogo.h"
#include "ranking.h"
#include <stdlib.h>

#define VERDE (Color){57, 255, 100, 255}
#define VERDE_CLARO (Color){120, 255, 160, 255}
#define CINZA (Color){165, 190, 175, 255}

void carregarSimbolos(Jogo *jogo) {
    const char *arquivos[9] = {
        "assets/images/X.png",
        "assets/images/circulo.png",
        "assets/images/confere.png",
        "assets/images/coracao.png",
        "assets/images/mais.png",
        "assets/images/pentagono.png",
        "assets/images/quadrado.png",
        "assets/images/trevo.png",
        "assets/images/triangulo.png"
    };

    
    for (int i = 0; i < 9; i++) {
        jogo->simbolos[i] = LoadTexture(arquivos[i]);
    }
}

void descarregarSimbolos(Jogo *jogo) {
    for (int i = 0; i < 9; i++) {
        if (jogo->simbolos[i].id != 0) {
            UnloadTexture(jogo->simbolos[i]);
        }
    }
}

void embaralharTabuleiro(Jogo *jogo) {
    int tamanho = jogo->tamanho;
    int bloco;

    if (tamanho == 4) {
        bloco = 2;
    } else {
        bloco = 3;
    }

    int linhas[9];
    int colunas[9];
    int simbolos[9];

    for (int i = 0; i < tamanho; i++) {
        linhas[i] = i;
        colunas[i] = i;
        simbolos[i] = i + 1;
    }

    for (int i = tamanho - 1; i > 0; i--) {
        int sorteado = rand() % (i + 1);

        int auxiliar = simbolos[i];
        simbolos[i] = simbolos[sorteado];
        simbolos[sorteado] = auxiliar;
    }

    for (int inicio = 0; inicio < tamanho; inicio += bloco) {
        for (int i = bloco - 1; i > 0; i--) {
            int sorteado = rand() % (i + 1);

            int auxiliar = linhas[inicio + i];
            linhas[inicio + i] = linhas[inicio + sorteado];
            linhas[inicio + sorteado] = auxiliar;

            sorteado = rand() % (i + 1);

            auxiliar = colunas[inicio + i];
            colunas[inicio + i] = colunas[inicio + sorteado];
            colunas[inicio + sorteado] = auxiliar;
        }
    }

    for (int grupo = tamanho / bloco - 1; grupo > 0; grupo--) {
        int sorteado = rand() % (grupo + 1);

        for (int i = 0; i < bloco; i++) {
            int auxiliar = linhas[grupo * bloco + i];
            linhas[grupo * bloco + i] =
                linhas[sorteado * bloco + i];
            linhas[sorteado * bloco + i] = auxiliar;
        }

        sorteado = rand() % (grupo + 1);

        for (int i = 0; i < bloco; i++) {
            int auxiliar = colunas[grupo * bloco + i];
            colunas[grupo * bloco + i] =
                colunas[sorteado * bloco + i];
            colunas[sorteado * bloco + i] = auxiliar;
        }
    }

    int novoTabuleiro[9][9] = {0};
    int novaSolucao[9][9] = {0};

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            int valor = jogo->tabuleiro[linhas[i]][colunas[j]];
            int resposta = jogo->solucao[linhas[i]][colunas[j]];

            if (valor != 0) {
                novoTabuleiro[i][j] = simbolos[valor - 1];
            }

            novaSolucao[i][j] = simbolos[resposta - 1];
        }
    }

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            jogo->tabuleiro[i][j] = novoTabuleiro[i][j];
            jogo->solucao[i][j] = novaSolucao[i][j];
            jogo->fixas[i][j] = novoTabuleiro[i][j] != 0;
        }
    }
}

void iniciarFase(Jogo *jogo, int fase) {
    liberarHistorico(&jogo->historico);

    int inicial4[4][4] = {
        {1, 0, 3, 4},
        {0, 4, 1, 0},
        {2, 0, 4, 3},
        {4, 3, 0, 0}
    };

    int resposta4[4][4] = {
        {1, 2, 3, 4},
        {3, 4, 1, 2},
        {2, 1, 4, 3},
        {4, 3, 2, 1}
    };

    int inicial9[9][9] = {
        {5, 3, 4, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 2, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 5, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 4},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 1, 7, 9}
    };

    int resposta9[9][9] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    jogo->fase = fase;

    if (fase == 1) {
        jogo->tamanho = 4;
    } else {
        jogo->tamanho = 9;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            jogo->tabuleiro[i][j] = 0;
            jogo->solucao[i][j] = 0;
            jogo->fixas[i][j] = false;
            jogo->acertou[i][j] = false;
        }
    }

    for (int i = 0; i < jogo->tamanho; i++) {
        for (int j = 0; j < jogo->tamanho; j++) {
            int valor;
            int resposta;

            if (fase == 1) {
                valor = inicial4[i][j];
                resposta = resposta4[i][j];
            } else {
                valor = inicial9[i][j];
                resposta = resposta9[i][j];
            }

            jogo->tabuleiro[i][j] = valor;
            jogo->solucao[i][j] = resposta;
            jogo->fixas[i][j] = valor != 0;
        }
    }

    embaralharTabuleiro(jogo);

    jogo->linhaSelecionada = -1;
    jogo->colunaSelecionada = -1;
    jogo->simboloSelecionado = 1;
    jogo->concluido = false;
}

void iniciarJogo(Jogo *jogo) {
    iniciarFase(jogo, 1);
}

bool verificarConflito(Jogo *jogo, int linha, int coluna) {
    int valor = jogo->tabuleiro[linha][coluna];

    if (valor == 0) {
        return false;
    }

    for (int i = 0; i < jogo->tamanho; i++) {
        if (i != coluna && jogo->tabuleiro[linha][i] == valor) {
            return true;
        }

        if (i != linha && jogo->tabuleiro[i][coluna] == valor) {
            return true;
        }
    }

    int bloco;

    if (jogo->tamanho == 4) {
        bloco = 2;
    } else {
        bloco = 3;
    }

    int inicioLinha = (linha / bloco) * bloco;
    int inicioColuna = (coluna / bloco) * bloco;

    for (int i = inicioLinha; i < inicioLinha + bloco; i++) {
        for (int j = inicioColuna; j < inicioColuna + bloco; j++) {
            if (i != linha || j != coluna) {
                if (jogo->tabuleiro[i][j] == valor) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool verificarVitoria(Jogo *jogo) {
    for (int i = 0; i < jogo->tamanho; i++) {
        for (int j = 0; j < jogo->tamanho; j++) {
            if (jogo->tabuleiro[i][j] != jogo->solucao[i][j]) {
                return false;
            }
        }
    }

    return true;
}

void desenharSimbolo(Texture2D imagem, Rectangle destino, Color cor) {
    if (imagem.id == 0) {
        return;
    }

    Rectangle origem = {
        0,
        0,
        (float)imagem.width,
        (float)imagem.height
    };

    DrawTexturePro(
        imagem,
        origem,
        destino,
        (Vector2){0, 0},
        0,
        cor
    );
}

/* Pontua uma jogada: compara o simbolo colocado com a solucao.
   Acerto: +100, uma unica vez por celula (impede apagar e repor para farmar pontos).
   Erro: perde metade dos pontos atuais. */
static void pontuarJogada(Jogo *jogo, int linha, int coluna) {
    int valor = jogo->tabuleiro[linha][coluna];

    if (valor == 0) {
        return;
    }

    if (valor == jogo->solucao[linha][coluna]) {
        if (!jogo->acertou[linha][coluna]) {
            jogo->acertou[linha][coluna] = true;
            pontuacao_acerto(&jogo->pontos);
        }
    } else {
        pontuacao_erro(&jogo->pontos);
    }
}

void atualizarJogo(Jogo *jogo) {
    if (jogo->concluido) {
        return;
    }

    int celula;
    int x;
    int y;
    int painelX;
    int larguraBotao;
    int espacoBotao;
    int yBotoes;

    if (jogo->tamanho == 4) {
        celula = 88;
        y = 210;
        painelX = 324;
        larguraBotao = 78;
        espacoBotao = 90;
        yBotoes = 605;
    } else {
        celula = 42;
        y = 190;
        painelX = 282;
        larguraBotao = 43;
        espacoBotao = 49;
        yBotoes = 625;
    }

    x = (1000 - jogo->tamanho * celula) / 2;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();

        for (int i = 0; i < jogo->tamanho; i++) {
            Rectangle botao = {
                painelX + i * espacoBotao,
                yBotoes,
                larguraBotao,
                45
            };

            if (CheckCollisionPointRec(mouse, botao)) {
                jogo->simboloSelecionado = i + 1;
            }
        }

        if (mouse.x >= x &&
            mouse.x < x + jogo->tamanho * celula &&
            mouse.y >= y &&
            mouse.y < y + jogo->tamanho * celula) {

            int coluna = (int)(mouse.x - x) / celula;
            int linha = (int)(mouse.y - y) / celula;

            if (!jogo->fixas[linha][coluna]) {
                int valorAnterior = jogo->tabuleiro[linha][coluna];

                if (jogo->linhaSelecionada == linha &&
                    jogo->colunaSelecionada == coluna &&
                    valorAnterior != 0) {

                    jogo->tabuleiro[linha][coluna] = 0;
                } else {
                    jogo->tabuleiro[linha][coluna] =
                        jogo->simboloSelecionado;
                }

                if (valorAnterior != jogo->tabuleiro[linha][coluna]) {
                    adicionarJogada(
                        &jogo->historico,
                        linha,
                        coluna,
                        jogo->tabuleiro[linha][coluna],
                        jogo->tabuleiro[linha][coluna] == 0
                    );

                    pontuarJogada(jogo, linha, coluna);
                }

                jogo->linhaSelecionada = linha;
                jogo->colunaSelecionada = coluna;
            }
        }
    }

    for (int numero = 1; numero <= jogo->tamanho; numero++) {
        if (IsKeyPressed(KEY_ONE + numero - 1) ||
            IsKeyPressed(KEY_KP_1 + numero - 1)) {

            jogo->simboloSelecionado = numero;

            int linha = jogo->linhaSelecionada;
            int coluna = jogo->colunaSelecionada;

            if (linha >= 0 && coluna >= 0) {
                if (!jogo->fixas[linha][coluna]) {
                    if (jogo->tabuleiro[linha][coluna] != numero) {
                        jogo->tabuleiro[linha][coluna] = numero;

                        adicionarJogada(
                            &jogo->historico,
                            linha,
                            coluna,
                            numero,
                            0
                        );

                        pontuarJogada(jogo, linha, coluna);
                    }
                }
            }
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE) ||
        IsKeyPressed(KEY_DELETE)) {

        int linha = jogo->linhaSelecionada;
        int coluna = jogo->colunaSelecionada;

        if (linha >= 0 && coluna >= 0) {
            if (!jogo->fixas[linha][coluna]) {
                if (jogo->tabuleiro[linha][coluna] != 0) {
                    jogo->tabuleiro[linha][coluna] = 0;

                    adicionarJogada(
                        &jogo->historico,
                        linha,
                        coluna,
                        0,
                        1
                    );
                }
            }
        }
    }

    if (verificarVitoria(jogo)) {
        jogo->concluido = true;
        pontuacao_fase(&jogo->pontos);
    }
}

void desenharJogo(Jogo *jogo) {
    int celula;
    int x;
    int y;
    int bloco;
    int painelX;
    int larguraBotao;
    int espacoBotao;
    int yBotoes;
    int yMensagem;

    if (jogo->tamanho == 4) {
        celula = 88;
        y = 210;
        bloco = 2;
        painelX = 324;
        larguraBotao = 78;
        espacoBotao = 90;
        yBotoes = 605;
        yMensagem = 575;
    } else {
        celula = 42;
        y = 190;
        bloco = 3;
        painelX = 282;
        larguraBotao = 43;
        espacoBotao = 49;
        yBotoes = 625;
        yMensagem = 592;
    }

    x = (1000 - jogo->tamanho * celula) / 2;

    const char *nomes[3] = {
        "",
        "ACESSO INICIAL",
        "QUEBRA DE FIREWALL"
    };

    DrawText(
        TextFormat("FASE %02d / %02d", jogo->fase, TOTAL_FASES),
        300, 75, 19, VERDE_CLARO
    );

    DrawText(nomes[jogo->fase], 300, 110, 29, VERDE);

    DrawText("P  PAUSAR", 770, 105, 18, VERDE_CLARO);

    int largura = jogo->tamanho * celula;

    Rectangle painel = {
        x - 10,
        y - 10,
        largura + 20,
        largura + 20
    };

    DrawRectangleRounded(
        painel,
        0.025f,
        8,
        (Color){9, 22, 16, 255}
    );

    DrawRectangleRoundedLinesEx(
        painel,
        0.025f,
        8,
        2,
        Fade(VERDE, 0.65f)
    );

    bool existeConflito = false;

    for (int i = 0; i < jogo->tamanho; i++) {
        for (int j = 0; j < jogo->tamanho; j++) {
            Rectangle area = {
                x + j * celula,
                y + i * celula,
                celula,
                celula
            };

            Color cor = {13, 30, 22, 255};

            if (jogo->linhaSelecionada == i &&
                jogo->colunaSelecionada == j) {
                cor = (Color){30, 78, 47, 255};
            }

            if (verificarConflito(jogo, i, j)) {
                cor = (Color){115, 32, 38, 255};
                existeConflito = true;
            }

            DrawRectangleRec(area, cor);
            DrawRectangleLinesEx(area, 1, Fade(VERDE, 0.45f));

            int valor = jogo->tabuleiro[i][j];

            if (valor > 0) {
                int margem;

                if (jogo->tamanho == 4) {
                    margem = 19;
                } else {
                    margem = 8;
                }

                int lado = celula - 2 * margem;

                Rectangle destino = {
                    area.x + margem,
                    area.y + margem,
                    lado,
                    lado
                };

                Color corSimbolo = VERDE_CLARO;

                if (jogo->fixas[i][j]) {
                    corSimbolo = WHITE;
                }

                desenharSimbolo(
                    jogo->simbolos[valor - 1],
                    destino,
                    corSimbolo
                );
            }

            if (jogo->fixas[i][j]) {
                int raio = 2;

                if (jogo->tamanho == 4) {
                    raio = 3;
                }

                DrawCircle(
                    area.x + 6,
                    area.y + 6,
                    raio,
                    Fade(VERDE, 0.7f)
                );
            }
        }
    }

    for (int i = 0; i <= jogo->tamanho; i += bloco) {
        DrawRectangle(
            x,
            y + i * celula - 2,
            largura,
            4,
            VERDE
        );

        DrawRectangle(
            x + i * celula - 2,
            y,
            4,
            largura,
            VERDE
        );
    }

    for (int i = 0; i < jogo->tamanho; i++) {
        const char *textoLinha = TextFormat("L%d", i + 1);
        int larguraLinha = MeasureText(textoLinha, 12);

        DrawText(
            textoLinha,
            x - larguraLinha - 15,
            y + i * celula + (celula - 12) / 2,
            12,
            VERDE_CLARO
        );
    }

    for (int j = 0; j < jogo->tamanho; j++) {
        const char *textoColuna = TextFormat("C%d", j + 1);
        int larguraColuna = MeasureText(textoColuna, 12);

        DrawText(
            textoColuna,
            x + j * celula + (celula - larguraColuna) / 2,
            y - 27,
            12,
            VERDE_CLARO
        );
    }

    if (existeConflito) {
        DrawText("CONFLITO DETECTADO", 300, yMensagem, 18, RED);
    } else if (jogo->concluido) {
        DrawText("SISTEMA INVADIDO!", 300, yMensagem, 18, VERDE);
    } else {
        DrawText("SELECIONE UM SIMBOLO", 300, yMensagem, 18, VERDE_CLARO);
    }

    for (int i = 0; i < jogo->tamanho; i++) {
        Rectangle botao = {
            painelX + i * espacoBotao,
            yBotoes,
            larguraBotao,
            45
        };

        bool selecionado = jogo->simboloSelecionado == i + 1;

        Color fundo = {14, 34, 24, 255};
        Color borda = Fade(VERDE, 0.5f);
        float espessura = 1.5f;

        if (selecionado) {
            fundo = (Color){32, 85, 49, 255};
            borda = VERDE_CLARO;
            espessura = 3;
        }

        DrawRectangleRounded(botao, 0.12f, 8, fundo);

        DrawRectangleRoundedLinesEx(
            botao,
            0.12f,
            8,
            espessura,
            borda
        );

        int margem;

        if (jogo->tamanho == 4) {
            margem = 19;
        } else {
            margem = 10;
        }

        int lado = larguraBotao - 2 * margem;

        Rectangle destino = {
            botao.x + margem,
            botao.y + (45 - lado) / 2,
            lado,
            lado
        };

        desenharSimbolo(
            jogo->simbolos[i],
            destino,
            WHITE
        );
    }

    const char *aviso = "PONTO VERDE = CELULA FIXA";
    int larguraAviso = MeasureText(aviso, 16);

    DrawText(
        aviso,
        (1000 - larguraAviso) / 2,
        675,
        16,
        CINZA
    );

    desenharHistorico(jogo->historico, jogo->simbolos);
}
