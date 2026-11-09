
#include "jogo.h"

#define VERDE (Color){57, 255, 100, 255}
#define VERDE_CLARO (Color){120, 255, 160, 255}
#define CINZA (Color){165, 190, 175, 255}

void carregarSimbolos(Jogo *jogo) {
    const char *arquivos[9] = {
        "assets/images/X.png",
        "assets/images/circulo.png",
        "assets/images/confere.png",
        "assets/images/coração.png",
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

void iniciarFase(Jogo *jogo, int fase) {
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
        }
    }

    for (int i = 0; i < jogo->tamanho; i++) {
        for (int j = 0; j < jogo->tamanho; j++) {

            int valor = 0;
            int resposta = 0;

            if (fase == 1) {
                valor = inicial4[i][j];
                resposta = resposta4[i][j];
            } else if (fase == 2) {
                valor = inicial9[i][j];
                resposta = resposta9[i][j];
            } else {
                valor = inicial9[j][i];
                resposta = resposta9[j][i];

                if (valor != 0) {
                    valor = valor % 9 + 1;
                }

                resposta = resposta % 9 + 1;
            }

            jogo->tabuleiro[i][j] = valor;
            jogo->solucao[i][j] = resposta;

            if (valor != 0) {
                jogo->fixas[i][j] = true;
            }
        }
    }

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

    if (jogo->tamanho == 4) {
        celula = 88;
        y = 205;
        painelX = 324;
        larguraBotao = 78;
        espacoBotao = 90;
    } else {
        celula = 45;
        y = 155;
        painelX = 255;
        larguraBotao = 48;
        espacoBotao = 54;
    }

    x = (1000 - jogo->tamanho * celula) / 2;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();

        for (int i = 0; i < jogo->tamanho; i++) {
            Rectangle botao = {
                painelX + i * espacoBotao,
                605,
                larguraBotao,
                55
            };

            if (CheckCollisionPointRec(mouse, botao)) {
                jogo->simboloSelecionado = i + 1;
            }
        }

        if (mouse.x >= x &&
            mouse.x < x + jogo->tamanho * celula &&
            mouse.y >= y &&
            mouse.y < y + jogo->tamanho * celula) {

            int coluna = (mouse.x - x) / celula;
            int linha = (mouse.y - y) / celula;

            if (!jogo->fixas[linha][coluna]) {

                if (jogo->linhaSelecionada == linha &&
                    jogo->colunaSelecionada == coluna &&
                    jogo->tabuleiro[linha][coluna] != 0) {

                    jogo->tabuleiro[linha][coluna] = 0;

                } else {
                    jogo->tabuleiro[linha][coluna] =
                        jogo->simboloSelecionado;
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
                    jogo->tabuleiro[linha][coluna] = numero;
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
                jogo->tabuleiro[linha][coluna] = 0;
            }
        }
    }

    if (verificarVitoria(jogo)) {
        jogo->concluido = true;
    }
}

void desenharSimbolo(Texture2D imagem, Rectangle destino, Color cor) {
    if (imagem.id == 0) {
        return;
    }

    Rectangle origem = {
        0,
        0,
        imagem.width,
        imagem.height
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

void desenharJogo(Jogo *jogo) {
    int celula;
    int x;
    int y;
    int bloco;
    int painelX;
    int larguraBotao;
    int espacoBotao;

    if (jogo->tamanho == 4) {
        celula = 88;
        y = 205;
        bloco = 2;
        painelX = 324;
        larguraBotao = 78;
        espacoBotao = 90;
    } else {
        celula = 45;
        y = 155;
        bloco = 3;
        painelX = 255;
        larguraBotao = 48;
        espacoBotao = 54;
    }

    x = (1000 - jogo->tamanho * celula) / 2;

    const char *nomes[4] = {
        "",
        "ACESSO INICIAL",
        "QUEBRA DE FIREWALL",
        "SISTEMA CENTRAL"
    };

    DrawText(
        TextFormat("FASE %02d / 03", jogo->fase),
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

    if (existeConflito) {
        DrawText("CONFLITO DETECTADO", 300, 575, 18, RED);
    } else if (jogo->concluido) {
        DrawText("SISTEMA INVADIDO!", 300, 575, 18, VERDE);
    } else {
        DrawText("SELECIONE UM SIMBOLO", 300, 575, 18, VERDE_CLARO);
    }

    for (int i = 0; i < jogo->tamanho; i++) {
        Rectangle botao = {
            painelX + i * espacoBotao,
            605,
            larguraBotao,
            55
        };

        bool selecionado = false;

        if (jogo->simboloSelecionado == i + 1) {
            selecionado = true;
        }

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
            botao.y + (55 - lado) / 2,
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
}
