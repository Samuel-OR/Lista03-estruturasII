#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>
int totalFunc = 1000;
int tamanho = 101;

typedef struct{
	char matricula[7];
	char nome[50];
	char funcao[50];
	float salario;	
}FUNCIONARIO;


void setEntrada(FUNCIONARIO *func){
	
	FILE *arq = fopen("entradaHASHING-05.txt", "r");
	if (arq == NULL){
    	printf("ERRO! O arquivo não foi aberto (FUNÇÃO setEntrada)!\n");
	}else{
		int i=0;
		while( (fscanf(arq, "%s", func[i].matricula)) != EOF){
			//printf("%ld = [%s]\n",strlen(func[i].matricula), func[i].matricula );
			i++;
		}
		if(i != totalFunc)
			printf("Inconsistência entre qtd de matriculas e qtd totalFunc.\n");
	}
	/*
	setbuf(stdin, NULL); scanf("%s", (func->matricula));
	setbuf(stdin, NULL); scanf("%s", (func->nome));
	setbuf(stdin, NULL); scanf("%s", (func->funcao));
	setbuf(stdin, NULL); scanf("%f", &(func->salario));
	setbuf(stdin, NULL);
	*/
}

int have_collision(int *ocupado, int num){
	if(ocupado[num]==1)
		return 1;
	return 0;
}

int hashing_A(char* matricula,int* ocupado, int *colisoes, int cheio){
	int posHashing =(	((matricula[5]-48)*100) +
						((matricula[1]-48)*10) + 
						((matricula[3]-48)*1) 
					)%tamanho;
	int novo = posHashing;
	int primeiroNUM  = matricula[4]-48;
	if(cheio==0){

		//Enquanto estiver ocupado e chave ser melhor que tamanho
		while( novo < tamanho && have_collision(ocupado, novo)==1){ 	
			(*colisoes)++;					//Gera nova chave e conta colisão
			novo = (novo+primeiroNUM);
			
			if(primeiroNUM==0)				//Evitar loop somando 0
				break;
		}
		if(novo < tamanho)					//Se encontrou uma posição desocupada
			posHashing = novo;
	
	}else{
		//cheio
		(*colisoes)++;
	}
	return posHashing;
}

int hashing_B(char* matricula,int* ocupado, int *colisoes, int cheio){
	int num1 = (	((matricula[0]-48)*100) +
					((matricula[2]-48)*10) + 
					((matricula[5]-48)*1)
				);

	int num2 = (
					((matricula[1]-48)*100) +
					((matricula[3]-48)*10) + 
					((matricula[4]-48)*1)
				);
	int posHashing =( (num1+num2)%1000 )%tamanho;
	int novo = posHashing;
	int primeiroNUM  = matricula[4]-48;
	
	if(cheio==0){

		//Enquanto estiver ocupado e chave ser melhor que tamanho
		while( have_collision(ocupado, novo)==1 && novo < tamanho){
			novo += 7;
			(*colisoes)++;	//Gera nova chave e conta colisão
		}
		
		if(novo < tamanho)	//Se encontrou uma posição desocupada
			posHashing = novo;

	}else{
		(*colisoes)++;
	}
	return posHashing;
}

int cheio(int *ocupado){
	for(int i = 0; i<tamanho; i++){
		if(ocupado[i] == 0){
			return 0;		
		}
	}
	return 1;		
}

int main(){

	FUNCIONARIO *func;
	func = (FUNCIONARIO*) calloc(totalFunc, sizeof(FUNCIONARIO));
	int colisoesA=0, posA, colisoesB=0, posB;
	int *ocupadoA = (int*) calloc(tamanho, sizeof(int));
	int *ocupadoB = (int*) calloc(tamanho, sizeof(int));

	setEntrada(func);

	for(int i=0; i<totalFunc; i++){
			
		posA = hashing_A(func[i].matricula, ocupadoA, &colisoesA, (i<tamanho)? 0:1);
		posB = hashing_B(func[i].matricula, ocupadoB, &colisoesB, (i<tamanho)? 0:1);
		//posA = hashing_A(func[i].matricula, ocupadoA, &colisoesA, cheio(ocupadoA));
		//posB = hashing_B(func[i].matricula, ocupadoB, &colisoesB, cheio(ocupadoB));
		ocupadoA[posA] = 1;
		ocupadoB[posB] = 1;	
	}
	printf("Colisoes_A[%d]: %d\n",tamanho,colisoesA );
	printf("Colisoes_B[%d]: %d\n\n",tamanho, colisoesB );
	
	colisoesA = 0;
	colisoesB = 0;
	
	free(ocupadoA);
	free(ocupadoB);
	
	ocupadoA = (int*) calloc(tamanho, sizeof(int));
	ocupadoB = (int*) calloc(tamanho, sizeof(int));

	tamanho = 150;

	for(int i=0; i<totalFunc; i++){
		
		posA = hashing_A(func[i].matricula, ocupadoA, &colisoesA,  (i<tamanho)? 0:1);
		posB = hashing_B(func[i].matricula, ocupadoB, &colisoesB,  (i<tamanho)? 0:1);	
		//posA = hashing_A(func[i].matricula, ocupadoA, &colisoesA, cheio(ocupadoA));
		//posB = hashing_B(func[i].matricula, ocupadoB, &colisoesB, cheio(ocupadoB));
		ocupadoA[posA] = 1;
		ocupadoB[posB] = 1;
	}
	printf("Colisoes_A[%d]: %d\n",tamanho,colisoesA );
	printf("Colisoes_B[%d]: %d\n",tamanho,colisoesB );
	
	return 0;
}