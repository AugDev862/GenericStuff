#include <stdlib.h>
#include "include/tipos.h"


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