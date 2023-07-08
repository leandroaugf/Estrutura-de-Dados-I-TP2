#include "labirinto.h"

#include <stdio.h>
#include <stdlib.h>

/*TAD que armazena o labirinto e suas caracteristicas*/
struct labirinto{

  char **lab;
  int lins;
  int cols;

};

/*TAD que armazena as coordenadas de posicao do labirinto*/
struct posicao{

  int x;
  int y;

};

/*TAD que armazena o tamanho do percurso e as posicoes ate a saida*/
struct percurso{

  Posicao** posicao_saida;
  int tamanho;

};



// Funcao que imprime pontilhados ate o final do lab;
void printLabirinto(Labirinto *labirinto, Posicao *posicao) {
  for (int i = 0; i < labirinto->lins; i++) {
    for (int j = 0; j < labirinto->cols; j++) {
      
      // Volta a colocar o 'M' na posicao inicial ja que a funcao acharSaida o substitui por pontilhado
      if(i == posicao->x && j == posicao->y){
        labirinto->lab[i][j] = 'M';
      }
      printf("%c", labirinto->lab[i][j]);
    }
    printf("\n");
  }
}

/*Funcao que aloca a TAD labirinto e designa o valor das linhas e colunas*/
Labirinto* alocarLabirinto(int lins, int cols) {
  Labirinto* labirinto = malloc(sizeof(Labirinto));
  
  labirinto->lins = lins;
  labirinto->cols = cols;

  labirinto->lab = malloc(lins * sizeof(char*));
  for(int i = 0; i < lins; i++){
    labirinto->lab[i] = malloc(cols * sizeof(char));
  }

  return labirinto;
}

/*Funcao que aloca a TAD posicao e coloca os valores de x e y na TAD*/
Posicao* designaPosicao(int x, int y){
  Posicao* posicao = malloc(sizeof(Posicao));

  posicao->x = x;
  posicao->y = y;

  return posicao;
}

// Funcao que retorna a quantidade de passos ate o final do labirinto
int contador(Labirinto* labirinto){

    int contador = 0;

    /*Percorre o labirinto resolvido e conta onde entao os pontilhados, 
    caminho para a saida*/
    for(int i = 0; i < labirinto->lins; i++){
        for(int j = 0; j < labirinto->cols; j++){
            if(labirinto->lab[i][j] == '.' || labirinto->lab[i][j] == 'o'){
                contador++;
            }        
        }
    }
    /*Como a funcao 'acharSaida' preenche a primeira posicao com o '.', é preciso retornar contador - 1*/
    return contador - 1;
}

/*Funaco que aloca a TAD percurso*/
Percurso* alocarPercurso(Labirinto* labirinto, int x, int y){
  Percurso* percurso = malloc(sizeof(Percurso));

  /*Chama a funcao contador e armazena o resultado na TAD percurso*/
  percurso->tamanho = contador(labirinto);

  /*Alocacao da posicao_saida a partir da funcao que aloca posicao*/
  if(percurso->tamanho > 0){
    percurso->posicao_saida = (Posicao**)malloc(percurso->tamanho * sizeof(Posicao*));
  } else {
    /*Caso o cotador seja negativo, labirinto sem solucao, nao tem necessidade de alocar o 
    tamanho das posicoes*/
    percurso->posicao_saida = (Posicao**)malloc(sizeof(Posicao*));
  }

  for(int i = 0; i < percurso->tamanho; i++){
    percurso->posicao_saida[i] = designaPosicao(x, y);
  }

  return percurso;

}

/*Funcao que designa as coordenadas ate a saida para a TAD percurso*/
void designaCoordenadas(Labirinto* labirinto, Percurso* percurso, Posicao* posicao){

    /*Inicializa as coordenadas com valor invalido (-1,-1), para usar de paremetro de troca*/
    for(int k = 0; k < percurso->tamanho; k++){
      percurso->posicao_saida[k]->x = -1;  
      percurso->posicao_saida[k]->y = -1;
    }

    /*Coloca o 'M' na posicao inicial, pois a funcao 'acharSaida' o substitui por '.'*/
    for(int i = 0; i < labirinto->lins; i++) {
      for(int j = 0; j < labirinto->cols; j++) {
        if(i == posicao->x && j == posicao->y){
        labirinto->lab[i][j] = 'M';
        }

        /*Coloca as coordenadas ate a saida na TAD percurso*/
        if(labirinto->lab[i][j] == 'o') {
          for(int k = 0; k < percurso->tamanho; k++){
            if(percurso->posicao_saida[k]->x == -1 && percurso->posicao_saida[k]->y == -1){
              percurso->posicao_saida[k]->x = i;
              percurso->posicao_saida[k]->y = j;
              break;
            }
          }
        }
      }
    }
}

