#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef enum {OK, ERROR} Status;
typedef enum {FALSE = 0, TRUE = 1} Bool;
typedef struct noBB NoBB, *ArvoreBB;
typedef long int Dado;
struct noBB{
        Dado conteudo;
        ArvoreBB esquerda;
        ArvoreBB direita;
};

#define CONTEUDO(T) ((T)->conteudo)
#define ESQUERDA(T) ((T)->esquerda)
#define DIREITA(T)  ((T)->direita)
#define MAX 32768

//Aloca um nó de uma árvore de busca binária para armazenar um número inteiro
Status aloca_ABB (ArvoreBB *p_T, Dado conteudo){
        ArvoreBB T = (ArvoreBB)malloc (sizeof (NoBB));
        if (T == NULL)
            return ERROR;
        *p_T = T;
        CONTEUDO(T) = conteudo;
        ESQUERDA(T) = NULL;
        DIREITA(T)  = NULL;
        return OK;
}

//Libera a memória de um nó de uma árvore de busca binária
void libera_ABB (ArvoreBB *p_T){
        free (*p_T);
        *p_T = NULL;
}

//Inicializa uma árvore de busca binária, apontando-a para NULL
void inicia_ABB (ArvoreBB *p_T){
        *p_T = NULL;
}

//Verifica se uma árvore de busca binária está vazia
Bool ABB_vazia (ArvoreBB T){
        return (T == NULL)?TRUE:FALSE;
}

//Insere um nó numa árvore de busca binária
Status insere_ABB_i (ArvoreBB *p_T, Dado conteudo){
    ArvoreBB T = *p_T, N, pai;
    pai = NULL;
    aloca_ABB (&N, conteudo);
    while(T !=NULL){
        pai = T;
        if(CONTEUDO(N) == CONTEUDO(T)){
            libera_ABB(&N);
            return ERROR;
        }
        if(CONTEUDO(N) < CONTEUDO(T))
            T = ESQUERDA(T);
        else
            T = DIREITA(T);
    }
    if(ABB_vazia(pai))
        *p_T = N;
    else if(CONTEUDO(N) == CONTEUDO(pai)){
        libera_ABB(&N);
        return ERROR;
    }
    else if (CONTEUDO(N) < CONTEUDO(pai))
        ESQUERDA(pai) = N;
    else
        DIREITA(pai) = N;
    return OK;
}

//Busca um inteiro numa árvore binária e armazena o nó em que ele se encontra
Status busca_ABB (ArvoreBB *p_T, ArvoreBB *resp, ArvoreBB *pai, Dado conteudo){
    ArvoreBB T = *p_T;
    if (ABB_vazia(T)) return ERROR;
    while (T != NULL){
        if (conteudo == CONTEUDO(T)){
            *resp = T;
            return OK;
        }
        *pai = T;
        if (conteudo < CONTEUDO(T))
            T = ESQUERDA(T);
        else
            T = DIREITA(T);
    }
    return ERROR;
}

//Remove o nó que contém um inteiro informado da árvore de busca binária e libera a memória associada a ele
Status remove_ABB (ArvoreBB *p_T, Dado conteudo){
    ArvoreBB T = *p_T, pai, no, sucessor;
    pai = no = NULL;
    if (ABB_vazia(T)) return ERROR;
    if (busca_ABB (&T, &no, &pai, conteudo) == ERROR) return ERROR;
    if ((ESQUERDA(no) != NULL) && (DIREITA(no) != NULL)){ //Se o nó a ser removido tiver dois filhos
        pai = no;
        sucessor = DIREITA(no);
        while (ESQUERDA(sucessor) != NULL){
            pai = sucessor;
            sucessor = ESQUERDA(sucessor);
        }
        CONTEUDO(no) = CONTEUDO(sucessor);
        no = sucessor;
    }
    if ((ESQUERDA(no) == NULL) && (DIREITA(no) == NULL)){ //Se o nó a ser removido não tiver filhos
        if(ABB_vazia(pai)){
            libera_ABB(&no);
            *p_T = NULL;
            return OK;
        }
        if (DIREITA(pai) == no)
            DIREITA(pai) = NULL;
        else
            ESQUERDA(pai) = NULL;
        libera_ABB (&no);
        return OK;
    }
    if ((ESQUERDA(no) == NULL) && (DIREITA(no) != NULL)){ //Se o nó a ser removido só tiver filho à direita
        if(ABB_vazia(pai)){
            *p_T = DIREITA(no);
            libera_ABB(&no);
            return OK;
        }
        if (ESQUERDA(pai) == no)
            ESQUERDA(pai) = DIREITA(no);
        else
            DIREITA(pai) = DIREITA(no);
        libera_ABB (&no);
        return OK;
    }
    if ((ESQUERDA(no) != NULL) && (DIREITA(no) == NULL)){ //Se o nó a ser removido só tiver filho à esquerda
        if(ABB_vazia(pai)){
            *p_T = ESQUERDA(no);
            libera_ABB(&no);
            return OK;
        }
        if (ESQUERDA(pai) == no)
            ESQUERDA(pai) = ESQUERDA(no);
        else
            DIREITA(pai) = ESQUERDA(no);
        libera_ABB (&no);
        return OK;
    }
}

