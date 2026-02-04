#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if( argc != 2){
        printf("Número de argumentos inválidos");
        return 1;
    }

    FILE *entrada;
    entrada = fopen(argv[1], "r");
    if(entrada == NULL){
        printf("Erro ao abrir o arquivo de entrada!\n");
        return 1;
    }

    int colunas, linhas;

    //ignora a primeira linha
    fscanf(entrada, "%*[^\\n]\\n");

    if(fscanf(entrada, "%d %d", &colunas, &linhas) == 2){
        prtinf("A imagem possui %d colunas e %d linhas", linhas, colunas);
    } else {
        printf("Erro ao ler as dimensões da imagem");
    }



    return 0;
}