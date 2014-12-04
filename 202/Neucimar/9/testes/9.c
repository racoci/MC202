/* Nome: Lucas Alves Racoci
 * RA: 156331 
 * Laboratorio 09 - Árvores 2-3 (inserção) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* STRUCTS, DEFINIÇÕES E CABEÇALHOS DAS MINHAS FUNÇÕES DA TAD */
/*************************************************************/

#define MAX_CHAVES 2

typedef enum bool{false, true} bool;

/*arvore23_no_t - define tipo nó de uma arvore 2-3*/
typedef struct arvore23_no {
	/*valor das chaves do no, se existirem*/
	int chave[MAX_CHAVES];
	/*numero de chaves existentes no nó. (Pode ser 1 ou 2) */
	int nchaves;
	/*ponteiros para os nós filhos*/
	struct arvore23_no * filho[MAX_CHAVES+1];	
	/*ponteiro para o nó pai (não é usado na função imprime)*/
	struct arvore23_no *pai;
} Pagina, *Arvore23;

/*Novos campos podem ser acrescentados ou retirados da estrutura de dados a gosto.*/
/*Mas cuidado! Excluir um campo pode alterar a corretude da função imprime*/

/* TAD: Arvores 2-3 */
/*******************/

/* arvore23_aloca() - Cria árvore 2-3. */
Arvore23 arvore23_aloca();

/* arvore23_libera() - Libera árvore 2-3. */
void arvore23_libera(Arvore23 t);

/* arvore23_insere() - insere chave na árvore 2-3. Não insere se chave já existir na árvore */
void arvore23_insere(Arvore23 *t, int chave);

/* arvore23_busca() - busca chave na árvore 2-3. Retorna 1 se encontrar ou 0 se não encontrar */
int arvore23_busca(Arvore23 t, int chave);

/* arvore23_imprime() - imprime árvore 2-3. */
void arvore23_imprime(Arvore23 t);


/* ASSINATURA DE OUTRAS FUNÇÕES */ 
/*******************************/


/*programa que implementa arvores 2-3 com chaves inteiras*/

int main(){
	Arvore23 t;
	int i,N,chave;
	char comando[9];

	/* Cria árvore 2-3 */
	t = arvore23_aloca();

	/*le quantidade de entradas*/
	scanf("%d\n",&N);
	for(i = 0; i < N; i++) {
	    /*le instrução*/
		scanf("%s", comando);
		if (!strcmp(comando,"inserir")){
			scanf("%d", &chave);
			arvore23_insere(&t, chave);
		}
		if (!strcmp(comando,"buscar")){
			scanf("%d", &chave);
			if (!arvore23_busca(t, chave))
				printf(":(\n");
			else
				printf(":)\n");		
		}
		if (!strcmp(comando,"imprimir")){
			arvore23_imprime(t);
			printf("\n");
		}
	} 
	/* libera memória */
	arvore23_libera(t);

    return 0;
}

/* IMPLEMENTAÇÃO DES OUTRAS FUNÇÕES */ 
/***********************************/
/**********************************/


/* IMPLEMENTAÇÃO DAS FUNÇÕES DE MINHA TAD */ 
/*****************************************/
/****************************************/

/* TAD: Arvores 2-3 */
/*******************/


/* Cria árvore 2-3. */
Arvore23 arvore23_aloca(){
	return NULL;
}

/* Libera árvore 2-3. */
void arvore23_libera(Arvore23 t){
	int i;
	if(t!=NULL){
		for(i = 0; i < 3; i++){
			arvore23_libera(t->filho[i]);
		}
		free(t); t=NULL;
	}
}

Arvore23 cria_pagina_arvore23(){
	int i;
	Arvore23 nova;
	nova = (Arvore23) malloc(sizeof(Pagina));
	nova->nchaves = 0;
	nova->pai = NULL;
	for(i = 0; i < MAX_CHAVES; i++){
		(*t)->filho[i] = NULL;
		(*t)->chave[i] = -1;
	}
	(*t)->filho[i] = NULL;
	
	return nova;
}

void insere_pagina(Arvore23 *t, int chave){
	if(*t == NULL) /*se t for vazia*/{
		(*t) = cria_pagina_arvore23();
		(*t)->chave[0] = chave;
		((*t)->nchaves)++;
		return;
	}if(t->nchaves < MAX_CHAVES){
		i = t->nchaves - 1;
		while(chave < t->chave[i] && i > 0)
			t->chave[i] = t->chave[--i];
		t->chave[i] = chave;
		return;
	}else{
		/*split()*/
		pai = (t->chave)/2;

	}
}

void arvore23_insere(Arvore23 *t, int chave){
	int i, pai;
	Arvore23 *p;
	if(*t == NULL) /*se t for vazia*/{
		insere_pagina(t, chave);
	}
	if (!arvore23_busca((*t), chave)) /*se chave ja existe em árvore*/ 
		return;
	p = t;
	/* Enquanto não é folha */
	while(p->filho[0] != NULL){
		if(chave < p->chave[0]){
			p = p->filho[0];
		}else if(chave < p->chave[1]){
			p = p->filho[1];
		}else {
			p = p->filho[2];
		}
	}

	if(t->nchaves < MAX_CHAVES){
		i = t->nchaves - 1;
		while(chave < t->chave[i] && i > 0)
			t->chave[i] = t->chave[--i];
		t->chave[i] = chave;
	}else{
		/*split()*/
		pai = (t->chave)/2;
		if(t->pai == NULL){
			t->pai = cria_pagina_arvore23();
		}


	}
}

bool arvore23_busca(Arvore23 t, int chave){
	if(t == NULL){
		return false;
	}
	if(chave == t->chave[0]){
		return true;
	}
	if(chave < t->chave[0]){
		return arvore23_busca(t->filho[0], chave)
	}else{
		if(chave == t->chave[1]){
			return true;
		}
		if(chave < t->chave[1]){
			return arvore23_busca(t->filho[1], chave);
		}else{
			return arvore23_busca(t->filho[2], chave);
		}
	}


}
void arvore23_imprime(Arvore23 t){
	if(t!=NULL){
		printf("( ");
		if (t->nchaves == 1){
			arvore23_imprime(t->filho[0]);
			printf("%d ",t->chave[0]);
			arvore23_imprime(t->filho[1]);
		}
		else /*t->nchaves == 2*/{
			arvore23_imprime(t->filho[0]);
			printf("%d ",t->chave[0]);
			arvore23_imprime(t->filho[1]);
			printf("%d ",t->chave[1]);
			arvore23_imprime(t->filho[2]);
		}
		printf(") ");
    }
}

void arvore23_imprime_identado(Arvore23 t){
	 arvore23_imprime_identado_aux(t, 0);
}
void arvore23_imprime_identado_aux(Arvore23 t, short int s){
	int i;
	for(i = 0; i < t->nchaves; i++){
		arvore23_imprime_identado_aux(t->filho[i], s+1);
		spaces(s); printf("%d ",t->chave[i]);
	}
	if(t->filho[i]) 
		arvore23_imprime_identado_aux(t->filho[i], s+1);
}
void spaces(short int s){
	while(s--){
		printf(" ");
	}
}