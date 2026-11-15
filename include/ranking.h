
#ifndef RANKING_H
#define RANKING_H

#define RANKING_MAX_ENTRADAS 10
#define RANKING_TAM_NOME 24
#define RANKING_ARQUIVO "data/ranking.txt"

#define PONTOS_ACERTO 100
#define PONTOS_FASE 1000

typedef struct {
    char nome[RANKING_TAM_NOME];
    int pontos;
    int tempo;
} EntradaRanking;

typedef struct {
    EntradaRanking entradas[RANKING_MAX_ENTRADAS];
    int quantidade;
} Ranking;

/*Pontuacao da partida*/

/* Acerto de simbolo: +100 */
void pontuacao_acerto(int *pontos);

/* Erro de simbolo: perde metade dos pontos atuais */
void pontuacao_erro(int *pontos);

/* Passou de fase: +1000 */
void pontuacao_fase(int *pontos);

/*Ranking (Top 10 salvo em arquivo)*/

/*Carrega do arquivo se nao existir, começa vazio. Retorna 1 se leu, 0 se vazio/novo.*/
int ranking_carregar(Ranking *ranking);

/*Salva no arquivo. Retorna 1 em sucesso, 0 em falha.*/
int ranking_salvar(const Ranking *ranking);

/*Verifica se a pontuação entra no Top 10.*/
int ranking_qualifica(const Ranking *ranking, int pontos, int tempo);

/*Insere mantendo ordem por pontos e tempo. Retorna a posição, 0 se não qualificar ou -1 em falha.*/
int ranking_inserir(Ranking *ranking, const char *nome, int pontos, int tempo);

#endif
