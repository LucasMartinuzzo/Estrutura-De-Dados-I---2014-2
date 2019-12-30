#include <stdio.h>
#include <stdlib.h>
#include "TAD_onibus.h"

#define TAM_FILE 260 /*Tamanho máximo para nomes de arquivos contendo sua respectiva extensão no Windows */
enum comandos{CADASTRA_LINHA = 1, CADASTRA_PONTO = 2, ALTERA_PONTO = 3, ELIMINA_PONTO = 4, ELIMINA_LINHA = 5, ID = 6};

status procura_ponto_origem (Dupla Chegada, Dupla *p_Partida, char *origem){
    Dupla D;
    for(D = ANT(Chegada); D!=(Chegada) && (strcmp(PONTO(D),origem) != 0); D = ANT(D));
    if(D == Chegada) return ERROR;
    *p_Partida = D;
    return OK;
}

status procura_ponto_chegada (Dupla *p_D, Dupla *p_Partida, Dupla *p_Chegada, Horario horario, char *origem, char*destino){
    Dupla D;
    for(D=*p_D; POST(D)!=*p_D; D = POST(D)){
        if((strcmp(destino, PONTO(D)) == 0) && (compara_horario(horario,CHEGADA(D)) != -1)){
            *p_Chegada = D;
            if(procura_ponto_origem(D,p_Partida,origem) == ERROR) return ERROR;
            return OK;
        }
    }
    if((strcmp(destino, PONTO(D)) == 0) && (compara_horario(horario,CHEGADA(D)) != -1)){
            *p_Chegada = D;
            if(procura_ponto_origem(D,p_Partida,origem) == ERROR) return ERROR;
            return OK;
    }
    return ERROR;
}

status procura_linha_onibus (Lista *p_L){
    Lista L;
    Dupla chegada, partida;
    char origem[50], destino[50];
    Horario horario;
    printf("Digite a cidade de origem: ");
    scanf("%s",origem);
    printf("Digite a cidade de destino: ");
    scanf("%s",destino);
    if(strcmp(origem,destino)==0){
        printf("A cidade de destino deve ser diferente da cidade de origem.\n");
        return ERROR;
    }
    printf("Digite o horario de chegada (formato hh:mm): ");
    scanf("%d:%d",&horario.hora,&horario.minuto);
    if(verifica_horario(horario) == FALSE){
        printf("Horario invalido.\n");
        return ERROR;
    }
    for(L=*p_L; L!=NULL; L=POST(L)){
        if(procura_ponto_chegada(&PARADAS(L),&partida,&chegada,horario,origem,destino)==OK){
            printf("Horario de partida: %d:%dh.\nHorario de chegada: %d:%dh.\n",SAIDA(partida).hora,SAIDA(partida).minuto,CHEGADA(chegada).hora,CHEGADA(chegada).minuto);
            return OK;
        }
    }
    printf("Nao foi encontrado onibus que atenda as suas especificacoes.\n");
    return OK;
}

//Imprime linhas
void imprime_linhas(Lista L){
    Dupla D;
    for(L;L!=NULL;L = POST(L)){
        printf("%s\t",COMPANHIA(L));
        printf("%d\n",LINHA(L));
        if(dupla_vazia(PARADAS(L)) == FALSE){
            for(D = PARADAS(L);POST(D) != PARADAS(L); D = POST(D)){
                printf("\t%s\t%02d:%02d\t%02d:%02d\n",PONTO(D),CHEGADA(D).hora, CHEGADA(D).minuto, SAIDA(D).hora,SAIDA(D).minuto);
            }
            printf("\t%s\t%02d:%02d\t%02d:%02d\n",PONTO(D),CHEGADA(D).hora, CHEGADA(D).minuto, SAIDA(D).hora,SAIDA(D).minuto);
        }
    }
    printf("\n");
}

//FUNÇÕES DE ARQUIVO
int compara_comando(char *comando){
    if(strcmp(comando,"CADASTRA_LINHA") == 0) return 1;
    if(strcmp(comando,"CADASTRA_PONTO") == 0) return 2;
    if(strcmp(comando,"ALTERA_PONTO") == 0) return 3;
    if(strcmp(comando,"ELIMINA_PONTO") == 0) return 4;
    if(strcmp(comando,"ELIMINA_LINHA") == 0) return 5;
    if(strcmp(comando,"ID") == 0) return 6;
    return 0;
}
status executa_comando(FILE *fp, Lista *p_L){
    char comando[20], comp[50], ponto[50], pontoV[50], pontoN[50];
    Horario chegadaV, saidaV, chegadaN, saidaN, chegada, saida;
    int linha;
    fscanf(fp,"%s",comando);
    switch (compara_comando(comando)){
        case CADASTRA_LINHA:
            fscanf(fp,"%d %s",&linha,comp);
            if((cadastra_linha(p_L,linha,comp)) == ERROR) return ERROR;
            break;
        case CADASTRA_PONTO:
            fscanf(fp,"%d %s %d:%d %d:%d", &linha, ponto,&chegada.hora, &chegada.minuto, &saida.hora, &saida.minuto);
            if((cadastra_ponto(p_L,linha, ponto, chegada, saida)) == ERROR) return ERROR;
            break;
        case ALTERA_PONTO:
            fscanf(fp,"%d %s %d:%d %d:%d",&linha, pontoV, &chegadaV.hora, &chegadaV.minuto, &saidaV.hora, &saidaV.minuto);
            fscanf(fp,"%s %d:%d %d:%d",pontoN, &chegadaN.hora, &chegadaN.minuto, &saidaN.hora, &saidaN.minuto);
            if((altera_parada(p_L,linha,pontoV,chegadaV,saidaV,pontoN,chegadaN,saidaN)) == ERROR) return ERROR;
            break;
        case ELIMINA_PONTO:
            fscanf(fp,"%d %s %d:%d %d:%d", &linha, ponto,&chegada.hora, &chegada.minuto, &saida.hora, &saida.minuto);
            if((remove_parada(p_L,linha,ponto,chegada,saida)) == ERROR) return ERROR;
            break;
        case ELIMINA_LINHA:
            fscanf(fp,"%d",&linha);
            if((remove_linha(p_L, linha)) == ERROR) return ERROR;
            break;
        case ID:
            fscanf(fp,"%d",&linha); //Só para passar para a próxima linha
            break;
        default:
            printf("Comando invalido.\n");
            return ERROR;
    }
    return OK;
}

