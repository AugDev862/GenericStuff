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
    int linhas;
    int colunas;
    Pixel **pixels;
} Imagem;

typedef struct {
    Coordenada *dados;
    int topo;
    int capacidade;
} Pilha;

typedef struct {
    Coordenada min;
    Coordenada max;
} InfoObjeto;

void pilha_init(Pilha *p, int capacidade_inicial){
    p->topo = 0;
    p->capacidade = capacidade_inicial;
    p->dados = malloc(capacidade_inicial * sizeof(Coordenada));
}

void pilha_push(Pilha *p, Coordenada coord){
    if(p->topo >= p->capacidade){
        p->capacidade *=2;
        p->dados = realloc(p->dados, p->capacidade * sizeof(Coordenada));
    }
    p->dados[p->topo].i = coord.i;
    p->dados[p->topo].j = coord.j;
    p->topo++;
}

int pilha_vazia(Pilha *p){
    return p->topo == 0;
}

Coordenada pilha_pop(Pilha *p){
    if(pilha_vazia(p)){
        Coordenada invalida = {-1, -1};
        return invalida;
    }

    p->topo--;
    return p->dados[p->topo];
}

void pilha_free(Pilha *p){
    free(p->dados);
    p->dados = NULL;
    p->capacidade = 0;
    p->topo = 0;
}

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

void marcar_viz4(Imagem *img, Coordenada coord){

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

    Imagem *imagem = criar_imagem(linhas, colunas);

    for(int i = 0; i < linhas; i ++){
        for(int j = 0; j < colunas; j++){
            if(i == 0 || j == 0 || i == linhas - 1 || j == colunas - 1){
                imagem->pixels[i][j].valor = 0;
                imagem->pixels[i][j].rotulo = 0;

            } else if(fscanf(entrada, "%d", &imagem->pixels[i][j].valor) == 1) {
               imagem->pixels[i][j].rotulo = 0;
            } else{
                printf("Pixel não lido corretamente");
                return 1;
            }
        }
    }

    fclose(entrada);

    for(int q  = 0; q < linhas; q ++){
        for(int k = 0; k < colunas; k ++){
            printf("%d ", imagem->pixels[q][k].valor);
        }
        printf("\n");
    }

    InfoObjeto objetos_info[1000];
    int num_objetos = 0;

    for(int a = 1; a < linhas - 1; a ++){
        for(int b = 1; b < colunas - 1; b ++){
            if(imagem->pixels[a][b].valor == 1 && imagem->pixels[a][b].rotulo == 0){
                num_objetos++;
                Coordenada coord;
                coord.i = a;
                coord.j = b;
                rotular_objeto(imagem, coord, num_objetos, &objetos_info[num_objetos - 1]);
            }
        }
    }

    int objetos_com_buraco = 0;

    for(int id = 1; id <= num_objetos; id++){
        int altura_obj, largura_obj;
        int **obj_isolado = extrair_objeto(imagem, id, objetos_info[id - 1], &altura_obj, &largura_obj);
    
        if(tem_buraco(obj_isolado, altura_obj, largura_obj)){
            objetos_com_buraco++;
        }

        for(int i = 0; i < altura_obj; i++) free(obj_isolado[i]);
        free(obj_isolado);
    
    }

    printf("Objeto(s): %d\n", num_objetos);
    printf("Objeto(s) com buraco: %d\n", objetos_com_buraco);

    return 0;
}