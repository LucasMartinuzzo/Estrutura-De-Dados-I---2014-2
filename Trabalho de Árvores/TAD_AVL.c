#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef enum {OK, ERROR} Status;
typedef enum {FALSE = 0, TRUE = 1} Bool;
typedef struct noAVL NoAVL, *ArvoreAVL;
typedef long int Dado;

struct noAVL{
        Dado conteudo;
        ArvoreAVL esquerda;
        ArvoreAVL direita;
        Dado altura;
};

#define CONTEUDO(T) ((T)->conteudo)
#define ESQUERDA(T) ((T)->esquerda)
#define DIREITA(T)  ((T)->direita)
#define ALTURA(T) ((T)->altura)
#define MAX 32768

//Aloca um nó de uma árvore AVL para armazenar um número inteiro
Status aloca_AVL (ArvoreAVL *p_T, Dado conteudo){
        ArvoreAVL T = (ArvoreAVL)malloc (sizeof (NoAVL));
        if (T == NULL) return ERROR;
        *p_T = T;
        CONTEUDO(T) = conteudo;
        ESQUERDA(T) = NULL;
        DIREITA(T)  = NULL;
        ALTURA(T) = 1;
        return OK;
}

//Libera a memória de um nó de uma árvore AVL
void libera_AVL (ArvoreAVL *p_T){
        free (*p_T);
        *p_T = NULL;
}

//Inicializa uma árvore AVL, apontando-a para NULL
void inicia_AVL (ArvoreAVL *p_T){
        *p_T = NULL;
}

//Verifica se uma árvore AVL está vazia
Bool AVL_vazia (ArvoreAVL T){
        return (T == NULL)?TRUE:FALSE;
}

Dado maximo (Dado a, Dado b){
    return (a > b)?a:b;
}

Dado altura (ArvoreAVL T){
    if (AVL_vazia(T)) return 0;
    return ALTURA(T);
}

Dado fator_balanceamento (ArvoreAVL T){
    if (AVL_vazia(T)) return 0;
    return altura(ESQUERDA(T)) - altura(DIREITA(T));
}

Status rotacao_esquerda (ArvoreAVL *p_T){
    ArvoreAVL X = *p_T, Y = DIREITA(X), aux = ESQUERDA(Y);
    ESQUERDA(Y) = X;
    DIREITA(X) = aux;
    ALTURA(X) = maximo (altura(ESQUERDA(X)), altura(DIREITA(X))) + 1;
    ALTURA(Y) = maximo (altura(ESQUERDA(Y)), altura(DIREITA(Y))) + 1;
    *p_T = Y;
    return OK;
}

Status rotacao_direita (ArvoreAVL *p_T){
    ArvoreAVL Y = *p_T, X = ESQUERDA(Y), aux = DIREITA(X);
    DIREITA(X) = Y;
    ESQUERDA(Y) = aux;
    ALTURA(Y) = maximo (altura(ESQUERDA(Y)), altura(DIREITA(Y))) + 1;
    ALTURA(X) = maximo (altura(ESQUERDA(X)), altura(DIREITA(X))) + 1;
    *p_T = X;
    return OK;
}

Status insere_AVL (ArvoreAVL *p_T, Dado conteudo){
    ArvoreAVL T = *p_T; Dado fb;
    if (AVL_vazia(T)){
        if (aloca_AVL (&T, conteudo) == ERROR) return ERROR;
        *p_T = T;
        return OK;
    }
    if(conteudo == CONTEUDO(T)){
        libera_AVL(&T);
        return ERROR;
    }
    else if (conteudo < CONTEUDO(T))
        insere_AVL(&ESQUERDA(T), conteudo);
    else
        insere_AVL (&DIREITA(T), conteudo);
    ALTURA(T) = maximo (altura(ESQUERDA(T)), altura(DIREITA(T))) + 1;
    fb = fator_balanceamento (T);
    if (fb > 1 && conteudo < CONTEUDO(ESQUERDA(T))){
        rotacao_direita (&T);
        *p_T = T;
        return OK;
    }
    if (fb < -1 && conteudo > CONTEUDO(DIREITA(T))){
        rotacao_esquerda (&T);
        *p_T = T;
        return OK;
    }
    if (fb > 1 && conteudo > CONTEUDO(ESQUERDA(T))){
        rotacao_esquerda(&ESQUERDA(T));
        rotacao_direita(&T);
        *p_T = T;
        return OK;
    }
    if (fb < -1 && conteudo < CONTEUDO(DIREITA(T))){
        rotacao_direita(&DIREITA(T));
        rotacao_esquerda(&T);
        *p_T = T;
        return OK;
    }
}
//Busca um dado nó em uma árvore
Status busca_AVL (ArvoreAVL *p_T, ArvoreAVL *resp, Dado conteudo){
    ArvoreAVL T = *p_T;
    if (AVL_vazia(T)) return ERROR;
    while (T != NULL){
        if (conteudo == CONTEUDO(T)){
            *resp = T;
            return OK;
        }
        if (conteudo < CONTEUDO(T))
            T = ESQUERDA(T);
        else
            T = DIREITA(T);
    }
    return ERROR;
}

//Encontra o menor elemento da árvore
Status minimo_AVL (ArvoreAVL *p_T,ArvoreAVL *resp){
    ArvoreAVL T = *p_T;
    if(AVL_vazia(T)) return ERROR;
    while(ESQUERDA(T) != NULL)
        T = ESQUERDA(T);
    *resp = T;
    return OK;
}

