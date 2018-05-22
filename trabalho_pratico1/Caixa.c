#include <stdio.h>
#include <stdlib.h>
#include "Caixa.h"
#include "ordenacao.h"

/* Cria uma caixa.

    parameters
    ----------
    larg: largura da caixa
    alt: altura da caixa
    prof: profundidade da caixa
    valor: valor da caixa
    rotacao: rotação da caixa
    id: identificador da caixa

    returns
    -------
    A caixa criada
*/
Caixa cria_caixa(int larg, int alt, int prof, int valor, int rotacao, int id) {
    Caixa caixa;

    caixa.a = alt;
    caixa.l = larg;
    caixa.p = prof;
    caixa.v = valor;
    caixa.id = id;
    caixa.rotacao = rotacao;
    caixa.base = larg * prof;

    return caixa;
}

/* Rotaciona uma caixa em R2.

    parameters
    ----------
    cx: A caixa a ser rotacionada

    returns
    -------
    A caixa rotacionada
*/
Caixa rotaciona_caixa(Caixa cx) {
    return cria_caixa(cx.a, cx.l, cx.p, cx.v, R2, cx.id);
}


void imprime(Caixa *cxs, int n, int h) {

    printf("Altura máxima: %d \nNúmero total de caixas: %d", h, n);
    printf("\n\nindice\tid\tlargura\taltura\tprofundidade\trotação \tvalor");

    for(int i = 0; i<n; i++) {
        printf("\n%d\t%d\t%d\t%d\t%d\t\t%d\t\t%d\t%d",
            i, cxs[i].id, cxs[i].l, cxs[i].a, cxs[i].p, cxs[i].rotacao, cxs[i].v, cxs[i].l * cxs[i].p);
    }
    printf("\n\n");
}

void imprime_cx(int a, int l, int p) {
    printf("altura=%d, largura=%d, profundidade=%d", a, l, p);
}

void ordena(Caixa *cxs, int n){
    mergeSort(cxs, 0, n-1);
}

void libera_caixas(Caixa *cxs){
    free(cxs);
}
