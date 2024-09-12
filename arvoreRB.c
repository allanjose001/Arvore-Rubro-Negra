#include <stdio.h>
#include <stdlib.h>
#include "arvoreRB.h"

//⚠️Observação para o professor:⚠️
//- semelhante a arvore AVL, em diversas ocasiões eu uso o termo folha para um nó que
//não necessariamente é uma folha, isto é apenas uma preferencia pessoal.
//- nomiei algumas funções simples com a sintaxe de Java por preferencia pessoal.
//- o codigo está cheio de printf que dizem qual movimento foi realizado

//necessario para função de remover: quando é removido uma folha preta, ela tem que se tornar
//duplo preta, isso é problematico, pois nó temos uma folha que não existe e tem uma cor diferente das demais
//para resolver isso, foi criado uma variavel global que servirá de nó inexistente que tem cor duplo preta
Folha *no_null;

void iniciaNull(Folha **raiz) {
    *raiz = NULL;
    no_null = malloc(sizeof(Folha));
    no_null->cor = DUPLO_PRETO;
    no_null->valor = 0;
    no_null->esq = NULL;
    no_null->dir = NULL;
}

//============================Funções para redução de codigo============================

//nomiei de get por gosto pessoal e semelhanças com Java
enum cor getCor(Folha *folha) {
    enum cor c;
    if (folha == NULL || folha->cor == PRETO) {
        c = PRETO;
    } else {
        c = VERMELHO;
    }
    return c;
}

//se o pai da folha atual for null, então ela só pode ser a raiz da arvore
int eh_raiz(Folha *folha) {
    return (folha->pai == NULL);
}

//se o pai existir e a folha atual for um filho esquerdo
int eh_filho_esquerdo(Folha *folha) {
    return (folha->pai != NULL && folha == folha->pai->esq);
}
int eh_filho_direito(Folha *folha) {
    return (folha->pai != NULL && folha == folha->pai->dir);
}

Folha* getIrmao(Folha *folha) {
    if (eh_filho_esquerdo(folha)) {
        return folha->pai->dir;
    } else {
        return folha->pai->esq;
    }
}

Folha* getTio(Folha *folha) {
    return getIrmao(folha->pai);
}

int getAltura(Folha *raiz) {
    if (raiz == NULL) {
        return 0;
    } else {
        return 1 + maior(getAltura(raiz->dir), getAltura(raiz->esq));
    }
}

