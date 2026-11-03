#include "fila.h"
#include <stdlib.h>

void fila_inicializar(Fila *f) {
    if (!f) return;
    f->frente = NULL;
    f->tras = NULL;
    f->tamanho = 0;
}

int fila_vazia(const Fila *f) {
    if (!f || f->frente == NULL) {
        return 1;
    }
    return 0;
}

size_t fila_tamanho(const Fila *f) {
    if (!f) return 0;
    return f->tamanho;
}

int fila_enfileirar(Fila *f, int valor) {
    if (!f) return 0;

    NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
    if (!novo) {
        return 0;
    }

    novo->valor = valor;
    novo->prox = NULL;

    if (f->tras == NULL) {
        /* Fila estava vazia: frente e trás apontam para o único nó */
        f->frente = novo;
        f->tras = novo;
    } else {
        f->tras->prox = novo;
        f->tras = novo;
    }

    f->tamanho++;
    return 1;
}

int fila_desenfileirar(Fila *f, int *valor_saida) {
    if (!f || f->frente == NULL) {
        return 0;
    }

    NoFila *removido = f->frente;
    if (valor_saida) {
        *valor_saida = removido->valor;
    }

    f->frente = removido->prox;
    if (f->frente == NULL) {
        /* Se removeu o último elemento, zera também o ponteiro de fim */
        f->tras = NULL;
    }

    free(removido);
    f->tamanho--;
    return 1;
}

int fila_frente(const Fila *f, int *valor_saida) {
    if (!f || f->frente == NULL || !valor_saida) {
        return 0;
    }

    *valor_saida = f->frente->valor;
    return 1;
}

void fila_liberar(Fila *f) {
    if (!f) return;

    NoFila *atual = f->frente;
    while (atual != NULL) {
        NoFila *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    f->frente = NULL;
    f->tras = NULL;
    f->tamanho = 0;
}
