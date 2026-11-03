#include "ordenacao.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Intercala duas metades já ordenadas [esq..meio] e [meio+1..dir]
 * utilizando o buffer auxiliar pré-alocado e o comparador oficial do TAD Equipamento
 * (strcmp sensível a maiúsculas/minúsculas com desempate por ID crescente).
 */
static void intercalar(Equipamento *vetor, Equipamento *aux, size_t esq, size_t meio, size_t dir) {
    size_t k;
    for (k = esq; k <= dir; k++) {
        aux[k] = vetor[k];
    }

    size_t i = esq;
    size_t j = meio + 1;

    for (k = esq; k <= dir; k++) {
        if (i > meio) {
            vetor[k] = aux[j++];
        } else if (j > dir) {
            vetor[k] = aux[i++];
        } else if (comparar_equipamentos_por_nome(&aux[i], &aux[j]) <= 0) {
            vetor[k] = aux[i++];
        } else {
            vetor[k] = aux[j++];
        }
    }
}

/**
 * @brief Divisão recursiva do Merge Sort sobre o intervalo fechado [esq..dir].
 */
static void mergesort_recursivo(Equipamento *vetor, Equipamento *aux, size_t esq, size_t dir) {
    if (esq >= dir) {
        return;
    }

    size_t meio = esq + (dir - esq) / 2;
    mergesort_recursivo(vetor, aux, esq, meio);
    mergesort_recursivo(vetor, aux, meio + 1, dir);
    intercalar(vetor, aux, esq, meio, dir);
}

int ordenar_equipamentos_por_nome(Equipamento *vetor, size_t n) {
    if (n == 0) {
        return 1;
    }
    if (!vetor) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    /* Aloca um único vetor temporário O(n) compartilhado por toda a recursão */
    Equipamento *aux = (Equipamento *)malloc(n * sizeof(Equipamento));
    if (!aux) {
        return 0;
    }

    mergesort_recursivo(vetor, aux, 0, n - 1);

    free(aux);
    return 1;
}

void exibir_equipamentos_ordenados(const Equipamento *vetor, size_t n) {
    if (!vetor || n == 0) {
        printf("\n>> Nenhum equipamento cadastrado para ordenar e exibir.\n");
        return;
    }

    printf("\nListagem de Equipamentos Ordenados por Nome (Merge Sort - %lu registro(s)):\n",
           (unsigned long)n);
    exibir_cabecalho_equipamentos();
    for (size_t i = 0; i < n; i++) {
        exibir_equipamento_linha(&vetor[i]);
    }
    printf("+-------+--------------------------------+----------------------+----------------------+\n");
}
