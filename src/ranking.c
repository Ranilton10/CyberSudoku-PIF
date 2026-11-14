#include "ranking.h"
#include <stdio.h>
#include <string.h>

/*Pontucao da partida*/

void pontuacao_acerto(int *pontos) {
    *pontos += PONTOS_ACERTO;
}

void pontuacao_erro(int *pontos) {
    *pontos -= *pontos / 2;   /*perde metade  (divisao inteira)*/
}

void pontuacao_fase(int *pontos) {
    *pontos += PONTOS_FASE;
}

/*Ranking*/

/*Copia o nome trocando espaços/';' por '_' para não quebrar*/
static void copiar_nome(char *dest, const char *origem) {
    int i = 0;
    for (; origem[i] != '\0' && origem[i] != '\n' && i < RANKING_TAM_NOME - 1; i++) {
        dest[i] = (origem[i] == ' ' || origem[i] == ';') ? '_' : origem[i];
    }
    dest[i] = '\0';
    if (i == 0){
        strcpy(dest, "ANONIMO");
    }
}

int ranking_carregar(Ranking *r) {
    r->quantidade = 0;
    FILE *f = fopen(RANKING_ARQUIVO, "r");
    if (!f){
        return 0;
    }
    char nome[RANKING_TAM_NOME];
    int pontos;
    while (r->quantidade < RANKING_MAX_ENTRADAS && fscanf(f, "%23[^;];%d\n", nome, &pontos) == 2) {
        copiar_nome(r->entradas[r->quantidade].nome, nome);
        r->entradas[r->quantidade].pontos = pontos;
        r->quantidade++;
    }
    fclose(f);
    return r->quantidade > 0;
}

int ranking_salvar(const Ranking *r) {
    FILE *f = fopen(RANKING_ARQUIVO, "w");
    if (!f){
        return 0;
    }
    for (int i = 0; i < r->quantidade; i++) {
        fprintf(f, "%s;%d\n", r->entradas[i].nome, r->entradas[i].pontos);
    }
    fclose(f);
    return 1;
}

int ranking_qualifica(const Ranking *r, int pontos) {
    if (pontos <= 0){
        return 0;
    }
    if (r->quantidade < RANKING_MAX_ENTRADAS){
        return 1;
    }
    return pontos > r->entradas[r->quantidade - 1].pontos;
}

int ranking_inserir(Ranking *r, const char *nome, int pontos) {
    if (!ranking_qualifica(r, pontos)){ 
        return 0;
    }

    /*Em empate, quem chegou primeiro fica na frente (usa >)*/
    int pos = 0;
    while (pos < r->quantidade && r->entradas[pos].pontos >= pontos){ 
        pos++;
    }

    int fim = (r->quantidade < RANKING_MAX_ENTRADAS) ? r->quantidade : RANKING_MAX_ENTRADAS - 1;
    for (int i = fim; i > pos; i--) r->entradas[i] = r->entradas[i - 1];

    copiar_nome(r->entradas[pos].nome, nome);
    r->entradas[pos].pontos = pontos;
    if (r->quantidade < RANKING_MAX_ENTRADAS){
        r->quantidade++;
    }
    
    ranking_salvar(r);
    return pos + 1;
}