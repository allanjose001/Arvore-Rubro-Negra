#include <stdio.h>
#include <stdlib.h>
#include "arvoreRB.h"

int main() {
    int opcao, input;
    Folha *raiz = NULL;
    Folha *no_null = NULL;
    iniciaNull(&no_null);
    
    inserir(&raiz, 30);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 20);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 40);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 10);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 25);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 35);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 50);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 5);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 15);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 22);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 27);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 33);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 37);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 45);
    preOrder(raiz);
    printf("\n");
    inserir(&raiz, 55);
    preOrder(raiz);
    printf("\n");
    //testes de remoção de duplo preto
    remover(&raiz, 10);
    preOrder(raiz);
    printf("\n");
    //caso 4
    remover(&raiz, 5);
    preOrder(raiz);
    printf("\n");
    //caso 6
    remover(&raiz, 15);
    preOrder(raiz);
    printf("\n");
    //caso 5 e depois caso 6
    remover(&raiz, 27);
    preOrder(raiz);
    printf("\n");
    //caso 4
    remover(&raiz, 20);
    preOrder(raiz);

    /*do {
        printf("0 - sair\n1 - inserir\n2 - remover\n3 - imprimir PreOrder\n4 - imprimir inOrder\n");
        scanf("%d", &opcao);

        switch(opcao) {
            case 0:
                break;
            case 1:
                printf("\ndigite o numero a ser inserido: ");
                scanf("%d", &input);
                inserir(&raiz, input);
                break;
            case 2:
                printf("\ndigite o numero a ser removido: ");
                scanf("%d", &input);
                remover(&raiz, input);
                break;
            case 3:
                printf("\nImpressao Pre Order\n");
                preOrder(raiz);
                printf("\n");
                break;
            case 4:
                printf("\nimpressao in order\n");
                inOrder(raiz);
                printf("\n");
                break;
            default:
                printf("\nopcao invalida\n");
        }
    } while (opcao != 0);
*/
    return 0;
}