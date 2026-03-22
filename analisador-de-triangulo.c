//Leandro H Bitencort
//Projeto de extensao - Verificação de Triângulos
//Utilizei a Biblioteca do prof. que nos foi fornecida

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pss.h"

#define MAX_TRIANGULO 1000
#define NUM_TRIANGULO 12

typedef struct{
	int id, lado1, lado2, lado3;
	char tipo[NUM_TRIANGULO];
}Triangulo;

void clear(){
	#ifdef __linux__
		system("clear");
	#elif _WIN32
		system("cls");
	#endif
}

int carrega_dados(Triangulo *triangulos,int *total){
	
	FILE *arquivo = fopen("triangulos.bin", "rb");
	if (arquivo == NULL){
		printf("arquivo nao encontrado!\n");
		return 0;
	}
	*total = fread(triangulos, sizeof(Triangulo), MAX_TRIANGULO, arquivo);
	fclose(arquivo);
	return 1;
}

int gerar_id(){
	FILE *arquivo_id;
	int id = 1;
	
	arquivo_id = fopen("arquivo_id.txt", "r+");
	
	if (arquivo_id == NULL){
		arquivo_id = fopen("arquivo_id.txt", "w");
		    if (arquivo_id == NULL) {
            	perror("Erro ao criar o arquivo de ID");
            	return 0;
        } else {
            fprintf(arquivo_id, "%d", id);
            fclose(arquivo_id);
            return id;
        }
    } else {
        fscanf(arquivo_id, "%d", &id);
        id++;
        rewind(arquivo_id);
        fprintf(arquivo_id, "%d", id);
        fclose(arquivo_id);
        return id;
    }
}

char* verifica_triangulo(int a, int b, int c){
	if(a+b>c && a+c>b && b+c>a)
		if 		(a==b && b==c)  return "Equilatero";
		else if (a==b || b==c||a==c)  return "Isosceles";
		else 					return "Escaleno";
	else return "Nao existe";	
}

void salvar_arquivo(Triangulo *triangulos, int total){
	FILE *arquivo = fopen("triangulos.bin","wb");
	if(arquivo==NULL){
		printf("Erro ao abri o arquivo triangulo.bin!\n");
		return;
	}
	fwrite(triangulos, sizeof(Triangulo),total, arquivo);
	fclose(arquivo);
}

void cadastrar_triangulo(Triangulo *triangulos, int *total){
	int i = *total, novo_id;
	char* tipo_retornado;
	
	printf("\n *** CADASTRO TRIANGULO ***\n");
	
	if(i<MAX_TRIANGULO){
		
		novo_id = gerar_id();
		if(novo_id==0){
			printf("Erro ID nulo!\n");
		return;
		}
		triangulos[i].id = novo_id;
		
		triangulos[i].lado1 = input_d("Digite lado 1 ");
		triangulos[i].lado2 = input_d("Digite lado 2 ");
		triangulos[i].lado3 = input_d("Digite lado 3 ");
		
		tipo_retornado =  verifica_triangulo(triangulos[i].lado1, triangulos[i].lado2, triangulos[i].lado3);
		
		strcpy(triangulos[i].tipo, tipo_retornado);
		printf("Triangulo cadastrado com sucesso!\n");
		
		(*total)++;
		salvar_arquivo(triangulos, *total);	
	}
	else{
		printf("Limite atingido!\n");
	}
}

