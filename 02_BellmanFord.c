#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include<sys/time.h>
#define discos 4
#define pinos 3

int **estadoTorre;

typedef struct{
    int nro_vertices;
    int** arestas;
    int eh_ponderado;
    float** pesos;
    int* grau;
    int grau_max;
    int nivel[81];
}Grafo;

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
                insereAresta(grafo, x+1, y, 1, 0);
            }
        }

    }
}

void mostrarCaminho(int *dist, int *predecessor, int node, int *cont){
    if(dist[node]!=0){
        *cont += 1;
        mostrarCaminho(dist, predecessor, predecessor[node], cont);
    }
    printf("%d |[", 1+node);
    for(int z=0; z<4; z++){
        printf("%d ",estadoTorre[node][z] );
    }
    printf("]\n");
}

void bellmanFord(Grafo *gr, int orig){
    int cost = 0;
    int flag = 1;
    int predecessor[81], dist[81];

    for (int i = 0; i < 81; i++){ 
        dist[i] = INT_MAX/2;
    }

    dist[orig] = 0;
    predecessor[orig]= orig;
    for(int i=0; i<80 && flag==1;i++){ //parar quando não houver alterações ou iterar n-1 vezes
        flag = 0;

        for(int x=0;x<81;x++){
        
            if( dist[x] != (INT_MAX/2) ){
                for(int j=0;j< gr->grau[x];j++){
                    if(dist[gr->arestas[x][j]] > (dist[x]+1)){
                        predecessor[gr->arestas[x][j]] = x;
                        dist[gr->arestas[x][j]] = dist[x]+1;
                        flag = 1;
                    }
                }
            }
        }
    }

    printf("---------------------\n");
    int cont=0;
    mostrarCaminho(dist, predecessor,80, &cont);
    printf("Qtd Vertices a percorrer: %d\n",cont );
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
    
    estadoTorre = gerarPossibilidades(possibilidades);

    Grafo *grafo = cria_Grafo(possibilidades,pinos, 1);
    
    int *visitados = (int*) calloc(possibilidades+1, sizeof(int));
    
    construirGrafo(grafo, estadoTorre, possibilidades);

    int vet[4];
    printf("Digite  posição de inicio (Ex: [1 1 1 1]): ");
    scanf("%d %d %d %d", &vet[0], &vet[1], &vet[2], &vet[3]);
    
    int inicio = equivalente(estadoTorre,vet);
    printf("Inicio: %d \n",inicio+1);
    
    gettimeofday(&tempo_inicial, NULL);
    
    bellmanFord(grafo, inicio);  

    gettimeofday(&tempo_final, NULL);
    tempo_total = (tempo_final.tv_sec - tempo_inicial.tv_sec) * (int)1e6 + (tempo_final.tv_usec - tempo_inicial.tv_usec);
    printf("TEMPO TOTAL: %.3f microsegundos\n", tempo_total);

    return 0;
}