#ifndef PILHA_H
#define PILHA_H

typedef struct pilha Pilha;
typedef struct posicao Posicao;
typedef struct labirinto Labirinto;

struct labirinto {
    int lins;
    int cols;
    char **lab;
};

struct posicao {  
    int x;
    int y;
};

typedef struct celula2 {
    Posicao item;
    struct celula2 *prox;
} Celula2;

struct pilha {
    int tamanho;
    Celula2 *topo;
    Celula2 *cabeca;
};




// [ALOCAÇÃO x INICIALIZAÇÃO]
int pilhaEhVazia(Pilha*);
void iniciaPilha(Pilha*);
Labirinto *alocarLab(int, int);
Posicao primeiraPosicao(Labirinto*);


// [OPERAÇÕES DO LABIRINTO]
void vitoria(Labirinto *);
void derrota(Labirinto *);
void printLab(Labirinto *);
void leLabirinto(Labirinto *);


// [OPERAÇÕES DA PILHA]
Posicao pilhaPop(Pilha*);
int contapassos(Labirinto*);
int acharSaidaPilha(Labirinto*);
void pilhaPush(Pilha*, Posicao);

// [OPERAÇÕES DA FILA]

// [DESALOCAÇÃO x FIM]
void desalocarPilha(Pilha*);
void desalocaVisitado(Labirinto*);
void desalocarLabirinto(Labirinto**);            

#endif // LABIRINTO_H