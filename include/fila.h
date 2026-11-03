#ifndef FILA_H
#define FILA_H

#include <stddef.h>

/**
 * @brief Nó da fila dinâmica encadeada.
 */
typedef struct NoFila {
    int valor;
    struct NoFila *prox;
} NoFila;

/**
 * @brief Descritor da Fila Dinâmica Encadeada (Requisito R05).
 * Segue a política FIFO (First-In, First-Out) com operações O(1),
 * utilizada para controlar o processamento de vértices na travessia BFS.
 */
typedef struct {
    NoFila *frente;
    NoFila *tras;
    size_t tamanho;
} Fila;

/**
 * @brief Inicializa a fila como vazia.
 */
void fila_inicializar(Fila *f);

/**
 * @brief Verifica se a fila está vazia.
 * @return 1 se estiver vazia (ou ponteiro nulo); 0 caso contrário.
 */
int fila_vazia(const Fila *f);

/**
 * @brief Retorna a quantidade de elementos aguardando na fila.
 */
size_t fila_tamanho(const Fila *f);

/**
 * @brief Insere um elemento no final da fila (enqueue) em O(1).
 * @return 1 em caso de sucesso; 0 em caso de falha de alocação ou ponteiro nulo.
 */
int fila_enfileirar(Fila *f, int valor);

/**
 * @brief Remove e retorna o elemento da frente da fila (dequeue) em O(1).
 * @param valor_saida Ponteiro opcional para receber o valor removido.
 * @return 1 se um elemento foi removido; 0 se a fila estiver vazia ou ponteiro nulo.
 */
int fila_desenfileirar(Fila *f, int *valor_saida);

/**
 * @brief Consulta o elemento na frente da fila sem removê-lo.
 * @return 1 se a fila não estiver vazia; 0 caso contrário.
 */
int fila_frente(const Fila *f, int *valor_saida);

/**
 * @brief Desaloca todos os nós remanescentes na fila e restaura o estado vazio.
 */
void fila_liberar(Fila *f);

#endif /* FILA_H */
