#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POST(L) ((L)->posterior)
#define ANT(L) ((L)->anterior)
#define LINHA(L) ((L)->linha)
#define COMPANHIA(L) ((L)->companhia)
#define PARADAS(L) ((L)->paradas)
#define PONTO(L) ((L)->ponto)
#define CHEGADA(L) ((L)->chegada)
#define SAIDA(L) ((L)->saida)

typedef enum {ERROR, OK} status;
typedef enum {FALSE = 0, TRUE = 1} Bool;
typedef struct celulaD CelulaD, *Dupla;
typedef struct celula Celula, *Lista;
typedef struct{
    int hora;
    int minuto;
}Horario;

struct celulaD {
    char* ponto;
    Horario chegada;
    Horario saida;
    Dupla posterior;
    Dupla anterior;

};

struct celula {
    int linha;
    char* companhia;
    Lista posterior;
    Dupla paradas;
};

//Verifica se a linha tem 5 digitos (X)
Bool linha_valida (int linha){
    if(linha>= 10000 && linha<=99999) return TRUE;
    else return FALSE;
}

//Inicializar a lista simples e dupla (X)
status inicializa_lista(Lista *p_L){
    *p_L = NULL;
    return OK;
}

status inicializa_dupla(Dupla *p_D){
    *p_D = NULL;
    return OK;
}

//Liberar memória da celula de lista simples e dupla (X)
status libera_lista (Lista *p_L){
    free(*p_L);
    *p_L = NULL;
    return OK;
}

status libera_dupla (Dupla *p_D){
    free(*p_D);
    *p_D = NULL;
    return OK;
}

//Verifica se a lista é vazia.
Bool lista_vazia (Lista L){
    if (L == NULL)
        return TRUE;
    else
        return FALSE;
}

//Verifica se a lista dupla é vazia.
Bool dupla_vazia (Dupla D){
    if(D == NULL)
        return TRUE;
    else
        return FALSE;
}

//Aloca memória para célula de lista simples ou dupla.
status aloca_celula (Lista *p_L, int linha, char *companhia){
    Lista L = (Lista)malloc (sizeof(Celula));
    if(L == NULL) return ERROR;
    LINHA(L) = linha;
    COMPANHIA(L) = (char*)malloc(sizeof(char)*(strlen(companhia)+1));
    if(COMPANHIA(L) == NULL) return ERROR;
    strcpy(COMPANHIA(L), companhia);
    POST(L) = NULL;
    PARADAS(L) = NULL;
    *p_L = L;
    return OK;
}

status aloca_celulaD (Dupla *p_D, char *ponto, Horario chegada, Horario saida){
    Dupla D;
    inicializa_dupla(&D);
    D = (Dupla)malloc (sizeof (CelulaD));
    if (D == NULL) return ERROR;
    *p_D = D;
    PONTO(D) = (char*)malloc(sizeof(char)*(strlen(ponto)+1));
    if (PONTO(D) == NULL) return ERROR;
    strcpy (PONTO(D), ponto);
    CHEGADA(D).hora = chegada.hora;
    CHEGADA(D).minuto = chegada.minuto;
    SAIDA(D).hora = saida.hora;
    SAIDA(D).minuto = saida.minuto;
    POST(D) = NULL;
    ANT (D) = NULL;
    return OK;
}
//Verifica a unicidade de uma linha
Bool verifica_unicidade (Lista L, int linha){
    Lista aux;
    for(aux=L;aux!=NULL;aux=POST(aux))
        if(LINHA(aux) == linha) return FALSE;
    return TRUE;
}
//Cadastra uma linha (ordenada).
status cadastra_linha (Lista *p_L, int linha, char *companhia){
    if(linha_valida(linha) == FALSE) return ERROR;
    if(verifica_unicidade(*p_L,linha) == FALSE){
        printf("Número de linha já existente.\n");
        return ERROR;
    }
    Lista L;
    if( aloca_celula(&L,linha,companhia) == ERROR) return ERROR;
    if(lista_vazia(*p_L) == TRUE) *p_L = L;
    else{
        Lista aux;
        if(linha<LINHA(*p_L)){
            POST(L) = *p_L;
            *p_L = L;
            return OK;
        }
        for(aux = *p_L;POST(aux) != NULL && linha> LINHA(POST(aux));aux=POST(aux));
        if(POST(aux) == NULL)
            POST(aux) = L;
        else{
            POST(L) = POST(aux);
            POST(aux) = L;
        }
    }
    return OK;
}

