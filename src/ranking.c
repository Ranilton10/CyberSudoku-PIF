
#include "ranking.h"
#include <stdio.h>
#include <string.h>

/*Pontucao da partida*/

void pontuacao_acerto(int *pontos) {
    *pontos += PONTOS_ACERTO;
}

void pontuacao_erro(int *pontos) {
    *pontos -= *pontos / 2; /*perde metade (divisao inteira)*/
}

void pontuacao_fase(int *pontos) {
    *pontos += PONTOS_FASE;
}

/*Ranking*/

/*Copia o nome sem deixar o ponto e virgula quebrar o arquivo*/
static void copiarNome(char destino[], const char origem[]) {
    int i = 0;

    while (origem[i] != '\0' &&
           origem[i] != '\n' &&
           i < RANKING_TAM_NOME - 1) {

        if (origem[i] == ';') {
            destino[i] = ' ';
        } else {
            destino[i] = origem[i];
        }

        i++;
    }

    destino[i] = '\0';

    if (i == 0) {
        strcpy(destino, "ANONIMO");
    }
}

/*Ordena por maior pontuacao. Em empate, menor tempo vem primeiro.*/
static int vemAntes(int pontos1, int tempo1, int pontos2, int tempo2) {
    if (pontos1 > pontos2) {
        return 1;
    }

    if (pontos1 == pontos2 && tempo1 < tempo2) {
        return 1;
    }

    return 0;
}

/*Insere um jogador na posicao correta do Top 10*/
static int inserirOrdenado(Ranking *ranking, const char *nome, int pontos, int tempo) {
    int posicao = 0;

    while (posicao < ranking->quantidade &&
           !vemAntes(
               pontos,
               tempo,
               ranking->entradas[posicao].pontos,
               ranking->entradas[posicao].tempo
           )) {
        posicao++;
    }

    if (posicao >= RANKING_MAX_ENTRADAS) {
        return 0;
    }

    int fim = ranking->quantidade;

    if (fim >= RANKING_MAX_ENTRADAS) {
        fim = RANKING_MAX_ENTRADAS - 1;
    }

    for (int i = fim; i > posicao; i--) {
        ranking->entradas[i] = ranking->entradas[i - 1];
    }

    copiarNome(ranking->entradas[posicao].nome, nome);
    ranking->entradas[posicao].pontos = pontos;
    ranking->entradas[posicao].tempo = tempo;

    if (ranking->quantidade < RANKING_MAX_ENTRADAS) {
        ranking->quantidade++;
    }

    return posicao + 1;
}

/*Carrega os registros salvos no arquivo*/
int ranking_carregar(Ranking *ranking) {
    ranking->quantidade = 0;

    FILE *arquivo = fopen(RANKING_ARQUIVO, "r");

    if (arquivo == NULL) {
        return 0;
    }

    char linha[256];
    char nome[RANKING_TAM_NOME];
    int pontos;
    int tempo;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        tempo = 0;

        int campos = sscanf(
            linha,
            "%23[^;];%d;%d",
            nome,
            &pontos,
            &tempo
        );

        if (campos < 2 || pontos <= 0 || tempo < 0) {
            continue;
        }

        inserirOrdenado(ranking, nome, pontos, tempo);
    }

    fclose(arquivo);

    return ranking->quantidade > 0;
}

/*Salva nome, pontuacao e tempo no ranking.txt*/
int ranking_salvar(const Ranking *ranking) {
    FILE *arquivo = fopen(RANKING_ARQUIVO, "w");

    if (arquivo == NULL) {
        return 0;
    }

    int sucesso = 1;

    for (int i = 0; i < ranking->quantidade; i++) {
        if (fprintf(
                arquivo,
                "%s;%d;%d\n",
                ranking->entradas[i].nome,
                ranking->entradas[i].pontos,
                ranking->entradas[i].tempo
            ) < 0) {
            sucesso = 0;
            break;
        }
    }

    if (fclose(arquivo) != 0) {
        sucesso = 0;
    }

    return sucesso;
}

/*Verifica se o jogador pode entrar no Top 10*/
int ranking_qualifica(const Ranking *ranking, int pontos, int tempo) {
    if (pontos <= 0 || tempo < 0) {
        return 0;
    }

    if (ranking->quantidade < RANKING_MAX_ENTRADAS) {
        return 1;
    }

    EntradaRanking ultimo =
        ranking->entradas[RANKING_MAX_ENTRADAS - 1];

    return vemAntes(pontos, tempo, ultimo.pontos, ultimo.tempo);
}

/*Insere em ordem decrescente de pontos e salva no arquivo*/
int ranking_inserir(Ranking *ranking, const char *nome, int pontos, int tempo) {
    if (!ranking_qualifica(ranking, pontos, tempo)) {
        return 0;
    }

    Ranking copia = *ranking;

    int posicao = inserirOrdenado(&copia, nome, pontos, tempo);

    if (!ranking_salvar(&copia)) {
        return -1;
    }

    *ranking = copia;

    return posicao;
}
