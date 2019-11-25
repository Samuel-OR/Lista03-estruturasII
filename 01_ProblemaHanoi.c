#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <sys/time.h>
#define discos 4
#define pinos 3

typedef struct{
	int nro_vertices;
	int** arestas;
	int eh_ponderado;
	float** pesos;
	int* grau;
	int grau_max;
	int nivel[81];
}Grafo;

typedef struct fila Fila;
struct fila{
    int vertice;
    struct fila *prox;    
};

//COLETAR TEMPO EM MICROSEGUNDOS
long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

//Funçõs para criar o grafo e gerar os estados da torre de hanói
Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado){
	Grafo *gr;
	gr = (Grafo*) malloc(sizeof(Grafo));

	if( gr != NULL){
		int i;
		gr->nro_vertices = nro_vertices;
		gr->eh_ponderado = (eh_ponderado != 0) ? 1:0;

		gr->grau=(int*)calloc(nro_vertices, sizeof(int));
		gr->arestas = (int**)malloc(nro_vertices*sizeof(int*));
		for(i=0; i<nro_vertices; i++){
			gr->arestas[i] = (int*)malloc(grau_max*sizeof(int*));
			for(int j=0; j<grau_max; j++){
				gr->arestas[i][j] = 0;
			}
		}
		if(gr->eh_ponderado == 1){
			gr->pesos = (float**)malloc(nro_vertices*sizeof(float*));
			for(int j=0; j<nro_vertices; j++){
				gr->pesos[j] = (float*)malloc(nro_vertices*sizeof(float));
			}
		}
		
	}
	return gr;
}
int **gerarPossibilidades(int possibilidades){
	int cont=0;
	int **estadoTorre = malloc(sizeof(int*)*possibilidades);;
	for(int x=0;x< pinos ;x++){
		for(int y=0;y< pinos;y++){
			for(int z=0;z<pinos;z++){
				for(int w=0;w<pinos; w++){
					estadoTorre[cont] =  malloc(sizeof(int)*discos);
					estadoTorre[cont][0] = x+1;
					estadoTorre[cont][1] = y+1;
					estadoTorre[cont][2] = z+1;
					estadoTorre[cont][3] = w+1;
					cont++;
				}
			}
		}
	}
	return estadoTorre;
}


//Funções para validar uma adjacência.
int qtdMovimentos(int* estadoTorre1, int* estadoTorre2){
	int cont = 0;
	for (int x=0; x < discos; x++){
		if(estadoTorre2[x] != estadoTorre1[x])
			cont++;
	}
	return cont;
}
int posMovimento(int* estadoTorre1, int* estadoTorre2){
	int pos = -1;
	int cont = 0;
	for (int x=0; x < discos; x++){
		if(estadoTorre2[x] != estadoTorre1[x]){
			cont++;
			pos = x;
		}
	}
	if(cont!=1)
		cont = -1;
	return pos;
}
int discoMenorAcima(int* estadoTorre, int pos){
	int resul = 1;
	for (int x=pos+1; x < discos; x++){
		if(estadoTorre[x] == estadoTorre[pos])
			resul = 0;
	}
	return resul;
}
int compararEstadoTorre(int* estadoTorre1, int* estadoTorre2){
	int resul = 0;
	if( qtdMovimentos(estadoTorre1, estadoTorre2)==1 ){
		int pos = posMovimento(estadoTorre1, estadoTorre2);
		if(  pos != -1){
			if(discoMenorAcima(estadoTorre1, pos) == 1){
				if(discoMenorAcima(estadoTorre2, pos) == 1){
					resul = 1;
				}

			}
		}
	}
	return resul;
}


//Funções para construir o grafo.
int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso){
	int resul = 0;
	if(gr != NULL){
		if(( orig >= 0 && orig <= gr->nro_vertices) && (dest > 0 && dest <= gr->nro_vertices)){		
			gr->arestas[orig-1][gr->grau[orig-1]] = dest;
			if(gr->eh_ponderado)
				gr->pesos[orig-1][gr->grau[orig-1]] = peso;
			gr->grau[orig-1]++;

			if(eh_digrafo == 0)
				insereAresta(gr,dest,orig,1,peso);
			resul = 1;
		}
	}
	return resul;
}
void construirGrafo(Grafo *grafo, int **estadoTorre, int possibilidades){
	int aux =0 ;
	for(int x = 0; x<possibilidades; x++){
		for(int y = 0; y<possibilidades; y++){
			if(compararEstadoTorre(estadoTorre[x], estadoTorre[y]) == 1){
				insereAresta(grafo, x+1, y+1, 1, 0);
			}
		}

	}
}

