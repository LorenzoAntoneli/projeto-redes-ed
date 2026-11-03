#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

void lista_inicializar(Lista *l) {
    if (!l) return;
    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
}

int lista_vazia(const Lista *l) {
    if (!l || l->inicio == NULL) {
        return 1;
    }
    return 0;
}

size_t lista_tamanho(const Lista *l) {
    if (!l) return 0;
    return l->tamanho;
}

int lista_inserir_fim(Lista *l, int id) {
    if (!l || id <= 0) {
        return 0;
    }

    NoLista *novo = (NoLista *)malloc(sizeof(NoLista));
    if (!novo) {
        return 0;
    }

    novo->id = id;
    novo->prox = NULL;

    if (l->inicio == NULL) {
        /* Lista estava vazia: início e fim apontam para o novo nó */
        l->inicio = novo;
        l->fim = novo;
    } else {
        /* Encadeia diretamente após o último nó em O(1) */
        l->fim->prox = novo;
        l->fim = novo;
    }

    l->tamanho++;
    return 1;
}

int lista_contem(const Lista *l, int id) {
    if (!l || id <= 0) return 0;

    const NoLista *atual = l->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

int lista_obter_em(const Lista *l, size_t indice, int *id_saida) {
    if (!l || !id_saida || indice >= l->tamanho) {
        return 0;
    }

    const NoLista *atual = l->inicio;
    size_t pos = 0;
    while (atual != NULL && pos < indice) {
        atual = atual->prox;
        pos++;
    }

    if (!atual) {
        return 0;
    }

    *id_saida = atual->id;
    return 1;
}

void lista_exibir(const Lista *l) {
    if (!l || lista_vazia(l)) {
        printf("[Lista vazia]\n");
        return;
    }

    const NoLista *atual = l->inicio;
    while (atual != NULL) {
        printf("%d", atual->id);
        if (atual->prox != NULL) {
            printf(" -> ");
        }
        atual = atual->prox;
    }
    printf("\n");
}

void lista_liberar(Lista *l) {
    if (!l) return;

    NoLista *atual = l->inicio;
    while (atual != NULL) {
        NoLista *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
}
