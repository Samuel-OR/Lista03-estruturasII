#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
int tamanho = 101;

typedef struct{
	char matricula[6];
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
			i++;
		}
	}
	
	/*
	setbuf(stdin, NULL);
	scanf("%s", (func->matricula));
	setbuf(stdin, NULL);
	scanf("%s", (func->nome));
	setbuf(stdin, NULL);
	scanf("%s", (func->funcao));
	setbuf(stdin, NULL);
	scanf("%f", &(func->salario));
	setbuf(stdin, NULL);
	*/
}

int have_collision(int *ocupado, int num){
	if(ocupado[num])
		return true;
	return false;
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
		while( have_collision(ocupado, novo) && novo < tamanho){ 	
			novo = (novo+primeiroNUM);
			(*colisoes)++;					//Gera nova chave e conta colisão
			if(primeiroNUM==0)				//Evitar loop somando 0
				break;
		}
		if(novo < tamanho)					//Se encontrou uma posição desocupada
			posHashing = novo;
	}else{
			(*colisoes)++;
	}
	return posHashing;
}

int hashing_B(char* matricula,int* ocupado, int *colisoes, int cheio){
	int posHashing =(	((matricula[0]-48)*100) +
						((matricula[2]-48)*10) + 
						((matricula[5]-48)*1) +

						((matricula[1]-48)*100) +
						((matricula[3]-48)*10) + 
						((matricula[4]-48)*1)
					)%tamanho;
	int novo = posHashing;
	int primeiroNUM  = matricula[4]-48;
	
	if(cheio==0){

		//Enquanto estiver ocupado e chave ser melhor que tamanho
		while( have_collision(ocupado, novo) && novo < tamanho){
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

int main(){

	FUNCIONARIO *func;
	func = (FUNCIONARIO*) calloc(sizeof(FUNCIONARIO),1000);
	int colisoes=0, pos;
	int *ocupado = (int*) calloc(sizeof(int),tamanho);
	
	setEntrada(func);

	for(int i=0; i<1000; i++){
		if(i < tamanho)
			pos = hashing_A(func[i].matricula, ocupado, &colisoes, 0);
		else
			pos = hashing_A(func[i].matricula, ocupado, &colisoes, 1);
		ocupado[pos] = true;
	}
	printf("colisoes A: %d\n",colisoes );
	
	colisoes = 0;
	ocupado = (int*) calloc(sizeof(int),tamanho);

	for(int i=0; i<1000; i++){
		if(i < tamanho)
			pos = hashing_B(func[i].matricula, ocupado, &colisoes, 0);
		else
			pos = hashing_B(func[i].matricula, ocupado, &colisoes, 1);
		ocupado[pos] = true;
	}
	printf("colisoes B: %d\n",colisoes );
	return 0;
}