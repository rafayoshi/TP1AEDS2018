// =----------------------------------------------------------=
// =                           AVISO:                         =
// =                   MODIFIQUE ESTE ARQUIVO!                =
// = Aqui, você deve implementar, obrigatoriamente, essas     =
// = duas funções:                                            =
// =     - `a_menor_que_b`: deve retornar quando uma string   =
// =        é menor que a outra (ambos com o mesmo tamanho);  =
// =     - `external_sort`: aqui, você deverá implementar o   =
// =        seu trabalho. Ordene os registros em `input_file` =
// =        e escreva-os em `output_file`, usando somente     =
// =        `memory` KB de memória.                           =
// =                                                          =
// = Instruções:                                              =
// =     - use a função `mathias_malloc` ao invés de          =
// =       `malloc`;                                          =
// =     - use a função `mathias_free` ao invés de `free`;    =
// =     - lembre-se que você tem somente `memory` KB de      =
// =       memória;                                           =
// =     - note que a sua implementação de `a_menor_que_b`    =
// =       afetará o resultado da verificação da resposta.    =
// =                                                          =
// = Dicas:                                                   =
// =     - você pode usar a sua função `a_menor_que_b` em     =
// =       sua implementação da função `external_sort`;       =
// =                                                          =
// =                      BOM TRABALHO!                       =
// =----------------------------------------------------------=

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"

// Parâmetros:
//     - a: uma cadeia de caracteres de tamanho `len`;
//     - b: uma cadeia de caracteres de tamanho `len`;
//     - len: o tamanho (quantidade de bytes) de `a` e `b`.
// Essa função deve retornar 1, se `a` é menor que `b` (`a` deve vir antes de `b` no
// arquivo), e 0 caso contrário.
// Ela também é usada para verificação da ordenação! Então, se você implementá-la
// erroneamente, a verificação também estará errada! 

int cstring_cmp(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

int a_menor_que_b(char* a, char* b, int len) {
    int i;
    for(i = 0; i < len - 1; i++){
        if(a[i] < b[i]){
            return 1;
        }
        else if(a[i] > b[i]){
            return 0;
        }
    }
    return 0;
}

void external_sort(const char* input_file, const char* output_file, unsigned int memory) {
    int numArqs = 0, i, j = 0, tamanhoString;
    FILE *arq = fopen(input_file, "r");
    FILE *fita;
    memory = memory * 1000;
    fscanf(arq, "%d\n", &tamanhoString);
    tamanhoString++;
    size_t elementosMem = memory/sizeof(char);
    char nomeArq[20];
    char *string = malloc(tamanhoString * sizeof(char));
    sprintf(nomeArq, "arq%d.txt", numArqs);
    fita = fopen(nomeArq, "w");
    numArqs++;
    while(fgets(string, tamanhoString, arq))
    {
        if(strcmp(string, "\n") != 0){
            if (j == (elementosMem)/tamanhoString) {
                fclose(fita);
                j = 0;
                sprintf(nomeArq, "arq%d.txt", numArqs);
                fita = fopen(nomeArq, "w");
                numArqs++;
            }
            fprintf(fita, "%s\n", string);
            j++;
        }
    }
    fclose(arq);
    fclose(fita);
    char **arraySort = (char **) malloc((elementosMem * sizeof(char *))/tamanhoString);
    for(i = 0; i < numArqs; i++){
        sprintf(nomeArq, "arq%d.txt", i);
        fita = fopen(nomeArq, "r");
        j = 0;
        while(fgets(string, tamanhoString, fita)){
            if(strcmp(string, "\n") != 0){
                arraySort[j] = (char *) malloc(tamanhoString * sizeof(char *));
                strcpy(arraySort[j], string);
                j++;
            }
        }
        fclose(fita);
        fita = fopen(nomeArq, "w+");
        qsort(arraySort, j, sizeof(char *), cstring_cmp);
        arraySort[j] = '\0';
        j = 0;
        while(arraySort[j] != '\0'){
            fprintf(fita, "%s\n", arraySort[j]);
            j++;
        }
        fclose(fita);
    }
    free(string);
    int arquivosFaltantes = numArqs;
    char *string1 = malloc(tamanhoString * sizeof(char));
    char *string2 = malloc(tamanhoString * sizeof(char));
    FILE *arq1;
    FILE *arq2;
    FILE *merged;
    int status;
    i = 0;
    while(arquivosFaltantes > 1){
        sprintf(nomeArq, "arq%d.txt", i);
        arq1 = fopen(nomeArq, "r");
        i++;
        sprintf(nomeArq, "arq%d.txt", i);
        arq2 = fopen(nomeArq, "r");
        i++;
        sprintf(nomeArq, "arq%d.txt", numArqs);
        merged = fopen(nomeArq, "w");
        status = 0;
        fgets(string1, tamanhoString, arq1);
        fgets(string2, tamanhoString, arq2);
        while(status == 0){
            if(a_menor_que_b(string1, string2, tamanhoString)){
                fprintf(merged, "%s\n", string1);
                if(fgets(string1, tamanhoString, arq1) == NULL){
                    status = 1;
                    fclose(arq1);
                }
                if(strcmp(string1, "\n") == 0){
                    if(fgets(string1, tamanhoString, arq1) == NULL){
                        status = 1;
                        fclose(arq1);
                    }
                }
            }
            else{
                fprintf(merged, "%s\n", string2);
                if(fgets(string2, tamanhoString, arq2) == NULL){
                    status = 2;
                    fclose(arq2);
                }
                if(strcmp(string2, "\n") == 0){
                    if(fgets(string2, tamanhoString, arq2) == NULL){
                        status = 2;
                        fclose(arq2);
                    }
                }
            }
            if(status == 1){
                do{
                    if(strcmp(string2, "\n") != 0){
                        fprintf(merged, "%s\n", string2);
                    }
                }while(fgets(string2, tamanhoString, arq2));
                fclose(arq2);
            }
            else if(status == 2){
                do{
                    if(strcmp(string1, "\n") != 0){
                        fprintf(merged, "%s\n", string1);
                    }
                }while(fgets(string1, tamanhoString, arq1));
                fclose(arq1);
            }
        }
        fclose(merged);
        arquivosFaltantes--;
        numArqs++;
    }
    FILE *output = fopen(output_file, "w");
    numArqs--;
    sprintf(nomeArq, "arq%d.txt", numArqs);
    arq1 = fopen(nomeArq, "r");
    fprintf(output, "%d\n", tamanhoString - 1);
    while(fgets(string1, tamanhoString, arq1)){
        if(strcmp(string1, "\n") != 0){
            fprintf(output, "%s\n", string1);
            printf("%s\n", string1);
        }
    }
    fclose(output);
    fclose(arq1);
    free(string1);
    free(string2);
    i = 0;
    while(i <= numArqs){
        sprintf(nomeArq, "arq%d.txt", i);
        i++;
        remove(nomeArq);
    }
}