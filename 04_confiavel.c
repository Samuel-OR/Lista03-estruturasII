#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct{
	int nro_vertices;
	int eh_ponderado;
	int *grau;
	int **arestas;
	float **pesos;
}Grafo;

typedef struct pi PILHA;
struct pi{
	int vertice;
	float arestaAnt;
	PILHA *prox, *inicio, *fim;
};

//struct para guardar os caminhos possíveis
typedef struct m_pilha Pilha;
struct m_pilha{
	int qtd_elementos;
	float confianca;
	PILHA *caminho;
};

//FUNÇÕES CRUD PILHA
PILHA *inserirPilha(PILHA *pilha, int vertice, float arestaAnt){
	PILHA *novo;
	novo = (PILHA *)malloc(sizeof(PILHA));
	novo->vertice = vertice;
	novo->arestaAnt = arestaAnt;
	if (pilha == NULL){
		novo->inicio = novo;
		novo->fim = novo;
		return novo;
	}
	novo->prox = pilha;
	novo->fim = pilha->fim;
	PILHA *aux = pilha;
	do{
		aux->inicio = novo;
		aux = aux->prox;
	} while (aux != NULL);
	return novo;
}
void mostrarPilha(PILHA *pilha){
	if (pilha->prox != NULL){
		mostrarPilha(pilha->prox);
		printf("%d ", pilha->vertice);
	}
}
PILHA *removerPilha(PILHA *pilha){
	if (pilha == NULL){
		return pilha;
	}
	PILHA *aux;
	for (aux = pilha; aux->prox != NULL; aux = aux->prox){
		aux->inicio = pilha->prox;
	}
	aux->inicio = pilha->prox;
	return pilha->prox;
}
void copiarPilha(PILHA **p1, PILHA **p2){
	PILHA *aux = (PILHA *)malloc(sizeof(PILHA));
	if (*p1 == NULL){
		*p2 = NULL;
	}else{
		copiarPilha(&(*p1)->prox, p2);
		aux->vertice = (*p1)->vertice;
		aux->prox = *p2;
		*p2 = aux;
	}
}


//FUNÇÕES UTILIZADAS PARA GERENCIAR GRAFO
Grafo *cria_Grafo(int nro_vertices, int eh_ponderado){
	Grafo *gr;
	gr = (Grafo *)malloc(sizeof(Grafo));

	if (gr != NULL){
		int i;
		gr->nro_vertices = nro_vertices;
		gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;
		gr->grau = (int *)calloc(nro_vertices, sizeof(int));

		gr->arestas = (int **)malloc(nro_vertices * sizeof(int *));
		for (i = 0; i < nro_vertices; i++){
			gr->arestas[i] = (int *)malloc(nro_vertices * sizeof(int));
		}
		if (gr->eh_ponderado == 1){
			gr->pesos = (float **)malloc(nro_vertices * sizeof(float *));
			for (int j = 0; j < nro_vertices; j++){
				gr->pesos[j] = (float *)malloc(nro_vertices * sizeof(float));
			}
		}
	}
	return gr;
}
int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso){
	int resul = 0;
	if (gr != NULL){
		if ((orig >= 0 && orig < gr->nro_vertices) || (dest >= 0 && dest < gr->nro_vertices)){
			gr->arestas[orig][gr->grau[orig]] = dest;
			if (gr->eh_ponderado)
				gr->pesos[orig][gr->grau[orig]] = peso;
			gr->grau[orig]++;
			if (eh_digrafo == 0)
				insereAresta(gr, dest, orig, 1, peso);
			resul = 1;
		}
	}
	return resul;
}
void liberar_Grafo(Grafo *gr){
	if (gr != NULL){
		int i;
		for (i = 0; i < gr->nro_vertices; i++)
			free(gr->arestas[i]);
		free(gr->arestas);

		if (gr->eh_ponderado == 1){
			for (i = 0; i < gr->nro_vertices; i++)
				free(gr->pesos[i]);
			free(gr->pesos);
		}
		free(gr->grau);
		free(gr);
	}
}
int cont=0;


