#ifndef TIPOS_H
#define TIPOS_H

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

extern const int vizinhaca_8[8][2];
extern const int vizinhaca_4[4][2];

#endif