//Funções para colocar nivel nos vertices do grafo.
Fila *appendFila(Fila *F, int raiz){
    Fila *novo, *aux;
    novo = (Fila*)malloc(sizeof(Fila));
    novo->vertice = raiz;
    if(F==NULL){
        F = novo;
        novo->prox = NULL;
    }else{
        for(aux=F; aux->prox!=NULL; aux=aux->prox);
        aux->prox=novo;
        novo->prox = NULL;
    }
    return F;
}
Fila *deleteFila(Fila **F){
    Fila *aux;
    aux = (*F);
    (*F) = (*F)->prox;
    return aux;
}

void dijskstra(Grafo *gr, int orig, int* dist, int* predecessor){
    
    Fila *visitar;
    visitar = NULL;
    dist[orig-1] = 0;
    visitar = appendFila(visitar, orig);

    while(visitar != NULL){
        Fila *aux; 
        aux = deleteFila(&visitar);
        for(int i = 0; i < gr->grau[aux->vertice-1]; i++){
            int aresta_i = gr->arestas[aux->vertice-1][i] - 1;
            if(dist[aresta_i] > dist[aux->vertice-1] + gr->pesos[aux->vertice-1][i]){  
                dist[aresta_i] = (dist[aux->vertice-1]) + gr->pesos[aux->vertice-1][i];
                predecessor[aresta_i] = aux->vertice;
                visitar = appendFila(visitar, gr->arestas[aux->vertice-1][i]);
            }
        }
    }
}
void chamar_dijkstra(Grafo *gr, int dest){
	int *dist = (int*) calloc(81, sizeof(int));
	int *predecessor = (int*) calloc(81, sizeof(int));
   
    for(int i = 0; i <= 81; i++){
    	dist[i] = INT_MAX/2;
    	predecessor[i] = -1;
    }
 
	dijskstra(gr, 81, dist, predecessor);

    printf("%d ", dest);
    int final = dest, cont=0;
    for(int i = 0; final != -1; i++){
    	if(predecessor[final-1] != -1)
    		printf("%d ", predecessor[final-1]);
        final = predecessor[final-1];
        cont++;
    }   
	printf("|= %d\n", cont-1);
}

//Converter entrada do usuario
int equivalente(int **estadoTorre, int *vet){
	int resul = 0;
	for(int i=0; i<81; i++){
        if(estadoTorre[i][0] == vet[0] && estadoTorre[i][1] == vet[1] && 
           estadoTorre[i][2] == vet[2] && estadoTorre[i][3] == vet[3]){
            resul = i;
        }
    }
    return resul;
}


int main(){
 	
 	struct timeval tempo_inicial, tempo_final;
	float tempo_total;

	int possibilidades = pow(pinos,discos);
	
	int **estadoTorre = gerarPossibilidades(possibilidades);

 	Grafo *grafo = cria_Grafo(possibilidades,pinos, 1);
	
	int *visitados = (int*) calloc(possibilidades+1, sizeof(int));
	
	construirGrafo(grafo, estadoTorre, possibilidades);

	int vet[4];
	printf("Digite  posição de inicio (Ex: [1 1 1 1]): ");
	scanf("%d %d %d %d", &vet[0], &vet[1], &vet[2], &vet[3]);
	
	int inicio = equivalente(estadoTorre,vet);
	printf("Inicio: %d (%d)\n",inicio+1,inicio );
	
	gettimeofday(&tempo_inicial, NULL);
	
	chamar_dijkstra(grafo, inicio+1);
	
	gettimeofday(&tempo_final, NULL);
	tempo_total = (tempo_final.tv_sec - tempo_inicial.tv_sec) * (int)1e6 + (tempo_final.tv_usec - tempo_inicial.tv_usec);
	printf("TEMPO TOTAL: %.3f microsegundos\n", tempo_total);

	return 0;
}