#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

typedef struct labirinto Labirinto;

typedef struct{
    int x;
    int y;
} Coordenadas;

typedef struct celula{
    Coordenadas coordenada;
    struct celula *prox;
} Celula;

typedef struct {
    Celula *inicio;
    Celula *fim;
    int tamanho;
} Fila;

int contador_f(Labirinto*);
bool acharSaida_f(Labirinto* );
void printLabirinto_f(Labirinto*);

#endif //FILA_H