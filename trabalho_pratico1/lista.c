#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/* Cria uma lista encadeada. Faz a inicialização do inicio e do fim com NULL.

    returns
    -------
    A lista em caso de sucesso e NULL em caso de insucesso.
*/
lista cria_lista() {
    lista l;
    l = (lista) malloc(sizeof(tipo_lista));

    if(l != NULL) { /**l*/

        l -> first = NULL; // primeiro elemento da lista recebe NULL
        l -> last = NULL; // segundo elemento da lista recebe NULL.
        l -> tamanho = 0; // tamanho e inicializado com 0.
    }
    return l;
}

/* Insere um elemento no fim da lista.

    parameters
    ----------
    l: lista.
    e: elemento tipo_elemento a ser inserido.

    returns
    -------
    1 em caso de sucesso e 0 caso contrário.
*/
int insere_apos(lista l, tipo_elemento e) {
    tipo_nodo *p;

    p = (tipo_nodo*)malloc(sizeof(tipo_nodo)); //Aloca um nodo

    if(!p) // caso nao consiga alocar retorna 0
        return 0;

    p->item = e; // item do nodo recebe elemento
    p->next = NULL; // e next que a proxima posição recebe NULL

    if(l->first == NULL) // Caso seja o primeiro elemento o primeiro da lista recebe p;
        l->first = p;
    else                // Caso nao seja o proximo da lista recebe p
        l->last->next = p;

    l->last = p;
    l->tamanho++;
    return 1;
}

/* Calcula o tamanho de uma lista

    parameters
    ----------
    l: lista

    returns
    -------
    Um inteiro representando o tamanho da lista
*/
int tamanho_lista(lista l) {
    return l->tamanho;
}

// busca um elemento na lista
tipo_nodo* get_element(lista l, tipo_elemento *e) {

    if(!l)
        return NULL;

    *e = l->first->item; // elemento retorna item do elementdo do indice corrente
    return l->first;
}

/* Retorna o próximo elemento de uma lista.

    parameters
    ----------
    l: lista encadeada.
    e: ponteiro para elemento da lista.
    ptr: um ponteiro para posição corrente da lista. Com base neste ponteiro
    será calculado o próximo elemento.

    returns
    -------
    Um ponteiro para a posição corrente da lista.
*/
tipo_nodo* next_element(tipo_elemento *e, tipo_nodo *ptr) {

    if(!ptr->next)
        return NULL;

    *e = ptr->next->item;
    return ptr->next;
}

/* Desaloca a memória utizada por uma lista.

    parameters
    ----------
    l: lista a ser destruída.
*/
void termina_lista(lista l) {
    tipo_nodo *p;

    while(l->first != NULL) {
        p = l->first;
        l->first = l->first->next ; // primeiro elemento da lista recebe o proximo
        free(p);
    }
    free(l);
}

/* Imprime os elementos presentes na lista.

    parameters
    ----------
    l: lista a ser impressa.
*/
void imprime_lista(lista l) {
    tipo_nodo *p = l->first;
    while(p) {
        printf("%d -> ", p->item.indice);
        p=p->next;
    }
    printf("NULL");
}