/*Imprime as coordenadas ate a saida*/
void printCoordenadas(Percurso* percurso){
  /*Condicao para nao imprimir se o labirinto nao tiver saida*/
  if(percurso->tamanho >= 0){  
    printf("Coordenadas ate a saida: \n");
    for(int i = 0; i < percurso->tamanho; i++){
      printf("%d, %d\n", percurso->posicao_saida[i]->x, percurso->posicao_saida[i]->y);
    }
  }
}

// Funcao para achar o menor caminho atraves de recursao
int acharSaida(Labirinto* labirinto, Posicao* posicao){
  int x = posicao->x;
  int y = posicao->y;  
  
  // Verifica se está fora dos limites do labirinto ou encontrou um obstáculo
  if(x < 0 || x >= labirinto->lins || y < 0 || y >= labirinto->cols || labirinto->lab[x][y] == '*' || labirinto->lab[x][y] == 'o' || labirinto->lab[x][y] == '#'){
    return 0;
  }

  // Verifica se chegou a uma borda do labirinto
  if(x == 0 || x == labirinto->lins - 1 || y == 0 || y == labirinto->cols - 1){
    labirinto->lab[x][y] = 'o';
    return 1;
  }

  // Marca a posicao atual como visitada
  labirinto->lab[x][y] = 'o';

  /*Foi necessario alocar uma variavel auxiliar, para mudar os valores de x e y e
  passar de parametro na recursao*/
  Posicao* nova_posicao;
  nova_posicao = designaPosicao(x, y);

  // Tenta mover para cima
  nova_posicao->x = x-1;
  nova_posicao->y = y;
  if (acharSaida(labirinto, nova_posicao)){
    /*Foi necessario desalocar apos cada recursao para nao dar leak de memoria*/
    desalocarPosicao(&nova_posicao);
    return 1;
  }

  // Tenta mover pra baixo
  nova_posicao->x = x+1;
  nova_posicao->y = y;
  if(acharSaida(labirinto, nova_posicao)){
    desalocarPosicao(&nova_posicao);
    return 1;
  }

  // Tenta mover pra esquerda
  nova_posicao->x = x;
  nova_posicao->y = y-1;
  if(acharSaida(labirinto, nova_posicao)){
    desalocarPosicao(&nova_posicao);
    return 1;
  }

  // Tenta mover pra direita
  nova_posicao->x = x;
  nova_posicao->y = y+1;
  if(acharSaida(labirinto, nova_posicao)){
    desalocarPosicao(&nova_posicao);
    return 1;
  }

  // Marca atual posicao como nao visitada (backtracking)
  if(!acharSaida(labirinto, posicao)){
    labirinto->lab[x][y] = ' ';
  }

  //Desaloca geral a variavel
  desalocarPosicao(&nova_posicao);

  return 0;
  
}

void passosAteSaida(Percurso* percurso){
  
  /*Atraves de testes, foi possivel perceber que o contador de passos retorna negativo 
  quando o labirinto nao tiver saida, com isso, foi feito uma condicao para saber se o 
  labirinto tem resultado*/
  //if(percurso->tamanho < 0){
    //printf("EPIC FAIL!\n"); 
    //return;
  //} else {
    printf("%d", percurso->tamanho);
  //}
}

// Funcao para desalocar memoria do TAD labirinto

// Funcao pra desalocar TAD posicao
void desalocarPosicao(Posicao** posicao){
  
  free(*posicao);

}

// Funcao pra desalocar TAD percurso
void desalocarPercurso(Percurso** percurso) {
   
  for(int i = 0; i < (*percurso)->tamanho; i++){
    desalocarPosicao(&(*percurso)->posicao_saida[i]);
  }
  
  free((*percurso)->posicao_saida);
  free(*percurso);
}