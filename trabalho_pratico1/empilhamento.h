#ifndef EMPILHAMENTO_H_INCLUDED
#define EMPILHAMENTO_H_INCLUDED

#include "Caixa.h"
#include "lista.h"

Caixa* load_box(const char*, int*, int*);
Caixa* cria_rotacoes(Caixa*, int*);

lista* obtem_caixas_empilhaveis(Caixa*, int);

Solucao** cria_matriz_solucao(int, int);

int top_down_rec(Caixa*, lista*, int, int, int, Solucao**);
int top_down(Caixa caixas[], lista* cx_empilha, int n, int h, Solucao **m);

int bottom_up(Caixa*, int, int, lista*, Solucao**);

int grava_solucao_otima(Solucao**, Caixa*, int, int, char*);

int grava_saida(int, lista, Caixa*, char*);
void libera_matriz_solucao(Solucao**);
void libera_caixas_empilhavel(lista*, int);

#endif // EMPILHAMENTO_H_INCLUDED
