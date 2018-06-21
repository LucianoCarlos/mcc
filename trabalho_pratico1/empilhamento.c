#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "empilhamento.h"

/* Carrega caixas do arquivo texto para a memória

    Parameters
    ----------
    path: caminho do arquivo a ser carregado, que contém atributos de caixas.
    n: ponteiro para numero total de caixas
    h: ponteiro para altura máxima da pilha

    returns
    -------
    Ponteiro para array do tipo Caixa.
*/
Caixa* load_box(const char * path, int *n, int *h) {

    FILE *arq;
    arq = fopen(path, "r");
    if (arq == NULL) {
        printf("Arquivo Invalido...\n");
        return NULL;
    }

    // Lê total de caixas e altura da pilha
    fscanf(arq, "%d\n%d", &(*n), &(*h));

    if(*n <= 0 || *h <=0){
        printf("Arquivo de entrada inválido...\n");
        fclose(arq);
        return NULL;
    }

    // Aloca array para armazenar caixas
    Caixa* caixas = malloc(sizeof(Caixa) * (*n));

    if(!caixas) {
        printf("Falha ao alocar memoria...\n");
        fclose(arq);
        return NULL;
    }

    int *v = (int*) malloc(sizeof(int) * (*n));

    // Lê valores
    int i = 0;
    while(i < *n && fscanf(arq, "%d", &v[i++]) != EOF);

    // Lê valores das caixas. l=largura; a=altura; p=profundidade; v=valor
    int l, a, p;
    i = 0;
    while(fscanf(arq,"%d\n%d\n%d", &l, &a, &p ) != EOF && i < *n) {
        caixas[i] = cria_caixa(l, a, p, v[i], R1, i+1);
        i++;
    }

    // Libera recursos
    free(v);
    fclose(arq);

    if( i!= *n) {
        printf("\nArquivo de entrada incorreto!");
        free(caixas);
        return NULL;
    }

    return caixas;
}

/* Cria rotações R2 para todos as caixas.

    parameters
    ----------
    caixas: array de caixas. Essas caixas estão em rotação R1.
    n: número de elementos do array.

    returns
    -------
    Retorna o array de Caixas com rotações R1 e R2.
*/
Caixa* cria_rotacoes(Caixa *caixas, int *n) {

    int i, n_elementos = *n;

    *n = *n * 2;
    // Cria novo array para comportar as caixas e suas rotações
    caixas = realloc(caixas, sizeof(Caixa) * (*n));

    for(i = 0; i < n_elementos; i++) {
        caixas[i+n_elementos] = rotaciona_caixa(caixas[i]);
    }

    return caixas;
}

/* Cria um array com tamanho igual o número de caixas. Cada
    posição armazena uma lista. Cada lista armazena todas as
    caixas que podem ser empilhadas com aquela posição do array.

    parameters
    ----------
    caixas: array unidimensional de caixas
    n: tamanho do array de caixas

    returns
    -------
    array de lista encadeada.
*/
lista* obtem_caixas_empilhaveis(Caixa *caixas, int n) {
    int i, k;
    tipo_elemento e;

    lista *caixas_empilhaveis = malloc(sizeof(lista) * n);

    if(!caixas_empilhaveis)
        return NULL;

    for(i=0; i<n; i++) {
        caixas_empilhaveis[i] = cria_lista();

        for(k=0; k<n; k++) {

            //  Somente se tiver uma base maior que a base a ser empilhada
            //if((caixas[k].base) > (caixas[i].base)){
            //    break;
            //}

            if((caixas[k].p <= caixas[i].p) && (caixas[k].l <= caixas[i].l)) {
                e.indice = k;
                insere_apos(caixas_empilhaveis[i], e);
            }
        }

    }
    return caixas_empilhaveis;
}

/* Aloca espaço para a matriz onde será armazenados resultados já calculados.
    Cada posição desta matriz armazenará dois valores, uma irá armazenar o
    valor da solução ótima e outro será responsável por guardar o índice da
    caixa anterior que representa o valor ótimo para aquela posição.

    Os elementos da primeira linha será todos inicializados com 0. Já que com
    uma altura 0 não é possível empilhar nenhum caixa. O restante será
    inicializado com -1, de modo que seja possível saber se determinada posisão
    já foi anteriormente preenchida.

    parameters
    ----------
    h: número de linhas. Irá representar a altura máxima empilhável.
    n: número de colunas. Irá representar as caixas. Cada indice representa
       uma caixa indexada no array de caixas.

    returns
    -------
    Array de duas dimensões criado.

*/
Solucao** cria_matriz_solucao(int h, int n) {

    int i, k;
    Solucao **m;
    // Elementos empilhamento

    h++;
    m = malloc(sizeof(Solucao*) * h);
    m[0] = malloc(sizeof(Solucao)*h*n);

    for(i=1; i<h; ++i) {
        m[i] = m[i-1]+n;
    }

    // Inicializa primeiro elemento com 0
    for(k=0; k<n; k++) {
        m[0][k].valor = 0;
        m[0][k].indice_caixa = 0;
    }

    // Inicializar valores de cada altura
    for (i = 0; i < n; i++ ) {
        for(k = 1; k < h; k++) {

            m[k][i].valor = -1;
            m[k][i].indice_caixa = -1;
        }
    }
    return m;
}

