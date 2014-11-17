/* Nome: Lucas Alves Racoci
 * RA: 156331 
 * Laboratorio 07 - Cobertura de Conjuntos */

#include <stdio.h>
#include <stdlib.h>


/* STRUCTS, DEFINIÇÕES E CABEÇALHOS DAS MINHAS FUNÇÕES DA TAD */
/*************************************************************/

typedef enum bool{false, true} bool;

typedef struct Arvore{
	int info;
	struct Arvore *esq, *dir, *pai;
} NoArvBin, *Arvore;

typedef struct Set{
	int tam;
	Arvore arv;
} *Set, Conjunto;

typedef struct VetInt{
	int tam;
	bool cobre;
	int *v;
} VetInt;

/* TAD: Arvore Binária de Busca por Afunilamento */
/************************************************/

bool insere_arv_bin(Arvore * Raiz, int chave, NoArvBin** onde);
bool remove_arv_bin(Arvore *Raiz, int chave,  NoArvBin** onde);
void copia_arvore(Arvore* A, Arvore B);
bool afunila(NoArvBin * f);
void rotacao(NoArvBin * f) ;
void libera_arvore(Arvore X);

/* Funções de Debugar */
void pArv(Arvore A);
void pPre(Arvore A);
void pIn(Arvore A);
void confere_arvore(Arvore A, char *str);
Arvore raiz(NoArvBin* p);	

/* TAD: Conjuntos */
/*****************/
Set cria_conjunto();
bool insira(Set C, int chave);
bool remova(Set C, int chave);
void copia_conjunto(Set *A, Set B);
int maior(Set *S, int n);
void complemento(Set A, Set B);
void libera_conjunto(Set X);

/* Funções de Debugar */
void pSet(Set* x, int n, char *str);


/* ASSINATURA DE OUTRAS FUNÇÕES */ 
/*******************************/

void inicializa_universo(Set U, int u);
void imprime_final(VetInt R);
void salva_elementos(Set A, VetInt * A_elem);
void cobertura(Set *S, int n, Set U, VetInt * R);

/* Deletar */
void salva_elementos_pre_ordem(Arvore A, VetInt * A_elem);


/* MEU PROGRAMA - MAIN */
/**********************/
/*********************/

int main() { 
	int n, u;

	int i;
	int atual;
	char c;

	VetInt resposta;

	Set *subsets, universe;

	universe = cria_conjunto();

	scanf("%d %d\n", &u, &n);

	inicializa_universo(universe, u);

	resposta.v = (int*)malloc(n * sizeof(int));
	resposta.tam = 0;
	resposta.cobre = true;

	subsets = (Set*)malloc(n * sizeof(Set));

	for(i = 0; i < n; i++){
		subsets[i] = cria_conjunto();
		scanf("s");
		while(scanf("%c", &c) != EOF && c == ' '){
			scanf("%d", &atual);
			insira(subsets[i], atual);
		}
	}
	cobertura(subsets, n, universe, &resposta);
	/* A Resposta final de se há ou não cobertura 
	 * é dada pelo tamnho final do universo */
	resposta.cobre = universe->tam == 0;

	imprime_final(resposta);
	free(resposta.v);
	
	libera_conjunto(universe);
	for(i = 0; i < n; i++){
		libera_conjunto(subsets[i]);
	}
	free(subsets);

	return 0;
} 

/* IMPLEMENTAÇÃO DES OUTRAS FUNÇÕES */ 
/***********************************/
/**********************************/

void inicializa_universo(Set U, int u){
	int i;
	for(i = 1; i <= u; i++){
		insira(U, i);
	}
}
void cobertura(Set *S, int n, Set U, VetInt * R){
	int i, j, m; 
	Set M = NULL;
	/* Este loop roda no máximo n vezes, porque só 
	 * se tem n subconjuntos pra testar */
	for(i = 0; i < n; i++){
		/* Retorna -1 caso não haja subsets */
		m = maior(S, n);
		if(m == -1){
			return;
		}
		M = S[m];
		/* Se o maior conjunto for o vazio, a função também termina*/
		if(M->tam == 0) {
			return;
		}
		/* Caso nenhuma das condições de parada aconteça, o algoritmo
		 * prosegue e o m-ésimo elemento é retirado da coleção S */
		S[m] = NULL;
		/* Para cada elemento da coleção */
		for(j = 0; j < n; j++){
			/* Se ele ainda existir e não for vazio */
			if(S[j] && S[j]->tam) 
				/* Retira-se dele os elementos de M */
				complemento(S[j], M);
		}
		/* Depois retira-se de U ose elementos de M */
		complemento(U, M);

		/* Acrecenta-se em R o indice de M, m */
		R->v[R->tam++] = m + 1;

		libera_conjunto(M);
	}
}
void imprime_final(VetInt R){
	int i;
	if(R.cobre && R.tam > 0){
		printf("S%d ",R.v[0]);
		for(i = 1; i < R.tam; i++){
			printf("U S%d ",R.v[i]);
		}
	 	printf(":)\n");
	 }
	else printf(":(\n");
}
/*void salva_elementos(Set B, VetInt * A_elem){
	Set A;
	copia_conjunto(&A, B);
	while(A->tam > 0){
		if(A->arv){
			A_elem->v[A_elem->tam++] = A->arv->info;
			remova(A, A->arv->info);
		}
	}
	libera_conjunto(A);
}*/
void salva_elementos(Set B, VetInt * A_elem){
	salva_elementos_pre_ordem(B->arv, A_elem);
}
void salva_elementos_pre_ordem(Arvore A, VetInt * A_elem){
	if(A){
		A_elem->v[A_elem->tam++] = A->info;
		salva_elementos_pre_ordem(A->esq, A_elem);
		salva_elementos_pre_ordem(A->dir, A_elem);
	}
}