//Imprime os conteúdos dos nós de uma árvore de busca binária, realizando um percurso em ordem
Status ABB_emordem (ArvoreBB T){
    if (ABB_vazia(T)) return ERROR;
        ABB_emordem(ESQUERDA(T));
        printf ("%ld\n", CONTEUDO(T));
        ABB_emordem (DIREITA(T));
        return OK;
}
Status imprime_teste (ArvoreBB T){
    if (ABB_vazia(T)) return ERROR;
        imprime_teste(ESQUERDA(T));
        printf ("%ld\t", CONTEUDO(T));
        if(ESQUERDA(T) != NULL) printf ("E:%ld\t", CONTEUDO(ESQUERDA(T)));
        if(DIREITA(T) != NULL) printf ("D:%ld\t", CONTEUDO(DIREITA(T)));
        printf("\n");
        imprime_teste (DIREITA(T));
        return OK;
}
Dado minimo (ArvoreBB T){
    ArvoreBB N = T;
    if(ESQUERDA(N) != NULL){
            N = ESQUERDA(N);

    }
    return CONTEUDO(N);
}

int main(){
    ArvoreBB T, N, pai;
    inicia_ABB(&T);
    inicia_ABB (&N);
    inicia_ABB(&pai);
    //Trabalho
    /*/
    Dado *v = (Dado*)malloc(sizeof(Dado)*MAX);
    Dado i = 1;
    for(i=0; i<MAX; i++)
        v[i] = i+1;
    srand((unsigned)time(NULL));
    for(i=0; i<MAX; i++){
        int r = rand()%MAX;
        int temp = v[i];
        v[i] = v[r];
        v[r] = temp;
    }
    clock_t tempo;
    tempo = clock();
    for(i=0; i < MAX; i++){
        insere_ABB_i(&T,v[i]);
    }
    printf("Inserir: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    srand((unsigned)time(NULL));
    for(i=0; i<MAX; i++){
        int r = rand()%MAX;
        int temp = v[i];
        v[i] = v[r];
        v[r] = temp;
    }
    tempo = clock();
    for(i=0;i < MAX;i++){
        busca_ABB(&T,&N,&pai,v[i]);
    }
    printf("Buscar:  %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    srand((unsigned)time(NULL));
    for(i=0; i<MAX; i++){
        int r = rand()%MAX;
        int temp = v[i];
        v[i] = v[r];
        v[r] = temp;
    }
    tempo = clock();
    for(i=0;i<MAX;i++){
        remove_ABB(&T,v[i]);
    }
    printf("Remover: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    printf("Ordenada:\n");
    tempo = clock();
    for(i=0; i < MAX; i++){
        insere_ABB_i(&T,i);
    }
    printf("Inserir: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    tempo = clock();
    for(i=0;i < MAX;i++){
        busca_ABB(&T,&N,&pai,i);
    }
    printf("Buscar:  %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    tempo = clock();
    for(i=0;i<MAX;i++){
        remove_ABB(&T,i);
    }
    printf("Remover: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    /**/
    //Teste de implementação
    /**/
    printf("Insere a arvore:\n");
    insere_ABB_i(&T,16);
    insere_ABB_i(&T, 14);
    insere_ABB_i(&T, 20);
    insere_ABB_i(&T, 7);
    insere_ABB_i(&T, 15);
    insere_ABB_i(&T, 18);
    insere_ABB_i(&T, 25);
    insere_ABB_i(&T, 3);
    insere_ABB_i(&T, 24);
    insere_ABB_i(&T, 28);
    insere_ABB_i(&T, 2);
    insere_ABB_i(&T, 4);
    insere_ABB_i(&T, 22);
    insere_ABB_i(&T, 23);
    imprime_teste(T);
    system("pause");
    system("cls");
    printf("Busca um elemento e imprime sua sub-arvore se encontrado:\n");
    if(busca_ABB(&T,&N,&pai,20) == OK){
        printf("Elemento encontrado.\n");
        imprime_teste(N);
    }
    else
        printf("Elemento nao encontrado.\n");
    system("pause");
    system("cls");
    printf("Remove um elemento, imprime a nova arvore e busca o mesmo para verificar se foi removido.\n");
    remove_ABB(&T,20);
    imprime_teste(T);
    if(busca_ABB(&T,&N,&pai,20) == OK){
        printf("Elemento encontrado.\n");
        imprime_teste(N);
    }
    else
        printf("Elemento nao encontrado.\n");
    /**/
    system("pause");
    return 0;
}