/* Preenche a matriz solução de forma recursiva.

    parameters
    ----------
    caixas: array unidimensional com as caixas e suas possíveis rotações.
    n: número total de caixas e tamanho do array de caixas.
    h: altura máxima que será calculada a solução ótima.
    cx_empilha: array unidimensional. Cada posição possui uma lista
        com todas as caixas que respeital a restrição do empilhada com a caixa
        daquele índice.
    j: indice a posição a ser preenchida.
    m: array bidimensional onde será salvo as soluções. Cada linha representa uma
        altura e cada coluna representa um caixa, de acordo com indices do array
        cd caixas.

    returns
    -------
    valor total ótimo que pode ser obtidos utilizando as caixas para a altura máxima.
*/
int top_down_rec(Caixa caixas[], lista* cx_empilha, int n, int h, int j, Solucao **m) {

    int valor_max = 0, h_anterior;
    tipo_nodo *ptr;

    if(m[h][j].valor != -1)
        return m[h][j].valor;

    m[h][j].valor = 0;
    h_anterior = h - caixas[j].a;

    if(h_anterior >=0) {

        // Percorre a lista de caixas empilhaveis, a fim de encontrar a solução ótima
        ptr = cx_empilha[j]->first;
        while(ptr) {
            valor_max = caixas[j].v + top_down_rec(caixas, cx_empilha, n, h_anterior, ptr->item.indice, m);

            if(m[h][j].valor < valor_max) {
                m[h][j].valor = valor_max;
                m[h][j].indice_caixa = ptr->item.indice;
            }

            ptr=ptr->next;
        }

        /*
        tipo_elemento e;
        ptr = get_element(cx_empilha[j], &e);
        while(ptr){

            valor_max = arr[j].v + M_R(m, arr, cx_empilha, n, h_anterior, e.indice);
            //printf("\n%d", v);

            if(m[h][j].valor < valor_max){
                m[h][j].valor = valor_max;
                m[h][j].indice_caixa = e.indice;
            }
            ptr = next_element(cx_empilha[j], &e, ptr);
        }*/

    }
    return m[h][j].valor;
}


/* Resolve o problema do empilhamento usando paradigma de programação
    dinâmica, usando visão top down. No pior caso o custo assintótico
    será sempre O(n² * h)

    parameters
    ----------
    caixas: array unidimensional com as caixas e suas possíveis rotações.
    n: número total de caixas e tamanho do array de caixas.
    h: altura máxima que será calculada a solução ótima.
    cx_empilha: array unidimensional. Cada posição possui uma lista
        com todas as caixas que respeital a restrição do empilhada com a caixa
        daquele índice.
    m: array bidimensional onde será salvo as soluções. Cada linha representa uma
        altura e cada coluna representa um caixa, de acordo com indices do array
        cd caixas.

    returns
    -------
    valor total ótimo que pode ser obtidos utilizando as caixas para a altura máxima.
*/
int top_down(Caixa caixas[], lista* cx_empilha, int n, int h, Solucao **m) {

    int max=0, i, valor;

    for(i=0; i<n; i++) {
        valor = top_down_rec(caixas, cx_empilha, n, h, i, m);

        if(valor > max)
            max=valor;
    }

    return max;
}

/* Resolve o problema do empilhamento usando paradigma de programação
    dinâmica, usando visão bottom up. No pior caso o custo assintótico
    será sempre O(n² * h)

    parameters
    ----------
    caixas: array unidimensional com as caixas e suas possíveis rotações.
    n: número total de caixas e tamanho do array de caixas.
    altura: altura máxima que será calculada a solução ótima.
    cx_empilha: array unidimensional. Cada posição possui uma lista
        com todas as caixas que respeital a restrição do empilhada com a caixa
        daquele índice.
    m: array bidimensional onde será salvo as soluções. Cada linha representa uma
        altura e cada coluna representa um caixa, de acordo com indices do array
        cd caixas.

    returns
    -------
    valor total ótimo que pode ser obtidos utilizando as caixas para a altura máxima.
*/
int bottom_up(Caixa caixas[], int n, int altura, lista *cx_empilha, Solucao **m) {

    int posicao_ant, valor_max, h, i;
    tipo_nodo *ptr;

    // Itera sobre cadas altura calculando solução ótima
    for(h=1; h<=altura; h++) {

        // Percorrendo todas caixas obtendo a melhor solução
        for(i=0; i<n; i++) {

            // A solução ótima é formada combinando com uma versão anterior
            posicao_ant = h - caixas[i].a;
            m[h][i].valor=0;

            // Caso já tenha sido calculado uma solução ótima para um subproblema
            if(posicao_ant >=0) {
                valor_max = 0;

                // Percorre a lista de caixas empilhaveis, a fim de encontrar a solução ótima
                ptr = cx_empilha[i]->first;
                while(ptr) {
                    valor_max = m[posicao_ant][ptr->item.indice].valor + caixas[i].v;

                    if(m[h][i].valor < valor_max) {
                        m[h][i].valor = valor_max;
                        m[h][i].indice_caixa = ptr->item.indice;
                    }

                    ptr=ptr->next;
                }

                /*
                tipo_elemento e;
                ptr = get_element(cx_empilha[i], &e);
                while(ptr){

                   valor_max = m[posicao_ant][e.indice].valor + caixas[i].v;

                   // Calcula solução ótima, obtendo máximo de todas soluçãoes anteriores
                   if (m[h][i].valor < valor_max){

                       m[h][i].valor = valor_max;
                       m[h][i].indice_caixa = e.indice;
                   }
                   ptr = next_element(cx_empilha[e.indice], &e, ptr);

                } //while
                */
            } //for
        } //for
    }

    // Obtém solução otima para altura.
    valor_max = m[altura][0].valor;
    for(i=1; i<n; i++)
        if(valor_max < m[altura][i].valor)
            valor_max = m[altura][i].valor;

    return valor_max;
}

