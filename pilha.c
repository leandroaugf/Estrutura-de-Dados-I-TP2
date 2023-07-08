#include "pilha.h"
#include "labirinto.h"

#include <stdio.h>
#include <stdlib.h>

// ================================================================================= -[INIT]- ==============================================================================================

Posicao primeiraPosicao(Labirinto *labirinto) {
    Posicao firstpos;
    for (int i = 0; i < labirinto->lins; i++) {
        for(int j = 0; j < labirinto->cols; j++) {
            if (labirinto->lab[i][j] == 'M') {
                firstpos.x = i;
                firstpos.y = j;
            }
        }
    }

    return firstpos;
}

void leLabirinto(Labirinto *labirinto) {

  for (int i = 0; i < labirinto->lins; i++) {
    for (int j = 0; j < labirinto->cols; j++) {
      scanf("%c", &labirinto->lab[i][j]);
    }
    // Getchar para ler os espacos no labirinto 
    getchar();
  }
}

Labirinto *alocarLab(int lins, int cols) {
    Labirinto *labirinto = malloc(sizeof(Labirinto));
    labirinto->lins = lins; labirinto->cols = cols;
    labirinto->lab = malloc(lins * sizeof(char*));
    for (int i = 0; i < lins; i++) 
        labirinto->lab[i] = malloc(cols * sizeof(char)); 
    
    return labirinto;
}

// ================================================================================ [ OPERAÇÕES: PILHA ] ===================================================================================
//                                                                                                                                                                    BUSCA EM PROFUNDIDADE 

int caminhoValido(Labirinto *labirinto, int lins, int cols) {
    return (labirinto->lab[lins][cols] == ' ');
}

int acharSaidaPilha(Labirinto *labirinto) {
    int visitado[labirinto->lins][labirinto->cols];
    Pilha *pilha = (Pilha*) malloc(sizeof(Pilha));
    iniciaPilha(pilha);
    Posicao pos = primeiraPosicao(labirinto);
    Posicao posaux = primeiraPosicao(labirinto);
    
    // marcando tudo como nao visitado
    for (int i = 0; i < labirinto->lins; i++)
        for (int j = 0; j < labirinto->cols; j++)
            if (labirinto->lab[i][j] == ' ')
                visitado[i][j] = 0;
    pilhaPush(pilha, pos);

    while(!pilhaEhVazia(pilha)) {
        visitado[pos.x][pos.y] = 1;
        pos = pilhaPop(pilha);
        if (pos.x != posaux.x || pos.y != posaux.y)
            labirinto->lab[pos.x][pos.y] = 'o';
        // [WIN CONDITION]
        if((pos.x == labirinto->lins - 2) && (pos.y == labirinto->cols - 1)) {
            vitoria(labirinto);
            desalocarPilha(pilha);
            return 1;
        }
        
        // [DIREITA]
        else if (!(visitado[pos.x][pos.y + 1]) && (caminhoValido(labirinto, pos.x, pos.y + 1))) {
            pos.y++; 
            pilhaPush(pilha, pos);
        }
        // [BAIXO]
        else if (!(visitado[pos.x + 1][pos.y]) && (caminhoValido(labirinto, pos.x + 1, pos.y))) {
            pos.x++; 
            pilhaPush(pilha, pos);
        }
        // [ESQUERDA]
        else if (!(visitado[pos.x][pos.y - 1]) && (caminhoValido(labirinto, pos.x, pos.y - 1))) {
            pos.y--; 
            pilhaPush(pilha, pos);
        }
        // [CIMA]
        else if (!(visitado[pos.x - 1][pos.y]) && (caminhoValido(labirinto, pos.x - 1, pos.y))) {
            pos.x--; 
            pilhaPush(pilha, pos);
        }
    }

    printf("\nEPIC FAIL!\n\n");
    printLab(labirinto);
    printf("\n\nnúmero de passos: %d", contapassos(labirinto));
    desalocarPilha(pilha);
    return 0;
}

void iniciaPilha(Pilha *pilha) {
    pilha->cabeca = NULL;
    pilha->topo = NULL;
    pilha->tamanho = 0;
}

int pilhaEhVazia(Pilha *pilha) {
    return (pilha->tamanho == 0);
}


// [ADICIONA UM ELEMENTO AO TOPO] -- NOTA: "topo" não faz parte da lista, é como um clone;
void pilhaPush(Pilha *pilha, Posicao pos) {
    Celula2 *new = (Celula2*)malloc(sizeof(Celula2));
    if (new == NULL)
        printf("\nmemória insuficiente!!\n");

    new->item = pos;
    new->prox = NULL;
    pilha->tamanho++;

    if (pilha->cabeca == NULL) {
        pilha->topo = new;
        pilha->cabeca = new;
    } else {
        pilha->topo->prox = new;
        pilha->topo = new;
    }
}

// [RETIRA O ELEMENTO DO TOPO]
Posicao pilhaPop(Pilha *pilha) {
    Posicao pilhaVazia;
    pilhaVazia.x = -1; pilhaVazia.y = -1;
    if (pilhaEhVazia(pilha))
        return pilhaVazia;
    Celula2 *aux = pilha->cabeca;
    Posicao removed = pilha->topo->item;
        
    if (pilha->tamanho > 2) 
        while (aux->prox->prox != NULL) // para qdo estver uma posição anterior ao topo
            aux = aux->prox;
    
    pilha->tamanho--;
    free(pilha->topo);
    pilha->topo = aux;
    aux->prox = NULL;

    return removed;
}

int contapassos(Labirinto *labirinto) {
    int cont = 0;
    for (int i = 0; i < labirinto->lins; i++)
        for (int j = 0; j < labirinto->cols; j++)
            if(labirinto->lab[i][j] == 'o')
                cont++;

    return cont;
}

void printLab(Labirinto *labirinto) {
    for (int i = 0; i < labirinto->lins; i++) {
        for (int j = 0; j < labirinto->cols; j++) {
            printf("%c", labirinto->lab[i][j]);
        }
        printf("\n");
    }
}

void vitoria(Labirinto *labirinto) {
    printf("\nCaminho de Saída:\n\n");
    printLab(labirinto);
    printf("\nNúmero de passos: %d", contapassos(labirinto));
}

// =======================================================================
// [DESALOCAÇÃO DE MEMÓRIA] ==============================================
// =======================================================================

void desalocarPilha(Pilha *pilha) {
    Celula2 *aux = pilha->cabeca;
    Celula2 *aux2 = pilha->cabeca->prox;
    while(aux2 != NULL) {
        free(aux);
        aux = aux2;
        aux2 = aux2->prox;
    }

    free(pilha);
}

void desalocarLabirinto(Labirinto **labirinto) {
    for(int i = 0; i < (*labirinto)->lins; i++)
        free((*labirinto)->lab[i]);

    free((*labirinto)->lab);
    free(*labirinto);
}