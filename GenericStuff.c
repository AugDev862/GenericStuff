#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int valor;
    int rotulo;
} Pixel;

typedef struct {
    int i, j;
} Coordenada;

typedef struct {
    Coordenada dados[100000];
    int topo;
} Pilha;

void pilha_init(Pilha *p){
    p->topo = 0;
}

void pilha_push(Pilha *p, int i, int j){
    p->dados[p->topo].i = i;
    p->dados[p->topo].j = j;
    p->topo++;
}

Coordenada pilha_pop(Pilha *p){
    p->topo--;
    return p->dados[p->topo];
}

int pilha_vazia(Pilha *p){
    return p->topo == 0;
}

void rotular_objeto(int linhas, int colunas, Pixel imagem[linhas][colunas], int i, int j, int rotulo){
    int vizinhaca_8[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    Pilha pilha;
    pilha_init(&pilha);

    pilha_push(&pilha, i, j);
    imagem[i][j].rotulo = rotulo;

    while(!pilha_vazia(&pilha)){
        Coordenada atual = pilha_pop(&pilha);

        for(int k=0; k < 8; k++){
            int vi = atual.i + vizinhaca_8[k][0];
            int vj = atual.j + vizinhaca_8[k][1];

            if(vi < 0 || vi >= linhas || vj < 0 || vj >= colunas) continue;

            if(imagem[vi][vj].valor == 1 && imagem[vi][vj].rotulo == 0){
                imagem[vi][vj].rotulo = rotulo;
                pilha_push(&pilha, vi, vj);
            }
        }
    }
}

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

    //Lendo dimensões da imagem
    if(fscanf(entrada, "%d %d", &colunas, &linhas) == 2){
        colunas += 2;
        linhas += 2;
        printf("A imagem possui %d colunas e %d linhas(com padding)\n", colunas, linhas);
    } else {
        printf("Erro ao ler as dimensões da imagem!\n");
        return 1;
    }

    //Cria o array da imagem no c com flag de visitado e padding
    Pixel imagem[linhas][colunas];

    for(int i = 0; i < linhas; i ++){
        for(int j = 0; j < colunas; j++){
            if(i == 0 || j == 0 || i == linhas - 1 || j == colunas - 1){
                imagem[i][j].valor = 0;
                imagem[i][j].rotulo = 0;

            } else if(fscanf(entrada, "%d", &imagem[i][j].valor) == 1) {
               imagem[i][j].rotulo = 0;
            } else{
                printf("Pixel não lido corretamente");
                return 1;
            }
        }
    }

    fclose(entrada);

    for(int q  = 0; q < linhas; q ++){
        for(int k = 0; k < colunas; k ++){
            printf("%d ", imagem[q][k].valor);
        }
        printf("\n");
    }

    int num_objetos = 0;
   
    int va, vb;

    for(int a = 1; a < linhas - 1; a ++){
        for(int b = 1; b < colunas - 1; b ++){
            if(imagem[a][b].valor == 1 && imagem[a][b].rotulo == 0){
                num_objetos++;
                rotular_objeto(linhas, colunas, imagem, a, b, num_objetos);
            }
        }
    }

    printf("Total de objetos: %d\n", num_objetos);

    return 0;
}