//Encontra uma linha na lista, retornando OK se for válida e o ponteiro apontando para ela.
Bool encontra_linha (Lista *p_L, int linha){
    Lista L;
    for(L=*p_L;L!=NULL;L=POST(L)){
        if(linha == LINHA(L)){
            *p_L = L;
            return TRUE;
        }
    }
    return FALSE;
}

//Compara dois horários. Se o primeiro é menor, retorna 1, se são iguais, retorna 0, se o primeiro é maior retorna -1.
int compara_horario (Horario primeiro, Horario segundo){
        int resp;
        if(primeiro.hora < segundo.hora)
            resp = 1;
        else if(primeiro.hora == segundo.hora){
            if(primeiro.minuto < segundo.minuto)
                resp = 1;
            else if(primeiro.minuto == segundo.minuto)
                resp = 0;
            else
                resp = -1;
        }
        else
            resp = -1;
        return resp;
}

//Cadastra uma parada (ordenada).
status cadastra_ponto (Lista *p_L, int linha, char *ponto, Horario chegada, Horario saida){
    if (lista_vazia(*p_L) == TRUE) return ERROR;
    if (linha_valida (linha) == FALSE) return ERROR;
    Lista aux = *p_L;
    if (encontra_linha(&aux, linha) == FALSE) return ERROR;

    Dupla D, auxD, *p_D = &PARADAS(aux);
    if(aloca_celulaD(&D,ponto,chegada,saida) == ERROR) return ERROR;
    if(dupla_vazia(*p_D) == TRUE){
        *p_D = D;
        POST(D) = D;
        ANT(D) = D;
    }
    else{
        for (auxD = *p_D; POST (auxD) != *p_D; auxD = POST(auxD))
            if ((compara_horario(chegada, CHEGADA(auxD)) == 0) || (compara_horario (saida, SAIDA(auxD)) == 0) ||
            (compara_horario (chegada, SAIDA(auxD)) == 0) || (compara_horario (saida, CHEGADA(auxD)) == 0)){
                printf("Horário de saida ou de chegada já é existente.\n");
                libera_dupla (&D);
                return ERROR;
            }
        if ((compara_horario(chegada, CHEGADA(auxD)) == 0) || (compara_horario (saida, SAIDA(auxD)) == 0) ||
        (compara_horario (chegada, SAIDA(auxD)) == 0) || (compara_horario (saida, CHEGADA(auxD)) == 0)){
            printf("Horário de saida ou de chegada já é existente.\n");
            libera_dupla (&D);
            return ERROR;
        }
        if (compara_horario (chegada, saida) == -1){
            if(compara_horario(CHEGADA(*p_D),SAIDA(*p_D))== -1){
                printf("Nao e possivel ter dois pontos finais. Erro ao cadastrar %s na linha %d.\n",ponto,linha);
                return ERROR;
            }
            if((compara_horario(CHEGADA(*p_D),SAIDA(D))== -1) && (compara_horario(SAIDA(ANT(*p_D)),CHEGADA(D))== 1)){
                POST(D) = *p_D;
                ANT(D) = ANT(*p_D);
                POST(ANT(*p_D)) = D;
                ANT(*p_D) = D;
                *p_D = D;
            }
            else{
                printf("O horário não pode ser encaixado entre os pontos existentes.\n");
                libera_dupla(&D);
                return ERROR;
            }
        }
        else{
                if(compara_horario(SAIDA(D),CHEGADA(*p_D)) == -1){
                    printf("O horário de saida em %s é maior que o horário de chegada no ponto final.\n", ponto);
                    libera_dupla(&D);
                    return ERROR;
                }
            for(auxD = *p_D;(POST(auxD) != *p_D) && (compara_horario(CHEGADA(POST(auxD)),SAIDA(D)) == 1);auxD=POST(auxD));
                if(compara_horario(CHEGADA(D),SAIDA(auxD)) == 1){
                    printf("O horario nao pode ser encaixado entre os pontos existentes.");
                    libera_dupla(&D);
                    return ERROR;
                }
                POST(D) = POST(auxD);
                ANT(D) = auxD;
                ANT(POST(auxD)) = D;
                POST(auxD) = D;
        }
    }
    return OK;
}

