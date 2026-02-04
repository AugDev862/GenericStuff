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

    //lendo P1
    char ignorado[3];
    fscanf(entrada, "%2s", ignorado);

    if(fscanf(entrada, "%d %d", &colunas, &linhas) == 2){
        colunas += 2;
        linhas += 2;
        printf("A imagem possui %d colunas e %d linhas(com padding)\n", colunas, linhas);
    } else {
        printf("Erro ao ler as dimensões da imagem!\n");
        return 1;
    }

    int imagem[linhas][colunas];

    for(int i = 0; i < linhas; i ++){
        for(int j = 0; j < colunas; j++){
            if(i == 0 || j == 0 || i == linhas - 1 || j == colunas - 1){
                imagem[i][j] = 0;
            } else if(fscanf(entrada, "%d", &imagem[i][j]) != 1) {
               printf("Pixel não lido corretamente \n");
               return 1;
            }
        }
    }

    for(int q  = 0; q < linhas; q ++){
        for(int k = 0; k < colunas; k ++){
            printf("%d ", imagem[q][k]);
        }
        printf("\n");
    }

    fclose(entrada);
    return 0;
}