//FUNÇÕES UTILIZADAS NA BUSCA EM PROFUNDIDADE
void buscaProfundidade(Grafo *gr, int orig, int dest, int *visitado, float confiancaTotal, PILHA *pilha, Pilha *maior_p){

	
	visitado[orig] = 1;
	pilha = inserirPilha(pilha, orig, 0);
	for (int i = 0; i < gr->grau[orig]; i++){
		if ( (!visitado[ gr->arestas[orig][i]])){ //&&  gr->arestas[orig][i] <= dest){
		//if ( (!visitado[ gr->arestas[orig][i]])){
			cont++;
			//printf("[%d]\n", cont);
			buscaProfundidade(gr, gr->arestas[orig][i], dest, visitado, confiancaTotal * gr->pesos[orig][i], pilha, maior_p);
			cont--;
		}
	}
	if(orig == dest){
		if (confiancaTotal > maior_p->confianca && cont != 0){
			free(maior_p->caminho);
			maior_p->caminho = NULL;
			copiarPilha(&pilha, &maior_p->caminho);
			maior_p->qtd_elementos = cont;
			maior_p->confianca = confiancaTotal;
			//printf("CAMINHO [%f]\n", confiancaTotal);
		}
	}
	pilha = removerPilha(pilha);
	visitado[orig] = 0;
}
void buscaProfundidade_Grafo(Grafo *gr, int orig, int dest){

	int *visitados;
	visitados = (int *)calloc(gr->nro_vertices, sizeof(int));
	float confiancaTotal = 1;
	Pilha *maior_p = (Pilha *)malloc(sizeof(Pilha));
	PILHA *pilha = (PILHA *)malloc(sizeof(PILHA));
	maior_p->confianca = 0.0;
	pilha = NULL;
	buscaProfundidade(gr, orig, dest, visitados, confiancaTotal, pilha, maior_p);

	printf("\nCaminho: [ ");	
	mostrarPilha(maior_p->caminho);
	printf("]\nQuantidade de vertices: %d\n",maior_p->qtd_elementos);	
	printf("Confiabilidade: %f\n",maior_p->confianca);	
}



int main(){

	int Origem, Destino;

	Grafo *gr = NULL;
	gr = cria_Grafo(10, 1);
	printf("\n- - Grafo CRIADO - -\n");


	//TESTE DO GRAFO DO RELATÓRIO 
	//10 vertices ||  1 Ponderado
	insereAresta(gr, 0, 1, 1, 0.9);
	insereAresta(gr, 0, 2, 1, 0.2);
	insereAresta(gr, 0, 4, 1, 0.8);

	insereAresta(gr, 1, 2, 1, 0.9);
	insereAresta(gr, 1, 3, 1, 0.5);
	insereAresta(gr, 1, 4, 1, 1.0);
	
	insereAresta(gr, 2, 4, 1, 0.6);
	insereAresta(gr, 2, 5, 1, 0.6);
	
	insereAresta(gr, 3, 4, 1, 0.7);
	insereAresta(gr, 3, 6, 1, 0.0);
	insereAresta(gr, 3, 7, 1, 1.0);
	
	insereAresta(gr, 4, 5, 1, 0.3);
	insereAresta(gr, 4, 6, 1, 0.7);
	insereAresta(gr, 4, 7, 1, 0.1);
	insereAresta(gr, 4, 8, 1, 0.4);
	insereAresta(gr, 4, 9, 1, 0.8);
	
	insereAresta(gr, 5, 8, 1, 0.4);
	insereAresta(gr, 5, 9, 1, 0.3);
	
	insereAresta(gr, 6, 7, 1, 0.0);
	insereAresta(gr, 7, 8, 1, 0.9);
	insereAresta(gr, 8, 9, 1, 0.1);
	printf("\n- - Arestas INSERIDAS --\n");


			
	printf("Vertice Inicial: ");
	scanf("%d", &Origem);
	printf("Vertice Final: ");
	scanf("%d", &Destino);
		
	buscaProfundidade_Grafo(gr, Origem, Destino);
	


	liberar_Grafo(gr);
	printf("\n- - Grafo LIBERADO - -\n");
	return 0;
}