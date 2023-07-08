#ifndef LABIRINTO_H
#define LABIRINTO_H

typedef struct posicao Posicao;
typedef struct percurso Percurso;
typedef struct labirinto Labirinto;

//Funcoes para alocar as TADS
Labirinto* alocarLabirinto(int, int);
Posicao* designaPosicao(int, int);      
Percurso* alocarPercurso(Labirinto*, int, int); 

//Funcoes para resolucao do labirinto
int acharSaida(Labirinto*, Posicao*);   
void passosAteSaida(Percurso*);       
void designaCoordenadas(Labirinto*, Percurso*, Posicao*);  
int contador(Labirinto*);

//Funcoes para printar resultados
void printCoordenadas(Percurso*);  
void printLabirinto(Labirinto*, Posicao*);     

//Funcoes para desalocar as TADs    
void desalocarPosicao(Posicao**);        
void desalocarPercurso(Percurso**);      


#endif // LABIRINTO_H