/* IMPLEMENTAÇÃO DAS FUNÇÕES DE MINHA TAD */ 
/*****************************************/
/****************************************/

/* TAD: Conjuntos */
/*****************/

Set cria_conjunto(){
	Set novo = (Set)malloc(sizeof(Conjunto));
	novo->tam = 0;
	novo->arv = NULL;
	return novo;
}
bool insira(Set C, int chave){
	NoArvBin *futura_raiz;
	bool inserivel = insere_arv_bin(&C->arv, chave, &futura_raiz);
	if(!inserivel) return false;
	
	/* A função afunila só retorna falso quando não é mais possível afunilar
	 * ou seja quando seu argumento fora a raiz */
	while(afunila(futura_raiz));

	C->arv = futura_raiz;
	(C->tam)++;


	return true;
}
bool remova(Set C, int chave){
	NoArvBin *futura_raiz;
	bool removivel = remove_arv_bin(&C->arv, chave, &futura_raiz);
	if(!removivel) return false;

	while(afunila(futura_raiz));

	C->arv = futura_raiz;
	C->tam--;


	return true;
}
void copia_conjunto(Set *A, Set B){
	if(B){
		*A = (Set)malloc(sizeof(Conjunto));
		(*A)->tam = B->tam;
		copia_arvore(&((*A)->arv), B->arv);
	}else{
		*A = NULL;
	}
}
int maior(Set *S, int n){
	int i, max;
	for(max = n-1; max >= 0; max--){
		if(S[max] && S[max]->tam){
			break;
		}
	}
	for(i = max; i >= 0; i--){
		if(S[i] && S[max]->tam < S[i]->tam)
			max = i;

	}
	return max;
}
void complemento(Set A, Set B){
	int i;
	VetInt B_elem;
	B_elem.v = (int*)malloc(B->tam * sizeof(int));
	B_elem.tam = 0;
	salva_elementos(B, &B_elem);
	for(i = 0; i < B_elem.tam; i++){
		remova(A, B_elem.v[i]);
	}
}
void libera_conjunto(Set X){
	if(X){
		libera_arvore(X->arv);
		free(X);
	}
}
void pSet(Set* x, int n, char *str){
	int i;
	for(i = 0; i < n; i++){
		printf("\033[93m%s[%d]\033[97m:\n", str, i+1);
		if(x[i])pArv(x[i]->arv);
		printf("\n");
	}
}


/* TAD: Arvore Binária de Busca por Afunilamento */
/************************************************/