void listar(Triangulo *triangulos, int total, char *palavra, char *titulo){
	int encontrado = 0;
	
	printf("*** %s ***\n", titulo);
	for(int i = 0; i < total; i++){
		if(palavra == NULL || strcmp(triangulos[i].tipo, palavra) == 0 ){
			if(encontrado ==0){
				printf("+------+---------+---------+--------+--------------+\n");	
				printf("|  ID  |  LADO 1 |  LADO 2 | LADO 3 |    TIPO      |\n");	
				printf("+------+---------+---------+--------+--------------+\n");
			}
		printf("| %-4d | %-7d | %-7d | %-6d | %-12s |\n",	triangulos[i].id, 
			triangulos[i].lado1, 
			triangulos[i].lado2,
        	triangulos[i].lado3,
			triangulos[i].tipo);
		printf("+------+---------+---------+--------+--------------+\n");
		encontrado++;
		}
	}
	if(encontrado ==0){
		if(palavra == NULL)
			printf("\nNenhum triangulo cadastrado ainda.\n");
		else
			printf("\nNenhum triangulo do tipo %s foi encontrado. \n", palavra);
	}		
}

void altera_dados(Triangulo *triangulos, int total){
	int alteracao, encontrei =0;
	char* tipo_retornado;
	
	printf("\n*** Altera dados ***\n");
	alteracao  = input_d("Digite o ID que quer alterar: ");
	for(int i = 0; i<total;i++){
		if(triangulos[i].id == alteracao){
			printf("\n*** TRIANGULO QUE DESEJA ALTERAR ***\n");
			printf("+------+---------+---------+--------+--------------+\n");	
			printf("|  ID  |  LADO 1 |  LADO 2 | LADO 3 |    TIPO      |\n");	
			printf("+------+---------+---------+--------+--------------+\n");
			printf("| %-4d | %-7d | %-7d | %-6d | %-12s |\n",	
				triangulos[i].id, 
				triangulos[i].lado1, 
				triangulos[i].lado2,
        		triangulos[i].lado3,
				triangulos[i].tipo);
			printf("+------+---------+---------+--------+--------------+\n");
			
            triangulos[i].lado1 = input_d("\ndigite o novo lado 1: ");
            triangulos[i].lado2 = input_d("\ndigite o novo lado 2: ");
            triangulos[i].lado3 = input_d("\ndigite o novo lado 3: ");
            
			tipo_retornado = verifica_triangulo(triangulos[i].lado1, triangulos[i].lado2, triangulos[i].lado3);
			strcpy(triangulos[i].tipo, tipo_retornado);
			salvar_arquivo(triangulos, total);
			printf("\nTriangulo alterado com sucesso!\n");
            encontrei = 1;
            break;
		}
	}
	if(encontrei == 0){
		printf("triangulo nao encontrado!\n");
	}
}

void main(){
	Triangulo triangulos[MAX_TRIANGULO];
	int opcao, total = 0 ;
	
	if (carrega_dados(triangulos, &total))
		printf("%d", total);
	else
		printf("Nao existe nenhum dados salvo!\n");
	
	do{
		clear();
		printf("\n*** Menu ***\n");
		printf(".01 Cadastrar Triangulo\n");
		printf(".02 Listar Todos \n");
		printf(".03 Lista Somente Equilatero\n");
		printf(".04 Listar Somente Isosceles\n");
		printf(".05 Listar Somente Escaleno\n");
		printf(".06 Alterar dados \n");
		printf(".0 Sair\n");           
		opcao = input_d("\nDigite a opcao "); 
		
		switch (opcao){
			case 1:
				cadastrar_triangulo(triangulos,&total);
				break;
			case 2:
				listar(triangulos, total, NULL, " LISTAR TODOS ");
				break;
			case 3:
				listar(triangulos, total, "Equilatero", " LISTAR EQUILATERO ");
				break;
			case 4:
				listar(triangulos, total, "Isosceles", " LISTAR ISOSCELES ");
				break;
			case 5:
				listar(triangulos, total, "Escaleno", " LISTAR ESCALENO ");
				break;
			case 6:
				altera_dados(triangulos, total);
				break;
			case 0:
				printf("Saindo...\n");
				break;
			default:
				printf("Opcao invalida, tente novamente!\n");
		}	
		printf("\nPreciso Enter para continuar...");
		setbuf(stdin, NULL);
		getchar();
	}while (opcao!=0);              
}