//Remove o nó de um dado elemento da árvore
Status remove_AVL (ArvoreAVL *p_T, Dado conteudo){
    ArvoreAVL T = *p_T, temp; int fb;
    if(AVL_vazia(T)) return ERROR;
    if(conteudo < CONTEUDO(T))
        remove_AVL(&ESQUERDA(T), conteudo);
    else if(conteudo > CONTEUDO(T))
        remove_AVL(&DIREITA(T),conteudo);
    else {
        if(ESQUERDA(T) == NULL || DIREITA(T) == NULL){
            temp = ESQUERDA(T)?ESQUERDA(T):DIREITA(T);
            if(AVL_vazia(temp)){
                temp = T;
                T = NULL;
            }
            else
                *T = *temp;
            libera_AVL(&temp);
        }
        else {
            minimo_AVL(&DIREITA(T),&temp);
            CONTEUDO(T) = CONTEUDO(temp);
            remove_AVL(&DIREITA(T),CONTEUDO(temp));
        }
    }
    if(AVL_vazia(T)){
        *p_T = T;
        return OK;
    }
    ALTURA(T) = maximo(altura(ESQUERDA(T)),altura(DIREITA(T)))+1;
    fb = fator_balanceamento(T);
    if(fb > 1 && fator_balanceamento(ESQUERDA(T)) >= 0){
        rotacao_direita(&T);
        *p_T = T;
        return OK;
    }
    if(fb > 1 && fator_balanceamento(ESQUERDA(T)) < 0){
        rotacao_esquerda(&ESQUERDA(T));
        rotacao_direita(&T);
        *p_T = T;
        return OK;
    }
    if(fb < -1 && fator_balanceamento(DIREITA(T))<= 0){
        rotacao_esquerda(&T);
        *p_T = T;
        return OK;
    }
    if(fb < -1 && fator_balanceamento(ESQUERDA(T)) > 0){
        rotacao_direita(&DIREITA(T));
        rotacao_esquerda(&T);
        *p_T = T;
        return OK;
    }
    *p_T = T;
}

//Imprime
Status imprime_teste (ArvoreAVL T){
    if (AVL_vazia(T)) return ERROR;
        imprime_teste(ESQUERDA(T));
        printf ("%ld\t", CONTEUDO(T));
        printf("A:%ld\t",ALTURA(T));
        if(ESQUERDA(T) != NULL) printf ("E:%ld\t", CONTEUDO(ESQUERDA(T)));
        if(DIREITA(T) != NULL) printf ("D:%ld\t", CONTEUDO(DIREITA(T)));
        printf("\n");
        imprime_teste (DIREITA(T));
        return OK;
}
int main(){
    ArvoreAVL T, N;
    inicia_AVL(&T);
    inicia_AVL (&N);
    //Trabalho
    /*/
    Dado *v = (Dado*)malloc(sizeof(Dado)*MAX);
    Dado i = 1,j = 0, k;
    for(k=0; k<MAX; k++){
        v[k] = k+1;
    }
    srand((unsigned)time(NULL));
    for(k=0; k<MAX; k++){
        int r = rand()%MAX;
        int temp = v[k];
        v[k] = v[r];
        v[r] = temp;
    }
    clock_t tempo;
    tempo = clock();
    for(i=0; i < MAX; i++){
        insere_AVL(&T,v[i]);
    }
    printf("Aleatoria:\n");
    printf("Inserir: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    srand((unsigned)time(NULL));
    for(k=0; k<MAX; k++){
        int r = rand()%MAX;
        int temp = v[k];
        v[k] = v[r];
        v[r] = temp;
    }
    tempo = clock();
    for(i=0;i < MAX;i++){
        busca_AVL(&T,&N,v[i]);
    }
    printf("Buscar:  %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    srand((unsigned)time(NULL));
    for(k=0; k<MAX; k++){
        int r = rand()%MAX;
        int temp = v[k];
        v[k] = v[r];
        v[r] = temp;
    }
    tempo = clock();
    for(i=0;i<MAX;i++){
        remove_AVL(&T,v[i]);
    }
    printf("Remover: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    printf("Ordenada:\n");
    tempo = clock();
    for(i=0; i < MAX; i++){
        insere_AVL(&T,i);
    }
    printf("Inserir: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    tempo = clock();
    for(i=0;i < MAX;i++){
        busca_AVL(&T,&N,i);
    }
    printf("Buscar:  %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    tempo = clock();
    for(i=0;i<MAX;i++){
        remove_AVL(&T,i);
    }
    printf("Remover: %f\n",(clock() - tempo)/(double)CLOCKS_PER_SEC);
    /**/
    //Teste de implementação
    /**/
    printf("Insere a arvore:\n");
    insere_AVL(&T,50);
    insere_AVL(&T, 17);
    insere_AVL(&T, 72);
    insere_AVL(&T, 12);
    insere_AVL(&T, 23);
    insere_AVL(&T,9);
    insere_AVL(&T,14);
    insere_AVL(&T,19);
    insere_AVL(&T,54);
    insere_AVL(&T,76);
    insere_AVL(&T,67);
    imprime_teste(T);
    system("pause");
    system("cls");
    printf("Busca um elemento e imprime sua sub-arvore se encontrado:\n");
    if(busca_AVL(&T,&N,72) == OK){
        printf("Elemento encontrado.\n");
        imprime_teste(N);
    }
    else
        printf("Elemento nao encontrado.\n");
    system("pause");
    system("cls");
    printf("Remove um elemento, imprime a nova arvore e busca o mesmo para verificar se foi removido.\n");
    remove_AVL(&T,72);
    imprime_teste(T);
    if(busca_AVL(&T,&N,72) == OK){
        printf("Elemento encontrado.\n");
        imprime_teste(N);
    }
    else
        printf("Elemento nao encontrado.\n");
    /**/
    system("pause");
    return 0;
}
