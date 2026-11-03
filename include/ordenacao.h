#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <stddef.h>
#include "registro.h"

/**
 * @brief Ordena um vetor auxiliar de cópias independentes de Equipamento em ordem
 * crescente pelo campo 'nome' via strcmp() (sensível a maiúsculas/minúsculas),
 * desempatando por ID crescente quando os nomes forem idênticos (Requisito R06 / Seção 7).
 *
 * Algoritmo implementado: Merge Sort próprio — complexidade de tempo O(n log n)
 * no melhor, médio e pior casos, e espaço auxiliar O(n). Não utiliza qsort().
 *
 * @param vetor Vetor de registros Equipamento a ser ordenado.
 * @param n Quantidade de elementos no vetor.
 * @return 1 em caso de sucesso; 0 em falha de alocação de memória ou ponteiro inválido.
 */
int ordenar_equipamentos_por_nome(Equipamento *vetor, size_t n);

/**
 * @brief Exibe em formato tabular o vetor auxiliar de equipamentos ordenados.
 * Caso n == 0, informa que não há equipamentos para exibir.
 */
void exibir_equipamentos_ordenados(const Equipamento *vetor, size_t n);

#endif /* ORDENACAO_H */
