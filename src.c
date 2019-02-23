// lab1 trabalho final.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 31
#define M 15
typedef struct _Produto {	//estrutura Produto
	char designacao[N], tipo[M];
	int codigo, stock;
	float preco, iva;

}produto;
int conta_linhas(FILE *ficheiro);
void ler_ficheiro(FILE *ficheiro, produto *v, int *dim);
void log_txt(FILE *fp, produto *v, int *dim);
void listar_tipos(FILE *ficheiro, produto *v, int *dim);
void adicionar_produto(FILE *ficheiro, produto *v, int *dim);
void eliminar_produto(FILE *ficheiro, produto *v, int *dim);
void procurar_produto(FILE *ficheiro, produto *v, int *dim);
int main(void){
	produto *v = NULL;
	FILE *ficheiro=NULL, *fp=NULL;
	int linhas,n_menu;
	ficheiro = fopen("prod.txt", "r");
	if(ficheiro == NULL){	//abertura do ficheiro
		printf("Erro na abertura do ficheiro prod.txt2\n");
		exit(1);
	}
	linhas = conta_linhas(ficheiro);	//conta o número de linhas do ficheiro
	fseek(ficheiro, 0, SEEK_SET);
	v = (produto*)malloc(linhas*sizeof(produto));
	ler_ficheiro(ficheiro, v, &linhas);	//lê o ficheiro
	fclose(ficheiro);
	//---------------------------------------MENU-----------------------------------------------//
	while (1) {
		printf("MENU:\n");
		printf("Saber o preco de Produto: -->INTTRODUZA 1\n");
		printf("Listar Produtos por Tipo: -->INTRODUZA 2\n");
		printf("Acrescentar Produto: -->INTRODUZA 3\n");
		printf("Eliminar Produto: --> INTRODUZA 4\n");
		printf("Se pretende sair INTRODUZA --> 0\nObrigado!\n");
		scanf("%d", &n_menu);
		if (n_menu != 0) {
			switch (n_menu) {
			case 1:	//primeira opção 
				procurar_produto(ficheiro, v, &linhas);
				break;
			case 2:	//segunda opção
				listar_tipos(ficheiro, v, &linhas);
				break;
			case 3:	//terceira opção
				adicionar_produto(ficheiro, v, &linhas);
				break;
			case 4:	//quarta opção
				eliminar_produto(ficheiro, v, &linhas);
				break;
			
			default:
				printf("Erro! Opcao Invalida!\n");
			}
		}
		else {
			break;
		}
	}
	//-------------------------------------FIM DO MENU --------------------------------------//
	fp = fopen("log.txt", "w");
	if (fp == NULL) {
		printf("Erro na abertura do ficheiro log.txt");
		exit(1);
	}
	log_txt(fp, v, &linhas);
	fclose(ficheiro);
	free(v);
	printf("\n");
}
int conta_linhas(FILE *ficheiro) {	//contador de linhas	
	int cnt = 0;
	char a=' ';
	while (!feof(ficheiro)) {	//enquanto não for o fim do ficheiro o ciclo continua 
		a=fgetc(ficheiro);
		if (a == '\n') {	//quando encontrar um '\n' conta uma linha
			cnt++;
		}
	}
	return cnt;
}
void ler_ficheiro(FILE *ficheiro, produto *v, int *dim) {	//lê o ficheiro para o vetor v (alocado dinâmicamente) 
	int i;
	fseek(ficheiro, 0, SEEK_SET);
	for (i = 0;!feof(ficheiro) && i < (*dim); i++) {	
		fscanf(ficheiro, " %[^;];%d;%[^;];%f;%f;%d\n", &v[i].designacao, &v[i].codigo, &v[i].tipo, &v[i].preco, &v[i].iva, &v[i].stock);
			
	}
}
void log_txt(FILE *fp, produto *v, int *dim) {
	int i, c_designacoes = 0, c_stock = 0, ali_tipo = 0, lim_tipo = 0, hig_tipo = 0, cong_tipo = 0, beb_tipo = 0;
	float valor_semIVA = 0, valor_comIVA = 0, preco_comIVA = 0, preco_semIVA = 0;
	for (i = 0; i < (*dim); i++) {
		if (v[i].designacao != NULL) {	//contador de Designações
			c_designacoes++;
		}
		c_stock = c_stock + v[i].stock;		//contador de stock											
		if (strcmp(v[i].tipo, "alimentacao") == 0) {	//isto é para contar quantos tipo existem de cada um
			ali_tipo++;
		}
		if (strcmp(v[i].tipo, "limpeza") == 0) {
			lim_tipo++;
		}
		if (strcmp(v[i].tipo, "higiene") == 0) {
			hig_tipo++;
		}
		if (strcmp(v[i].tipo, "congelados") == 0) {
			cong_tipo++;
		}
		if (strcmp(v[i].tipo, "bebidas") == 0) {
			beb_tipo++;
		}
		//valor sem iva, tendo em conta que temos que multiplicar pelo stock, pois queremos o valor total
			preco_semIVA = (float)(v[i].preco*v[i].stock);
			valor_semIVA = (float)(valor_semIVA + preco_semIVA);
		//valor com iva tendo em conta o número de produtos em stock 
			preco_comIVA = (float)(((v[i].preco * v[i].iva) + v[i].preco)*v[i].stock);
			valor_comIVA = (float)(valor_comIVA + preco_comIVA);
	}
	fprintf(fp, "Total de Designacoes:%d\n", c_designacoes);	//escrever no ficheiro log.txt 
	fprintf(fp, "Total de Produtos em Stock:%d\n", c_stock);
	fprintf(fp, "Total de Desiganacoes por Produto:\n");
	fprintf(fp, "-->Alimentacao:%d\n", ali_tipo);
	fprintf(fp, "-->Limpeza:%d\n", lim_tipo);
	fprintf(fp, "-->Higiene:%d\n", hig_tipo);
	fprintf(fp, "-->Congelados:%d\n", cong_tipo);
	fprintf(fp, "-->Bebidas:%d\n", beb_tipo);
	fprintf(fp, "\n");
	fprintf(fp, "Valor Total dos Produtos(sem iva):%.2f euros\n", valor_semIVA);
	fprintf(fp, "Valor Total dos Produtos(com iva):%.2f euros\n", valor_comIVA);
}
void procurar_produto(FILE *ficheiro, produto *v, int *dim) {
	int i,flag_controle=0,codigo=0;
	while (1) {
		printf("Introduza o Codigo do Produto(maximo 6 digitos):\n");
		scanf("%d", &codigo);

		for (i = 0; i < (*dim); i++) {
			if (v[i].codigo == codigo) {
				flag_controle++;
				printf("O Produto com Codigo = %d tem um preco = %.2f euros.\n", v[i].codigo, v[i].preco);
				break;
			}
		}
		if (flag_controle > 0) {
			break;
		}
		else {
			printf("Nao existe nenhum produto com esse Codigo!\n");
			continue;
		}
	}
}
void listar_tipos(FILE *ficheiro, produto *v, int *dim) {
	int i, j;
	produto *vaux = NULL;
	j = 0;
	vaux = (produto*)malloc((*dim) * sizeof(produto));	//aqui copio a informação do vetor v para vaux

	for (i = 0; i < (*dim); i++) {
		/*
		strcpy(vaux[i].designacao, v[i].designacao);
		vaux[i].codigo = v[i].codigo;
		strcpy(vaux[i].tipo, v[i].tipo);
		vaux[i].preco = v[i].preco;
		vaux[i].iva = v[i].iva;
		vaux[i].stock = v[i].stock;
		*/
		strncpy(vaux[i].designacao, v[i].designacao, strlen(v[i].designacao)+1);
		vaux[i].codigo = v[i].codigo;
		strncpy(vaux[i].tipo, v[i].tipo, strlen(v[i].tipo)+1);
		vaux[i].preco = v[i].preco;
		vaux[i].iva = v[i].iva;
		vaux[i].stock = v[i].stock;
	}
	//free(v);
	v = (produto*)malloc((*dim) * sizeof(produto));
	for (i = 0; i < (*dim); i++) {
		if ((strcmp(vaux[i].tipo, "alimentacao") == 0)) {	//aqui procuro o tipo Alimentação
			strncpy(v[j].designacao, vaux[i].designacao, strlen(vaux[i].designacao)+1);
			v[j].codigo = vaux[i].codigo;
			strncpy(v[j].tipo, vaux[i].tipo, strlen(vaux[i].tipo)+1);
			v[j].preco = vaux[i].preco;
			v[j].iva = vaux[i].iva;
			v[j].stock = vaux[i].stock;
			j++;
		}
	}
	for (i = 0; i < (*dim); i++) {
		if ((strcmp(vaux[i].tipo, "limpeza") == 0)) {	//aqui procuro o tipo limpeza
			strncpy(v[j].designacao, vaux[i].designacao, strlen(vaux[i].designacao)+1);
			v[j].codigo = vaux[i].codigo;
			strncpy(v[j].tipo, vaux[i].tipo, strlen(vaux[i].tipo)+1);
			v[j].preco = vaux[i].preco;
			v[j].iva = vaux[i].iva;
			v[j].stock = vaux[i].stock;
			j++;
		}
	}
	for (i = 0; i < (*dim); i++) {
		if ((strcmp(vaux[i].tipo, "higiene") == 0)) {	//aqui procuro o tipo higiene
			strncpy(v[j].designacao, vaux[i].designacao, strlen(vaux[i].designacao)+1);
			v[j].codigo = vaux[i].codigo;
			strncpy(v[j].tipo, vaux[i].tipo, strlen(vaux[i].tipo)+1);
			v[j].preco = vaux[i].preco;
			v[j].iva = vaux[i].iva;
			v[j].stock = vaux[i].stock;
			j++;
		}
	}
	for (i = 0; i < (*dim); i++) {
		if ((strcmp(vaux[i].tipo, "congelados") == 0)) {	//aqui procuro o tipo congelados 
			strncpy(v[j].designacao, vaux[i].designacao, strlen(vaux[i].designacao)+1);
			v[j].codigo = vaux[i].codigo;
			strncpy(v[j].tipo, vaux[i].tipo, strlen(vaux[i].tipo)+1);
			v[j].preco = vaux[i].preco;
			v[j].iva = vaux[i].iva;
			v[j].stock = vaux[i].stock;
			j++;
		}
	}
	for (i = 0; i < (*dim); i++) {
		if ((strcmp(vaux[i].tipo, "bebidas") == 0)) {	//aqui procuro o tipo bebidas
			strncpy(v[j].designacao, vaux[i].designacao, strlen(vaux[i].designacao)+1);
			v[j].codigo = vaux[i].codigo;
			strncpy(v[j].tipo, vaux[i].tipo, strlen(vaux[i].tipo)+1);
			v[j].preco = vaux[i].preco;
			v[j].iva = vaux[i].iva;
			v[j].stock = vaux[i].stock;
			j++;
		}
	}
	free(vaux);
	ficheiro = fopen("prod.txt", "w");
	if(ficheiro == NULL){	//abertura do ficheiro
		printf("Erro na abertura do ficheiro prod.txt2\n");
		exit(1);
	}
	fseek(ficheiro, 0, SEEK_SET);	//posiciono o cursor no início do ficheiro
	for (i = 0; i < (*dim); i++) {	//escrevo no ficheiro
		fprintf(ficheiro, "%s;%d;%s;%.2f;%.2f;%d\n", v[i].designacao, v[i].codigo, v[i].tipo, v[i].preco, v[i].iva, v[i].stock);
	}
	fclose(ficheiro);
	printf("Produtos Listados por tipo com sucesso.\n");

}
void adicionar_produto(FILE *ficheiro, produto *v, int *dim) {
	int i,flag=0,codigoaux=0;
	*dim = *dim + 1;
	char auxstr[31];
	v = (produto*)realloc(v, ((*dim)* sizeof(produto))); //aqui preencho o meu novo produto
	printf("Designacao (maximo 30 carateres):\n");
	//scanf("%s", v[(*dim) - 1].designacao);
	
	scanf(" %[^\n]s", auxstr);
	strncpy(v[(*dim) - 1].designacao, auxstr, strlen(auxstr)+1);
	
	printf("Codigo (maximo de 6 digitos):\n");
	while (1) {									//aqui testamos se o código já existe 
		scanf("%d", &codigoaux);
		flag = 0;
		for (i = 0; i < (*dim) - 1; i++) {
			if (v[i].codigo == codigoaux) {
				flag++;							//flag é uma variável de controlo
			}
			if (flag > 0) {
				printf("Esse Codigo ja existe introduza um codigo diferente!\n");
				break;
			}
		}
		if (flag == 0) {
			v[(*dim) - 1].codigo = codigoaux;
			break;
		}
	}
	printf("Tipo (alimentacao;limpeza;higiene;congelados;bebidas):\n");
	//scanf(" %[^\n]s", auxstr);
	scanf("%s", v[(*dim) - 1].tipo);
	//strncpy(v[(*dim) - 1].tipo, auxstr, strlen(auxstr)+1);
	printf("Preco (com precisao de centimo e deve usar o caracter ponto '.'):\n");
	scanf(" %f", &(v[(*dim) - 1].preco));
	printf("IVA (13%%,23%%, mas coloque 0.13,0.23 respetivamente:\n");
	scanf(" %f", &(v[(*dim) - 1].iva));
	printf("Stock (quantidade em stock):\n");
	scanf(" %d", &(v[(*dim) - 1].stock));

	ficheiro = fopen("prod.txt", "w"); 
	if (ficheiro == NULL) {
		printf("Erro na abertura do ficheiro prod.txt\n");
		exit(1);
	}
	fseek(ficheiro, 0, SEEK_SET);
	for (i = 0; i < (*dim); i++) {
		fprintf(ficheiro, "%s;%d;%s;%.2f;%.2f;%d\n", v[i].designacao, v[i].codigo, v[i].tipo, v[i].preco, v[i].iva, v[i].stock);
	}
	fclose(ficheiro);
	printf("Produto acrescentado com sucesso!\n");	//como tenho mais um produto, aumentei a minha dimenssão numa unidade
}
void eliminar_produto(FILE *ficheiro, produto *v, int *dim) {
	int i,j,codigo=0;
	produto *vaux = NULL;
	vaux = (produto*)malloc((*dim) * sizeof(produto));
	printf("Introduza o Codigo do Produto que pretende eliminar.\n");
	scanf("%d", &codigo);
	for (i = 0; i < (*dim); i++) {
		strcpy(vaux[i].designacao, v[i].designacao);	//aqui copio v para vaux
		vaux[i].codigo = v[i].codigo;
		strcpy(vaux[i].tipo, v[i].tipo);
		vaux[i].preco = v[i].preco;
		vaux[i].iva = v[i].iva;
		vaux[i].stock = v[i].stock;
	}
	//free(v);	//limpo o vetor v
	*dim = *dim - 1;
	v = (produto*)malloc((*dim)* sizeof(produto));
	for (i = 0; i < ((*dim) + 1); i++) {
		if (vaux[i].codigo == codigo) {	//procuro o produto com o codigo e limpo a sua informação
			strcpy(vaux[i].designacao, "");
			vaux[i].codigo = 0;
			strcpy(vaux[i].tipo, "");
			vaux[i].preco = 0;
			vaux[i].iva = 0;
			vaux[i].stock = 0;
			break;
		}
	}
	for (i = 0, j = 0; i < (*dim), j < ((*dim) + 1); i++, j++) {	//aqui vou copiar vaux para v excluindo o produto que quero eliminar
		if (((strcmp(vaux[j].designacao, "")) == 0) && (vaux[j].codigo == 0) && (strcmp(vaux[j].tipo, "") == 0) && (vaux[j].preco == 0) && (vaux[j].iva == 0) && (vaux[j].stock == 0)) {
			i--;		//isto serve para que a posição do produto apagado seja ocupado pelo produto que vem a seguir 
		}
		else {
			strcpy(v[i].designacao, vaux[j].designacao);	//aqui copia normalmente o resto dos produtos
			v[i].codigo = vaux[j].codigo;
			strcpy(v[i].tipo, vaux[j].tipo);
			v[i].preco = vaux[j].preco;
			v[i].iva = vaux[j].iva;
			v[i].stock = vaux[j].stock;

		}


	}
	free(vaux);	//libertar a memória de vaux
	ficheiro = fopen("prod.txt", "w");
	if (ficheiro == NULL) {
		printf("Erro na abertura do ficheiro prod.txt\n");
		exit(1);
	}
	fseek(ficheiro, 0, SEEK_SET);
	for (i = 0; i < (*dim); i++) {
		fprintf(ficheiro, "%s;%d;%s;%.2f;%.2f;%d\n", v[i].designacao, v[i].codigo, v[i].tipo, v[i].preco, v[i].iva, v[i].stock);
	}
	fclose(ficheiro);
	printf("Produto eliminado com Sucesso!\n");
}