status copia_arquivo(FILE *fp){
    FILE *log;
    if((log = fopen("log.txt","a")) == NULL) return ERROR;
    char c = getc (fp);
    char comando[20], linha[150];
    fseek (fp, 0, SEEK_SET);
    while (c != EOF){
        fseek (fp, -sizeof(char), SEEK_CUR);
        fscanf(fp,"%s",comando);
        fgets(linha,150,fp);
        if(strcmp(comando,"ELIMINA_LINHA") != 0 && strcmp(comando,"ELIMINA_PONTO") != 0)
            fprintf(log,"%s %s",comando, linha);
        c = getc (fp);
    }
    fprintf(log,"\n");
    if(fclose(log) == EOF) return ERROR;
    return OK;
}

status ler_arquivo (Lista *p_L, char *arquivo){
    FILE *fp;
    char c, id[20];
    fp = fopen (arquivo, "r");
    if ((fp = fopen (arquivo, "r")) == NULL){
        printf ("Erro na abertura do arquivo.\n");
        return ERROR;
    }
    c = getc (fp);
    fseek (fp, 0, SEEK_SET);
    fscanf(fp,"%s",id);
    if(strcmp(id,"ID") == 0)
        copia_arquivo(fp);
    fseek (fp, 0, SEEK_SET);
    while (c != EOF){
        if(executa_comando (fp, p_L) == ERROR) return ERROR;
        c = getc (fp);
    }
    if(fclose(fp) == EOF) return ERROR;
    return OK;
}

status salva_arquivo(Lista *p_L){
    FILE *fp;
    if ((fp = fopen ("dados.txt", "w")) == NULL){
        printf ("Nao foi possível modificar o arquivo.\n");
        return ERROR;
    }
    Lista L;
    Dupla D;
    for(L = *p_L;L!=NULL;L = POST(L)){
        fprintf(fp,"CADASTRA_LINHA %d %s\n",LINHA(L),COMPANHIA(L));
        if(PARADAS(L) == NULL)
            continue;
        for(D = PARADAS(L);POST(D)!= PARADAS(L); D = POST(D)){
            fprintf(fp,"CADASTRA_PONTO %d %s %d:%d %d:%d\n",LINHA(L),PONTO(D),CHEGADA(D).hora,CHEGADA(D).minuto,SAIDA(D).hora,SAIDA(D).minuto);
        }
        fprintf(fp,"CADASTRA_PONTO %d %s %d:%d %d:%d\n",LINHA(L),PONTO(D),CHEGADA(D).hora,CHEGADA(D).minuto,SAIDA(D).hora,SAIDA(D).minuto);
    }
    if(fclose(fp) == EOF){
        printf("Erro ao salvar o arquivo.\n");
        return ERROR;
    }
    return OK;
}
int menu (){
    int opcao;
    printf("Selecione uma das opcoes abaixo:\n");
    printf("1) Procurar linha de onibus.\n");
    printf("2) Imprimir todas as linhas disponiveis.\n");
    printf("3) Efetuar manutencao do programa.\n");
    printf("4) Sair.\n");
    do{
        printf ("Opcao desejada: ");
        scanf ("%d", &opcao);
        if ((opcao < 1 ) || (opcao > 4))
            printf ("Opcao invalida.\n");
    }while ((opcao < 1) || (opcao > 4));
    return opcao;
}
int main(){
    int opcao;
    char arquivo[TAM_FILE];
    Lista p_L;
    inicializa_lista(&p_L);
    ler_arquivo(&p_L,"dados.txt");
    opcao = menu();
    while(opcao != 4){
        switch(opcao){
            case 1:
                procura_linha_onibus(&p_L);
                break;
            case 2:
                system("cls");
                imprime_linhas(p_L);
                break;
            case 3:
                printf("Digite o nome do arquivo de manutencao: ");
                scanf("%s", arquivo);
                if(ler_arquivo(&p_L,arquivo) == ERROR)
                    printf("Houve um problema. A manutencao foi encerrada.\n");
                else
                    printf("Manutencao realizada com sucesso.\n");
                break;
                salva_arquivo(&p_L);
        }
        system ("pause");
        system("cls");
        opcao = menu ();
    }
    destroi_memoria(&p_L);
    return 0;
}