//Encontra a posição de determinada parada em uma linha.
Bool encontra_ponto (Dupla *p_D, char *ponto, Horario chegada, Horario saida){
    Dupla aux;
    for(aux = *p_D; POST(aux)!= *p_D; aux=POST(aux)){
        if(((strcmp(ponto,PONTO(aux))) == 0)&& (compara_horario(CHEGADA(aux),chegada) == 0) && (compara_horario(SAIDA(aux),saida) == 0)){
            *p_D = aux;
            return TRUE;
        }
    }
    if(((strcmp(ponto,PONTO(aux))) == 0)&& (compara_horario(CHEGADA(aux),chegada) == 0) && (compara_horario(SAIDA(aux),saida) == 0)){
            *p_D = aux;
            return TRUE;
        }
    return FALSE;
}

//Elimina uma parada de uma linha.
status remove_parada (Lista *p_L, int linha, char *ponto, Horario chegada, Horario saida){
    if(linha_valida(linha) == FALSE) return ERROR;
    Dupla D;
    Lista L = *p_L;
    if(encontra_linha(&L,linha) == FALSE){
        printf("Linha informada não foi encontrada.\n");
        return ERROR;
    }
    if(dupla_vazia(PARADAS(L)) == TRUE){
        printf("Não foi encontrado um ponto com essas especificacoes.\n");
        return ERROR;
    }
    D = PARADAS(L);
    if(encontra_ponto(&D,ponto,chegada, saida) == FALSE){
        printf("Não foi encontrado um ponto com essas especificacoes.\n");
        return ERROR;
    }
    POST(ANT(D)) = POST(D);
    ANT(POST(D)) = ANT(D);
    if(D == POST(D)) PARADAS(L) = NULL;
    if(D == PARADAS(L)) PARADAS(L) = POST(D);
    libera_dupla(&D);
    return OK;
}

//Elimina uma linha de ônibus.
status remove_linha (Lista *p_L, int linha){
    if(linha_valida(linha) == FALSE){
        printf("A linha informada tem um formato invalido.\n");
        return ERROR;
    }
    Lista L, auxL;
    Dupla D, auxD;
    L = *p_L;
    if(encontra_linha(&L,linha) == FALSE){
        printf("A linha informada nao foi encontrada.\n");
        return ERROR;
    }
    D = PARADAS(L);
    auxD = D;
    if(dupla_vazia(PARADAS(L)) == FALSE){
        for(auxD = POST(D); auxD!=PARADAS(L);){
            D = POST(D);
            libera_dupla(&auxD);
            auxD = D;
        }
        libera_dupla(&auxD);
    }
    if(L == *p_L)
        *p_L = POST(*p_L);
    else{
        for(auxL= *p_L;POST(auxL) !=NULL && POST(auxL)!=L;auxL=POST(auxL));
        POST(auxL) = POST(L);
    }
    libera_lista(&L);
    return OK;
}

//Destroi todas as linhas
void destroi_memoria (Lista *p_L){
    while(*p_L!=NULL){
        remove_linha(p_L,LINHA(*p_L));
    }
}
//Altera conteúdo de uma parada.
status altera_parada (Lista *p_L, int linha, char *ponto_ant, Horario chega_ant, Horario sai_ant, char *ponto_novo, Horario chega_novo, Horario sai_novo){
    if(remove_parada(p_L, linha, ponto_ant, chega_ant, sai_ant)== ERROR) return ERROR;
    if(cadastra_ponto(p_L,linha,ponto_novo,chega_novo,sai_novo) == ERROR){
        if(cadastra_ponto(p_L,linha,ponto_ant,chega_ant,sai_ant) == OK){
            printf("Nao foi possivel alterar os dados.\n");
            return ERROR;
        }
        printf("Nao foi possivel alterar os dados e os antigos foram perdidos.\n");
        return ERROR;
    }
    return OK;
}

//Verifica se o horário é válido.
Bool verifica_horario (Horario horario){
    if(horario.hora <0 || horario.hora > 23) return FALSE;
    if(horario.minuto < 0 || horario.minuto > 59) return FALSE;
    return TRUE;
}