bool insere_arv_bin(Arvore * Raiz, int chave, NoArvBin** onde){
	NoArvBin *temp = NULL;
	Arvore *p = Raiz;
	while (*p != NULL){
		if(chave < (*p)->info){
			temp = (*p);
			p = &((*p)->esq);
		}else if(chave > (*p)->info){
			temp = (*p);
			p = &((*p)->dir);
		}else{
			return false;
		}
	}
	(*p) = (Arvore)malloc(sizeof(NoArvBin));
	(*p)->info = chave;
	(*p)->esq = NULL;
	(*p)->dir = NULL;
	(*p)->pai = temp;
	*onde = *p;
		
	return true;
}
bool remove_arv_bin(Arvore *Raiz, int chave,  NoArvBin** onde){
	/* subs acha um substituto pra alvo */
	NoArvBin *subs = NULL, *alvo = *Raiz;
	if(*Raiz == NULL) 
		return false;
	if((*Raiz)->info == chave){
		alvo = *Raiz;
	}
	/* alvo procura nó a ser removido */
	else {
		while(alvo != NULL && alvo->info != chave){
			if(chave > alvo->info){
				alvo = alvo->dir;
			}else{
				alvo = alvo->esq;
			}
		}if(alvo == NULL) 
			return false;
	}
	/* Se alvo só tem um filho */
	if(alvo->dir == NULL){
		subs = alvo->esq;
		if(alvo->pai){
			if(alvo->pai->dir == alvo) 
				alvo->pai->dir = subs;
			else 
				alvo->pai->esq = subs;
		}
		if(subs) 
			subs->pai = alvo->pai;

	}else if(alvo->esq == NULL) {
		subs = alvo->dir;

		if(alvo->pai){
			if(alvo->pai->dir == alvo) 
				alvo->pai->dir = subs;
			else 
				alvo->pai->esq = subs;
		}
		if(subs) 
			subs->pai = alvo->pai;

	/* Se tem os dois filhos, procura pelo mais a dir da subarvore esq */
	}else{
		for(subs = alvo->esq; subs->dir != NULL; subs = subs->dir);

		subs->dir = alvo->dir;
		
		alvo->dir->pai = subs;
		alvo->esq->pai = subs;
		if(alvo->pai){
			if(alvo->pai->dir == alvo) 
				alvo->pai->dir = subs;
			else 
				alvo->pai->esq = subs;
		}
		
		if(subs == alvo->esq)
			alvo->esq = subs->esq;
		else	
			subs->pai->dir = subs->esq;
		
		if(subs->esq)
			subs->esq->pai = subs->pai;
		
		subs->pai = alvo->pai;
		subs->esq = alvo->esq;
		
	}
	if(alvo == *Raiz){
		*Raiz = subs;
		*onde = subs;
	}else{
		*onde = alvo->pai;
	}
	free(alvo);
	return true;
}
void copia_arvore(Arvore * A, Arvore B){
	*A = NULL;
	if(B){
		*A = (Arvore) malloc(sizeof(NoArvBin));
		(*A)->info = B->info;
		copia_arvore(&((*A)->esq), B->esq);
		copia_arvore(&((*A)->dir), B->dir);
	}
}
bool afunila(NoArvBin * f){
	NoArvBin *p, *a;
	if (f == NULL) return false;
	else p = f->pai;
	
	if(p == NULL) return false;
	else a = p->pai;

	if(a == NULL){
		rotacao(f);
	}else{
		/* Se for homagêneo */
		if((a->dir == p && p->dir == f) || (a->esq == p && p->esq == f)){
			rotacao(p);
			rotacao(f);
		}else{
			rotacao(f);
			rotacao(f);
		}
	}
	return true;
}
void rotacao(NoArvBin * f) {
	/* Nesse caso f é o filho esquerdo */
	if(f->pai->esq == f){
		if(f->dir) f->dir->pai = f->pai;
		f->pai->esq = f->dir;
		f->dir = f->pai;
		f->pai = f->dir->pai;
		if(f->pai){
			if(f->pai->dir == f->dir) f->pai->dir = f;
			else f->pai->esq = f;
		}
		f->dir->pai = f;
	}else{
		if(f->esq) f->esq->pai = f->pai;
		f->pai->dir = f->esq;
		f->esq = f->pai;
		f->pai = f->esq->pai;
		if(f->pai){
			if(f->pai->dir == f->esq) f->pai->dir = f;
			else f->pai->esq = f;
		}
		f->esq->pai = f;
	}
}
void libera_arvore(Arvore X){
	if(X){
		libera_arvore(X->dir);
		libera_arvore(X->esq);
		free(X);
	}
}

void pArv(Arvore A){
	if(!A) return;
	pPre(A); printf("\n");
	pIn(A); printf("\n");
}
void pPre(Arvore A){
	if(A){
		printf("%d ", A->info);
		pPre(A->esq);
		pPre(A->dir);
	}
}
void pIn(Arvore A){
	if(A){
		pIn(A->esq);
		printf("%d ", A->info);
		pIn(A->dir);
	}
}
void confere_arvore(Arvore A, char *str){
	if(!A)
		return;
	if(A->esq){
		if(A->esq->pai != A){
			printf("\033[91m%s\n", str);
			printf("(%d)->pai != (%d), mas (%d)->esq == (%d)\n", 
				   A->esq->info, A->info, A->info, A->esq->info);
		}if(A->esq->info > A->info){
			printf("\033[91m%s\n", str);
			printf("%d > %d, mas (%d)->esq == (%d)\n", 
				   A->esq->info, A->info, A->info, A->esq->info);
		}
	confere_arvore(A->esq, str);
	}if(A->dir){
		if(A->dir->pai != A){
			printf("\033[91m%s\n", str);
			printf("(%d)->pai != (%d), mas (%d)->dir == (%d)\n", 
				   A->dir->info, A->info, A->info, A->dir->info);
		}if(A->dir->info < A->info){
			printf("\033[91m%s\n", str);
			printf("%d < %d, mas (%d)->dir == (%d)\n", 
				   A->dir->info, A->info, A->info, A->dir->info);
		}
	confere_arvore(A->dir, str);
	}
	printf("\033[97m");
}
Arvore raiz(NoArvBin* p){
	if(p) 
		while(p->pai){
			p = p->pai;
		}
	return p;
}