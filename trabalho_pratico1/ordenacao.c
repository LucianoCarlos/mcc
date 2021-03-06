#include <stdio.h>
#include <stdlib.h>
#include "ordenacao.h"


/* Ordena um array de caixas pela base em ordem crescente.
 O(n log n)
*/
void merge(Caixa vetor[], int comeco, int meio, int fim) {

    int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;
    Caixa *vetAux;
    vetAux = (Caixa*)malloc(tam * sizeof(Caixa));

    while(com1 <= meio && com2 <= fim) {

        if(vetor[com1].base < vetor[com2].base) {
            vetAux[comAux] = vetor[com1];
            com1++;
        } else {
            vetAux[comAux] = vetor[com2];
            com2++;
        }
        comAux++;
    }

    while(com1 <= meio) { //Caso ainda haja elementos na primeira metade
        vetAux[comAux] = vetor[com1];
        comAux++;
        com1++;
    }

    while(com2 <= fim) {   //Caso ainda haja elementos na segunda metade
        vetAux[comAux] = vetor[com2];
        comAux++;
        com2++;
    }

    for(comAux = comeco; comAux <= fim; comAux++) {   //Move os elementos de volta para o vetor original
        vetor[comAux] = vetAux[comAux-comeco];
    }

    free(vetAux);
}

/* Ordena um array de caixas pela base em ordem crescente.
    O(n log n)
*/
void mergeSort(Caixa vetor[], int comeco, int fim) {
    if (comeco < fim) {
        int meio = (fim+comeco)/2;

        mergeSort(vetor, comeco, meio);
        mergeSort(vetor, meio+1, fim);
        merge(vetor, comeco, meio, fim);
    }
}
