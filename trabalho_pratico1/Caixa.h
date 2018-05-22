#ifndef CAIXA_H_INCLUDED
#define CAIXA_H_INCLUDED

#define R1 1
#define R2 2
#define True 1
#define False 0

/* Representation of a Caixa */
typedef struct {
    int id;
    int a; /* Altura para caixa */
    int l; /* Largura da Caixa */
    int p; /* Produndidade da Caixa */
    int v; /* Valor da Caixa */
    int base;
    char rotacao; /* Tipo de rotação da caixa */
}Caixa;

typedef struct {
    int valor;
    int indice_caixa;
}Solucao;

Caixa* load_box(const char*, int*, int*);
Caixa cria_caixa(int, int, int, int, int, int);
Caixa rotaciona_caixa(Caixa);
void imprime(Caixa *, int, int);
void ordena(Caixa*, int);
void libera_caixas(Caixa*);
#endif // CAIXA_H_INCLUDED
