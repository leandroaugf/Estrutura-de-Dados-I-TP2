#include "fila.h"
#include "labirinto.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*TAD que armazena o labirinto e suas caracteristicas*/
struct labirinto{

  char **lab;
  int lins;
  int cols;
};

// Verifica se a coordenada indicada nao ultrapassa os limites do labirinto
bool coordenadaEhValida(int row, int col, int lins, int cols){
    return (row >= 0 && row < lins && col >= 0 && col < cols);
}

// Verifica se uma coordenada representa a saida do labirinto
bool ehSaida(int row, int col, int lins, int cols) {
    return (row == 0 || row == lins - 1 || col == 0 || col == cols - 1);
}

// Verifica se o caminho é permitido
bool caminhoVazio(char** lab, int row, int col){
    return (lab[row][col] == ' ');
}

// Inicializa a fila vazia (Padrao)
void inicializaFila(Fila* fila){
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
}

// Verifica se a fila esta vazia atravez do seu tamanho
bool filaVazia(Fila* fila){
    return (fila->tamanho == 0);
}

// Insere um elemento na fila
void inserirFila(Fila* fila, Coordenadas coordenada){
    Celula* novaCelula = (Celula*)malloc(sizeof(Celula));
    novaCelula->coordenada = coordenada;
    novaCelula->prox = NULL;

    if(fila->inicio == NULL) {
        fila->inicio = novaCelula;
        fila->fim = novaCelula;
    } else {
        fila->fim->prox = novaCelula;
        fila->fim = novaCelula;
    }

    fila->tamanho++;
}

// Remove um elemento da fila
Coordenadas removerFila(Fila* fila) {
    if(filaVazia(fila)) {
        Coordenadas coordenadaVazia = {-1, -1};
        return coordenadaVazia;
    }

    Celula* celulaRemovida = fila->inicio;
    Coordenadas coordenadaRemovida = celulaRemovida->coordenada;

    fila->inicio = fila->inicio->prox;
    fila->tamanho--;

    free(celulaRemovida);
    return coordenadaRemovida;
}

// Marca caminho percorrido no labirinto com pontilhado
void marcarCaminho(Labirinto* labirinto, Coordenadas* caminho, int tamanhoCaminho){

    char** lab = labirinto->lab;

    for(int i = 0; i < tamanhoCaminho; i++){
        int row = caminho[i].x;
        int col = caminho[i].y;
        lab[row][col] = 'o';
    }
}

// Executa a busca em largura para encontrar a saida do labirinto
bool acharSaida_f(Labirinto* labirinto) {
    
    // Atribui as caracteristicas do labirinto em variavis auxiliares 
    char** lab = labirinto->lab;
    int lins = labirinto->lins;
    int cols = labirinto->cols;

    /*Inicializa uma matriz 'visitado' que é usada para restrear se uma determinada
    posicao no labirinto ja foi visitada*/
    bool visitado[lins][cols];
    for(int i = 0; i < lins; i++){
        for(int j = 0;j < cols; j++){
            visitado[i][j] = false;
        }
    }

    // Inicializa a fila
    Fila fila;
    inicializaFila(&fila);

    // Encontrar posicao inicial 'M' no labirinto e armazena na variavel 'Start'
    Coordenadas start;
    // Inicializa com valor que nao se encaixa como posicao do labirinto, valor < 0;
    start.x = -1;
    start.y = -1;
    for(int i = 0; i < lins; i++){
        for(int j = 0; j < cols; j++){
            if(lab[i][j] == 'M'){
                start.x = i;
                start.y = j;
                break;
            }
        }
    }

    /*Funcao inserir fila adiciona as coordenadas de inicio do labirinto ao
    final da fila*/
    inserirFila(&fila, start);
    /*Atribui o valor 'true' a posicao 'start' indicando que essa coordenada ja foi
    visitada*/
    visitado[start.x][start.y] = true;


    /*Declaracao de array que sera usado para armazenar as coordenadas percorridas 
    durante a busca em largura*/
    const int MAX_CAMINHO = lins * cols;
    Coordenadas caminho[MAX_CAMINHO];
    // Declaracao da variavel que acompanha o tamanho do caminho
    int tamanhoCaminho = 0;

    /*Continua explorando as coordenadas vizinhas ate a fila estar vazia, ou seja ate
    que todas as coordenadas sejam visitadas*/
    while (!filaVazia(&fila)){
        
        // Remove a proxima coordenada a ser explorada, a atual;
        Coordenadas atual = removerFila(&fila);
        /*Coordenadas removidas sao armazenadas na variavel atual, para obter
        suas coordenadas de linha e coluna*/
        int row = atual.x;
        int col = atual.y;

        /*Verifica se a coordenada atual corresponde a saida do labirinto*/
        if(ehSaida(row, col, lins, cols)){
            /*Se for a saida, o caminho ate o final é marcado*/
            marcarCaminho(labirinto, caminho, tamanhoCaminho-1);
            // Funcao retorna true indicando que a saida foi encontrada
            return true;
        }

        /*Cria um array de coordenadas que contem as coordenadas vizinhas da coordenada
        atual, representam os movimentos possiveis a partir da coordenada atual*/
        Coordenadas vizinhos[4] = {
            {row, col + 1},  // Direita
            {row + 1, col},  // Baixo
            {row, col - 1},  // Esquerda
            {row - 1, col}   // Cima

        };

        // Percorre as 4 coordenadas vizinhas
        for(int i = 0; i < 4; i++){
            // Obtem as coordenadas da vizinhanca atual que serao exploradas
            int novaLinha = vizinhos[i].x;
            int novaColuna = vizinhos[i].y;

            /*Verifica se a coordenada vizinha é valida, ainda nao foi visitada e representa
            um caminho vazio*/
            if(coordenadaEhValida(novaLinha, novaColuna, lins, cols) && !visitado[novaLinha][novaColuna] && caminhoVazio(lab, novaLinha, novaColuna)){
                // Marca a coordenada vizinha como visitada;
                visitado[novaLinha][novaColuna] = true;
                // Cria uma nova struct coordenada com as coordenadas atuais
                Coordenadas novaCoordenada = {novaLinha, novaColuna};
                // Insere a nova coordenada na fila, pra ser explorada depois;
                inserirFila(&fila, novaCoordenada);
                /*Adiciona a nova coordenada ao caminho percorrido e incrementa
                o tamanho do caminho em +1*/
                caminho[tamanhoCaminho] = novaCoordenada;
                tamanhoCaminho++;
            }
        }
    }

    // Se a saida nao for encontrada o caminho da tentativa é marcado
    marcarCaminho(labirinto, caminho, tamanhoCaminho);
    // Retorna false se o labirinto nao tiver saida
    return false;

}

// Funcao que imprime pontilhados ate o final do lab;
void printLabirinto_f(Labirinto *labirinto) {
  for (int i = 0; i < labirinto->lins; i++) {
    for (int j = 0; j < labirinto->cols; j++) {
      printf("%c", labirinto->lab[i][j]);
    }
    printf("\n");
  }
}

// Funcao que retorna a quantidade de passos ate o final do labirinto
int contador_f(Labirinto* labirinto){

    int contador = 0;

    /*Percorre o labirinto resolvido e conta onde entao os pontilhados, 
    caminho para a saida*/
    for(int i = 0; i < labirinto->lins; i++){
        for(int j = 0; j < labirinto->cols; j++){
            if(labirinto->lab[i][j] == 'o'){
                contador++;
            }        
        }
    }
    return contador;
}