/* Dada uma matriz de solução, de tamanho hxn, com seus valores já calculados,
    calcula quais caixas foram utilizadas para formar a solução ótima.
    De posse dessas caixas salva em arquivo.

    parameters
    ----------
    m: Matriz solução completa
    caixas: array de caixas
    n: numero de caixas
    h: altura máxima
    path_saida: caminho do arquivo de saída a ser gravado.

    returns
    -------
    1 caso a operação tenha sido realizada com sucesso e 0 caso contrário.
*/
int grava_solucao_otima(Solucao **m, Caixa *caixas, int n, int h, char *path_saida) {

    int k;

    int indice_melhor_solucao = -1;
    lista l = cria_lista();
    tipo_elemento e;

    if(!l) {
        printf("\nFalha ao criar lista para armazenar caixas da solução!");
        return 0;
    }

    // Calcula solução ótima para altura h
    Solucao *s = &m[0][h];

    for(k=1; k<n; k++) {

        if(m[h][k].valor > s->valor) {
            s = &m[h][k];
            indice_melhor_solucao = k;
        }
    }

    if(indice_melhor_solucao == -1) {
        printf("\nNão possue solução ótima!");
        return False;
    }

    // Obtendo caixas utilizada
    while(h > 0 && m[h][indice_melhor_solucao].valor != 0 && m[h][indice_melhor_solucao].indice_caixa != -1) {

        e.indice = indice_melhor_solucao;
        insere_apos(l, e);

        indice_melhor_solucao = m[h][indice_melhor_solucao].indice_caixa;

        h -= caixas[e.indice].a;
    }

    // Grava solução otima em arquivo
    grava_saida(s->valor, l, caixas, path_saida);

    //printf("\nO total de caixas utilizadas: %d", l->tamanho);
    //printf("\nO Valor máximo conseguido é %d\n", s->valor);

    termina_lista(l);
    return True;
}

/* Consome uma lista encadeada de índice de caixa, gravando cada caixa
    em arquivo texto, padrão descrito no relatório.

    parameters
    ----------
    valor_total: valor total conseguido com empilhamento ótimo.
    l: lista de índices que caixas.
    caixas: array contendo todas caixas e suas possíveis rotações R1 e R2.
    path_saida: caminho de gravação do arquivo de saída.

    returns
    -------
    0 caso ocorra algum erro na operação e 1 caso tenha sido realizada com sucesso.

*/
int grava_saida(int valor_total, lista l, Caixa *caixas, char *path_saida) {

    tipo_elemento e;
    tipo_nodo *ptr;

    FILE *arq = fopen(path_saida, "w");

    if (arq == NULL) {
        printf("Arquivo de saída Invalido...\n");
        return False;
    }

    fprintf(arq, "%d\n%d", valor_total, tamanho_lista(l));

    ptr = get_element(l, &e);
    while(ptr) {
        fprintf(arq, "\n%d %d", caixas[e.indice].id, caixas[e.indice].rotacao);
        ptr = next_element(&e, ptr);
    }

    fclose(arq);
    return True;
}

/* Libera memória alocada para armazenar caixas

    parameters
    ----------
    m: matriz solução
    h: número de linhas da matriz.
*/
void libera_matriz_solucao(Solucao **m) {
    if(m[0]) {
        free(m[0]);
        free(m);
    }
}

/* Libera o array de lista que contém todas as caixas que podem ser empilhadas
    com aquela referente a aquele índice.

    parameters
    ----------
    caixa_empilhavel: array de lista encadeada.
    n: tamanho do array.
*/
void libera_caixas_empilhavel(lista *caixa_empilhavel, int n) {
    int i;

    if(caixa_empilhavel) {
        for(i=0; i<n; i++)
            termina_lista(caixa_empilhavel[i]);

        free(caixa_empilhavel);
    }
}