int maior(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int maior_elemento(Folha *raiz) {
    if (raiz == NULL) {
        return -1;
    } else if (raiz->dir == NULL) {
        return raiz->valor;
    } else {
        return maior_elemento(raiz->dir);
    }
}

int menor_elemento(Folha *raiz) {
    if (raiz == NULL) {
        return -1;
    } else if (raiz->esq == NULL) {
        return raiz->valor;
    } else {
        return maior_elemento(raiz->esq);
    }
}

void imprimir(Folha *raiz) {
    switch(raiz->cor) {
        case PRETO:
            printf("\x1b[30m[%d]\x1b[0m", raiz->valor);
            break;
        case VERMELHO:
            printf("\x1b[31m[%d]\x1b[0m", raiz->valor);
            break;
        case DUPLO_PRETO:
            printf("\x1b[32m[%d]\x1b[0m", raiz->valor);
            break;
    }
}

void preOrder(Folha *raiz) {
    if (raiz) {
        imprimir(raiz);
        preOrder(raiz->esq);
        preOrder(raiz->dir);
    }
}

void inOrder(Folha *raiz) {
    if (raiz) {
        inOrder(raiz->esq);
        printf("%d ", raiz->valor);
        inOrder(raiz->dir);
    }
}

Folha* novaFolha(Folha *pai, int input) {
    Folha *novo = malloc(sizeof(Folha));

    if (novo) {
        novo->valor = input;
        novo->cor = VERMELHO;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->pai = pai;
    } else {
        printf("erro no malloc\n");
    }
    printf("\nnumero inserido\n");
    return novo;
}

//==================================Funções importantes========================================

void giroEsquerda(Folha **raizReal, Folha *raiz) {
    Folha *novaRaiz, *t2;

    int raiz_era_direito = eh_filho_direito(raiz);

    novaRaiz = raiz->dir;
    t2 = novaRaiz->esq;

    raiz->dir = t2;
    if (t2 != NULL) {
        t2->pai = raiz;
    }

    novaRaiz->esq = raiz;
    novaRaiz->pai = raiz->pai;
    raiz->pai = novaRaiz;

    if (eh_raiz(novaRaiz)) {
        *raizReal = novaRaiz;
    } else {
        if (raiz_era_direito) {
            novaRaiz->pai->dir = novaRaiz;
        } else {
            novaRaiz->pai->esq = novaRaiz;
        }
    }
}

//raizReal diz respeito a raiz universal da arvore,a segunda raiz se refere
//a raiz da sub-arvore,
void giroDireita(Folha **raizReal, Folha *raiz) {
    Folha *novaRaiz, *t2;
    int raiz_era_esquerdo = eh_filho_esquerdo(raiz);
    //declara quem é quem na rotação
    novaRaiz = raiz->esq;
    t2 = novaRaiz->dir;

    //inicia a rotação de fato, semelhante a como é feito na AVL
    raiz->esq = t2;
    if (t2 != NULL) {
        t2->pai = raiz;
    }
    novaRaiz->dir = raiz;

    //atualiza os pais de todo mundo
    novaRaiz->pai = raiz->pai;
    raiz->pai = novaRaiz;

    //agora é preciso subir para o pai para dizer quem é o novo filho.
    //se for a raiz real da arvore, então não existe pai e atualiza a raizReal
    if (eh_raiz(novaRaiz)) {
        *raizReal = novaRaiz;
    } else {
        //se a antiga raiz era um filho esquerdo, precisa dizer para o pai
        //quem é o novo filho esquerdo
        if (raiz_era_esquerdo) {
            novaRaiz->pai->esq = novaRaiz;
        } else {
            //analogo a anterior, so que para quando é filho direito
            novaRaiz->pai->dir = novaRaiz;
        }
    }
}

//na arvore rb a verificação de desbalanceamento não é feito de cima para baixo,
//e sim de baixo para cima, portanto o elemento "atual" não é a raiz da sub-arvore
//e sim o elemento mais recente inserido, isso se deve ao fato de ser uma função iterativa.
void balancear(Folha **raiz, Folha *atual) {
    while (getCor(atual->pai) == VERMELHO && getCor(atual) == VERMELHO) {
        //caso 1, recolore quando pai e tio são vermelhos
        if (getCor(getTio(atual)) == VERMELHO) {
            //pai e tio ficam pretos, e avô agora é vermelho
            //nó atual continua vermelho.
            getTio(atual)->cor = PRETO;
            atual->pai->cor = PRETO;
            atual->pai->pai->cor = VERMELHO;

            //avô virar vermelho pode ocasionar em outra sequencia de vermelho
            //define atual como sendo avô e o faz passar novamente pelo balanceamento
            atual = atual->pai->pai;
            printf("recoloracao\n");
            continue;
        }
        //case 2, rotação direita quando há sequencia de 2 filhos esquerdos
        if (eh_filho_esquerdo(atual) && eh_filho_esquerdo(atual->pai)) {
            //o pivo de rotação é o avô da folha recem inserida (a folha atual)
            giroDireita(raiz, atual->pai->pai);
            //desce a cor vermelho da raiz da subarvore para o seu filho que ainda é preto
            atual->pai->cor = PRETO;
            atual->pai->dir->cor = VERMELHO;
            printf("rotacao simples direita\n");
            continue;
        }
        //caso 3, rotação esquerda quando há sequencia de 2 filhos direitos
        if (eh_filho_direito(atual) && eh_filho_direito(atual->pai)) {
            giroEsquerda(raiz, atual->pai->pai);
            atual->pai->cor = PRETO;
            atual->pai->esq->cor = VERMELHO;
            printf("rotacao simples esquerda\n");
            continue;
        }

        //caso 4, rotação dupla esquerda
        //sequencia de pais diferentes, ou seja, pai direito e filho esquerdo ou vice versa
        //isso gera o caracteristimo joelho da rotação dupla
        if (eh_filho_esquerdo(atual) && eh_filho_direito(atual->pai)) {
            //pivo da rotação é o pai do nó recem inserido
            giroDireita(raiz, atual->pai);
            //após a rotação, a folha atual agora se encontra no meio de uma reta de 3 folhas para a direita
            //então basta fazer uma rotação para a esquerda usando o pai dele
            giroEsquerda(raiz, atual->pai);
            //após as 2 rotações, "atual" agora é a raiz da subarvore e ainda é vermelho, então ele deve
            //trocar de cor com seu filho que ainda é preto
            atual->cor = PRETO;
            atual->esq->cor = VERMELHO;
            printf("rotacao dupla esquerda\n");
            continue;
        }
        //caso 5, rotacao dupla direita
        if (eh_filho_direito(atual) && eh_filho_esquerdo(atual->pai)) {
            giroEsquerda(raiz, atual->pai);
            giroDireita(raiz, atual->pai);

            atual->cor = PRETO;
            atual->dir->cor = VERMELHO;
            printf("rotacao dupla direita\n");
            continue;
        }
    }
    //caso 0: sempre define a cor da raiz como preto
    (*raiz)->cor = PRETO;
    
}

//ponteiro duplo se refere a raiz real da arvore
void inserir(Folha **raiz, int input) {
    Folha *atual, *pai, *novo;

    //começa a percorrer a arvore a partir da raiz
    atual = *raiz;
    pai = NULL;

    //percorre a arvore até chegar em um NULL
    while (atual != NULL) {
        pai = atual;
        if (input > atual->valor) {
            atual = atual->dir;
        } else {
            atual = atual->esq;
        }
    }

    novo = novaFolha(pai, input);

    if (eh_raiz(novo)) {
        *raiz = novo;
    } else {
        if (input > pai->valor) {
            pai->dir = novo;
        } else {
            pai->esq = novo;
        }
    }

    balancear(raiz, novo);
}

//⚠️começar pela função de remover, depois volta para estas 2 funções⚠️

//função para automatizar a atualização de ponteiros dos pais
void remove_duplo_preto(Folha **raiz, Folha *atual) {
    if (atual == no_null) {
        if(eh_filho_esquerdo(atual)) {
            atual->pai->esq = NULL;
        } else {
            atual->pai->dir = NULL;
        }
    } else {
        atual->cor = PRETO;
    }
}

//o slide de arvores RB tem todos os casos de remoção para melhor entendimento

void balancearRemocao(Folha **raiz, Folha *atual) {
    printf("\niniciou balanceamento\n");

    //caso 1, recolore a raiz pra preto
    if (eh_raiz(atual)) {
        atual->cor = PRETO;

        printf("caso 0\n");
        return;
    }   

    //caso 2: pai preto, irmão vermelho e sobrinhos preto
    if (getCor(atual->pai) == PRETO &&
        getCor(getIrmao(atual)) == VERMELHO &&
        getCor(getIrmao(atual)->dir) == PRETO &&
        getCor(getIrmao(atual)->esq) == PRETO) {

        printf("caso 2 inicio\n");
        //se for filho esquerdo, faz a rotação para esquerda usando o pai como pivo de rotação
        if(eh_filho_esquerdo(atual)) {
            giroEsquerda(raiz, atual->pai);
        } else {
            giroDireita(raiz, atual->pai);
        }
        //faz a correção de cores normalmente
        atual->pai->pai->cor = PRETO;
        atual->pai->cor = VERMELHO;

        //este caso não corrige o desbalanceamento, apenas faz a arvore entrar em um
        //caso que possa ser tratado
        balancearRemocao(raiz, atual);
        printf("caso 2 fim\n");
        return;
    }

    //caso 3: pai, irmãos e sobrinhos preto 
    if (getCor(atual->pai) == PRETO &&
        getCor(getIrmao(atual)) == PRETO &&
        getCor(getIrmao(atual)->dir) == PRETO &&
        getCor(getIrmao(atual)->esq) == PRETO) {
        
        printf("inicio caso 3\n");
        getIrmao(atual)->cor = VERMELHO;
        atual->pai->cor = DUPLO_PRETO;

        remove_duplo_preto(raiz, atual);

        balancearRemocao(raiz, atual->pai);

        printf("fim caso 3\n");
        return;
    }

    //caso 4, pai vermelho, irmão e sobrinhos preto
    if (getCor(atual->pai) == VERMELHO &&
        getCor(getIrmao(atual)) == PRETO &&
        getCor(getIrmao(atual)->dir) == PRETO &&    
        getCor(getIrmao(atual)->esq) == PRETO) {
        printf("inicio caso 4\n");

        atual->pai->cor = PRETO;
        getIrmao(atual)->cor = VERMELHO;

        remove_duplo_preto(raiz, atual);
        printf("fim caso 4\n");
        return;
    }

    //caso 5a
    if (eh_filho_esquerdo(atual) &&
        getCor(getIrmao(atual)) == PRETO &&
        getCor(getIrmao(atual)->esq) == VERMELHO &&
        getCor(getIrmao(atual)->dir) == PRETO) {

        printf("caso 5a part1 \n");
        giroDireita(raiz, atual->pai->dir);
        atual->pai->dir->cor = PRETO;
        atual->pai->dir->dir->cor = VERMELHO;

        balancearRemocao(raiz, atual);
        printf("caso 5a\n");
        return;
    }

    //caso 5b: https://imgur.com/Gzej4sO
    if (eh_filho_direito(atual) &&
        getCor(getIrmao(atual)) == PRETO &&
        getCor(getIrmao(atual)->dir) == VERMELHO &&
        getCor(getIrmao(atual)->esq) == PRETO) {

        printf("caso 5b inicio \n");
        giroEsquerda(raiz, atual->pai->esq);

        atual->pai->esq->cor = PRETO;
        atual->pai->esq->esq->cor = VERMELHO;

        balancearRemocao(raiz, atual);
        printf("caso 5b fim\n");
        return;
    }

    //caso 6a
    if (eh_filho_esquerdo(atual) &&
        getCor(getIrmao(atual)) == PRETO &&
        getCor(getIrmao(atual)->dir) == VERMELHO) {

        printf("caso 6a inicio \n");
        giroEsquerda(raiz, atual->pai);

        //a folha roxa muda de cor com seu filho, que na linha seguinte vira preto
        atual->pai->pai->cor = atual->pai->cor;
        atual->pai->cor = PRETO;
        atual->pai->pai->dir->cor = PRETO;

        remove_duplo_preto(raiz, atual);
        printf("caso 6a fim\n");
        return;
    }

    //caso 6b https://imgur.com/a/G81j1PH
    if (eh_filho_direito(atual) &&
        getCor(getIrmao(atual)) == PRETO &&
        getCor(getIrmao(atual)->esq) == VERMELHO) {
        
        printf("caso 6b inicio \n");
        giroDireita(raiz, atual->pai);

        atual->pai->pai->cor = atual->pai->cor;
        atual->pai->cor = PRETO;
        atual->pai->pai->esq->cor = PRETO;

        remove_duplo_preto(raiz, atual);
        printf("caso 6b fim\n");
        return;
    }
}

void remover(Folha **raiz, int input) {
    Folha *atual;
    atual = *raiz;

    while(atual != NULL) {
        if (input == atual->valor) {
            //possui 2 filhos
            if (atual->esq != NULL && atual->dir != NULL) {
                //procura a folha com valor antecessor e troca os numeros entre si
                atual->valor = maior_elemento(atual->esq);
                //faz um segundo chamada "recursivo" da função passando
                //como argumento 
                remover(&(atual->esq), atual->valor);
                break;
            }
            //possui um filho direito
            if (atual->esq == NULL && atual->dir != NULL) {
                //troca a cor do filho e sobe ele para o lugar da folha removida
                atual->dir->cor = PRETO;
                //primeiro atualiza o ponteiro do filho que ira ficar no lugar de atual
                atual->dir->pai = atual->pai;

                if(eh_raiz(atual)) {
                    *raiz = atual->dir;
                } else {
                    //depois atualiza os ponteiros do pai do nó atual para receber o neto (filho de atual)
                    if (eh_filho_esquerdo(atual)) {
                        atual->pai->esq = atual->dir;
                    } else {
                        atual->pai->dir = atual->dir;
                    }
                }

                break;
            }
       
            //possui um filho esquerdo
            if (atual->esq != NULL && atual->dir == NULL) {
                atual->esq->cor = PRETO;
                atual->esq->pai = atual->pai;

                if (eh_raiz(atual)) {
                    *raiz = atual->dir;
                } else {
                    if (eh_filho_direito(atual)) {
                        atual->pai->dir = atual->esq;
                    } else {
                        atual->pai->esq = atual->esq;
                    }
                }
                break;
            }

            //não possui filho algum
            if (atual->esq == NULL && atual->dir == NULL) {
                //se a folha for a raiz então apenas remove ela
                if(eh_raiz(atual)) {
                    *raiz = NULL;
                    break;
                }

                //se a folha for vermelha, tambem apenas remove ela só que faz isso 
                //atualizando o ponteiro do pai
                if (atual->cor == VERMELHO) {
                    if (eh_filho_esquerdo(atual)) {
                        atual->pai->esq = NULL;
                    } else {
                        atual->pai->dir = NULL;
                    }
                    break;

                } else {
                    
                    //para entender a partir daqui, é necessario ver a logica por trás neste video:
                    //https://youtu.be/X3JW72nmbyw?si=-y87mQNAW4zqM2FO&t=2503 
                    //no_null->cor = DUPLO_PRETO;
                    //se a folha for preta é necessario recolorir ela para um duplo preto
                    no_null->pai = atual->pai;

                    if(eh_filho_esquerdo(atual)) {
                        atual->pai->esq = no_null;
                    } else {
                        atual->pai->dir = no_null;
                    }
                    //quando existe um nó duplo preto é necessario uma função de ajuste completamente 
                    //dedicada a cuidar dos casos de correção para tal
                    balancearRemocao(raiz, no_null);
                    break;
                }
            }
        }
        if (input > atual->valor) {
            atual = atual->dir;
        } else {
            atual = atual->esq;
        }
    }
}
