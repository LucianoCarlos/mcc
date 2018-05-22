#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "Caixa.h"

typedef struct {
    int indice;
} tipo_elemento;

typedef struct nodo {
    tipo_elemento item;
    struct nodo *next;
} tipo_nodo;

typedef struct {
    tipo_nodo *first, *last;
    int tamanho;
} tipo_lista;

typedef tipo_lista *lista;

lista cria_lista();
int insere_apos(lista, tipo_elemento);
tipo_nodo* get_element(lista, tipo_elemento*);
int tamanho_lista(lista);
tipo_nodo* next_element(tipo_elemento*, tipo_nodo*);
void termina_lista(lista);
void imprime_lista(lista);

#endif // LISTA_H_INCLUDED
