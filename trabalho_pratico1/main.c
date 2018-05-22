/*
 Universidade Federal de Ouro Preto
 Departamento de Computação
 Programa de Pós-Graduação em Ciência da Computação (PPGCC)
 Trabalho Prático - Projeto e Análise de Algoritmos
 Prof.: Dr. Haroldo Gambini Santos

 Objetivo do trabalho: Este trabalho tem por objetivo implementar (em linguagem C)
 uma solução, usando estratégia de programação dinâmica, para o problema de construir
 uma pilha de altura máxima h, incluindo o maior valor possível em caixas, com repetição ilimitada.

 Autor: Luciano Carlos de Paiva
 Email: luciano.paiva@aluno.ufop.edu.br
 
 Indentation style = attach
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "empilhamento.h"

#define ajuda "\
uso: \n \
    %s <file_in> <file_out> [-r] \n\n \
    file_in: \tArquivo com dados das caixas e empilhamento \n \
    file_out: \tArquivo da solucao \n \
    -r:  \tVersão top down (default -> bottom up) \n\n \
    Ex.: %s stk10.data stk10.sol -r \n"


/* Resolve o problema do empilhamento de acordo com os
    parametros passado.

*/
void problema_empilhamento(char *file_in, char *file_out, int type) {

    int n, h, solucao_otima;
    Solucao **m;

    // Carrega as caixas
    Caixa* caixas = load_box(file_in, &n, &h);

    // Verifica se ouve erro ao carregar arquivo
    if(!caixas)
        exit(EXIT_FAILURE);

    caixas = cria_rotacoes(caixas, &n);
    //ordena(caixas, n);
    //imprime(caixas, n, h);

    lista *lista_empilhaveis = obtem_caixas_empilhaveis(caixas, n);

/*
    for(int k=0; k<n; k++) {
        printf("\n%d: ", k);
        imprime_lista(lista_empilhaveis[k]);
    }
*/
    m = cria_matriz_solucao(h, n);

    if(type==2) {
        solucao_otima = top_down(caixas, lista_empilhaveis, n, h, m);
    } else
        solucao_otima = bottom_up(caixas, n, h, lista_empilhaveis, m);

    printf("\nSolução ótima para h=%d é %d\n", h, solucao_otima);
/*
    for(int i=0; i<n; i++) {
        printf("\n");
        for(int k=0; k<=h; k++)
            printf("%d,%d\t", m[k][i].valor, m[k][i].indice_caixa);
    }*/

    grava_solucao_otima(m, caixas, n, h, file_out);
    libera_matriz_solucao(m);
    libera_caixas(caixas);
    libera_caixas_empilhavel(lista_empilhaveis, n);
}

int main(int argc, char *argv[]) {

    // Identifica o tipo de versão será executado.
    // 1 = versão iterativa; 2 = recursiva
    int type=1;

    if (argc<3) {
        fprintf( stderr, ajuda, argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    // Verifica o argumento passado como opcao caso seja 1
    // executa bottom up e 2 executa top down
    if(argc > 3 && (!strcmp(argv[3], "-r") || !strcmp(argv[3], "-R")))
        type=2;

    // Executa problema do empilhamento.
    problema_empilhamento(argv[1], argv[2], type);

    return 0;
}
