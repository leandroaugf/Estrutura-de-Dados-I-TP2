#include "labirinto.h"
#include "fila.h"
#include "pilha.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main(void) {

    //Variaveis pra contagem de tempo
    double tempo_exec;
    clock_t inicio, fim;

    inicio = clock();

    srand(time(NULL));

    // Declaracao de variaveis;
    Labirinto *labirinto;
    Posicao pos2;
    Posicao *posicao;
    Percurso *percurso;
    int x, y;  //Coordenadas iniciais
    char opcao;
    int lins, cols;

    scanf("%d %d", &lins, &cols);
    labirinto = alocarLabirinto(lins, cols);   // Alocacao do labirinto
    getchar();
    scanf("%c", &opcao);
    getchar();

    switch (opcao)
    {   
        // Imprime o labirinto com os pontilhados;
        case 'r':
          leLabirinto(labirinto);
          pos2 = primeiraPosicao(labirinto);
          x = pos2.x; y = pos2.y;
          posicao = designaPosicao(x, y);
          acharSaida(labirinto, posicao);
          percurso = alocarPercurso(labirinto, x, y);
          passosAteSaida(percurso);
          //int aux = contador(labirinto);
          
            printf("\n");
            printLabirinto(labirinto, posicao);
          
        break;

        case 'f':
          leLabirinto(labirinto);
          acharSaida_f(labirinto);
          int cont = contador_f(labirinto);
          printf("\n%d\n", cont);
          printLabirinto_f(labirinto);
        
        break;

        case 'p':
          leLabirinto(labirinto);
          acharSaidaPilha(labirinto);
        break;
    }
  
  //Desaloca TADs
  desalocarLabirinto(&labirinto);
  //Percurso e Posicao sao usados apenas na opcao r
  if(opcao == 'r'){
    desalocarPercurso(&percurso);
    desalocarPosicao(&posicao);
  }

  //Imprime tempo 
  fim = clock();
  tempo_exec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("\nTempo de execucao: %fs\n", tempo_exec);

  return 0;

}