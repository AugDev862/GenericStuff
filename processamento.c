#include "include/tipos.h"
#include <stdlib.h>
#include "pilha.c"


const int vizinhaca_8[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
const int vizinhaca_4[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

void rotular_objeto(Imagem *img, Coordenada coord, int rotulo, InfoObjeto *info){

    Pilha pilha;
    pilha_init(&pilha, 128);

    pilha_push(&pilha, coord);
    img->pixels[coord.i][coord.j].rotulo = rotulo;

    info->min.i = coord.i;
    info->max.i = coord.i;
    info->min.j = coord.j;
    info->max.j = coord.j;

    while(!pilha_vazia(&pilha)){
        Coordenada atual = pilha_pop(&pilha);

        if(atual.i < info->min.i) info->min.i = atual.i;
        if(atual.i > info->max.i) info->max.i = atual.i;
        if(atual.j < info->min.j) info->min.j = atual.j;
        if(atual.j > info->max.j) info->max.j = atual.j;

        for(int k=0; k < 8; k++){
            Coordenada vizinho;
            vizinho.i = atual.i + vizinhaca_8[k][0];
            vizinho.j = atual.j + vizinhaca_8[k][1];
            
            if(vizinho.i < 0 || vizinho.i >= img->linhas || vizinho.j < 0 || vizinho.j >= img->colunas) continue;

            if(img->pixels[vizinho.i][vizinho.j].valor == 1 && img->pixels[vizinho.i][vizinho.j].rotulo == 0){
                img->pixels[vizinho.i][vizinho.j].rotulo = rotulo;
                pilha_push(&pilha, vizinho);
            }
        }
    }

    pilha_free(&pilha);

}

int** extrair_objeto(Imagem *img, int rotulo, InfoObjeto info, int *altura_out, int *largura_out){
    
    *altura_out = info.max.i - info.min.i + 3;
    *largura_out = info.max.j - info.min.j + 3;

    int **obj = malloc(*altura_out * sizeof(int *));
    for(int i = 0; i < *altura_out; i++){
        obj[i] = calloc(*largura_out, sizeof(int));
    }

    for(int i = info.min.i; i <= info.max.i; i++){
        for(int j = info.min.j; j <= info.max.j; j++){
            if(img->pixels[i][j].rotulo == rotulo){
                obj[i - info.min.i + 1][j - info.min.j + 1] = 1;
            }
        }
    }

    return obj;
}

static void marcar_viz4(Imagem *img, Coordenada coord){

    Pilha pilha;
    pilha_init(&pilha, 128);
    pilha_push(&pilha, coord);
    img->pixels[coord.i][coord.j].valor = -1;

    while(!pilha_vazia(&pilha)){
        Coordenada atual = pilha_pop(&pilha);

        for(int k = 0; k < 4; k++){
            Coordenada vizinho;
            vizinho.i = atual.i + vizinhaca_4[k][0];
            vizinho.j = atual.j + vizinhaca_4[k][1];

            if(vizinho.i < 0 || vizinho.i >= img->linhas || vizinho.j < 0 || vizinho.j >= img->colunas) continue;

            if(img->pixels[vizinho.i][vizinho.j].valor == 1){
                img->pixels[vizinho.i][vizinho.j].valor = -1;
                pilha_push(&pilha, vizinho);
            }

        }

    }

    pilha_free(&pilha);

}

Imagem* criar_imagem(int linhas, int colunas) {
    Imagem *img = malloc(sizeof(Imagem));
    img->linhas = linhas;
    img->colunas = colunas;

    img->pixels = malloc(linhas * sizeof(Pixel*));
    for(int i = 0; i < linhas; i++){
        img->pixels[i] = malloc(colunas * sizeof(Pixel));
    }

    return img;
}

void liberar_imagem(Imagem *img){
    for(int i = 0; i < img->linhas; i++){
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}


int tem_buraco(int **obj_isolado, int altura_obj, int largura_obj){

    Imagem *obj_invertido = criar_imagem(altura_obj, largura_obj);

   
    for(int i = 0; i < altura_obj; i++){
        for(int j = 0; j < largura_obj; j ++){
            obj_invertido->pixels[i][j].valor = (obj_isolado[i][j] == 1) ? 0 : 1;
        }
    }

    int num_componentes = 0;

    for(int i = 0; i < altura_obj; i++){
        for(int j = 0; j < largura_obj; j++){
            if(obj_invertido->pixels[i][j].valor == 1) {
                Coordenada coord;
                coord.i = i;
                coord.j = j;
                num_componentes++;
                marcar_viz4(obj_invertido, coord);
            }
        }
    }

    liberar_imagem(obj_invertido);

    return (num_componentes > 1);
}