#include <stdio.h>
#include <stdlib.h>

enum cor { VERMELHO, PRETO, DUPLO_PRETO };

typedef struct no {
    int valor;
    enum cor cor;
    struct no *esq, *dir, *pai;
} Folha;

//funções de redução de codigo
void iniciaNull(Folha **raiz);
enum cor getCor(Folha *folha);
int eh_raiz(Folha *folha);
int eh_filho_esquerdo(Folha *folha);
int eh_filho_direito(Folha *folha);
Folha* getIrmao(Folha *folha);
Folha* getTio(Folha *folha);
int getAltura(Folha *raiz);
int maior(int a, int b);
int maior_elemento(Folha *raiz);
int menor_elemento(Folha *raiz);
void imprimir(Folha *raiz);
void preOrder(Folha *raiz);
void inOrder(Folha *raiz);
Folha* novaFolha(Folha *pai, int input);

//funções importantes
void giroEsquerda(Folha **raizReal, Folha *raiz);
void giroDireita(Folha **raizReal, Folha *raiz);
void balancear(Folha **raiz, Folha *atual);
void inserir(Folha **raiz, int input);

void remove_duplo_preto(Folha **raiz, Folha *atual);
void balancearRemocao(Folha **raiz, Folha *atual);
void remover(Folha **raiz, int input);