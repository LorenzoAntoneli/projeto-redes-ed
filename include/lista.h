#ifndef LISTA_H
#define LISTA_H

#include <stddef.h>

/**
 * @brief Nó da lista simplesmente encadeada.
 * Armazena o identificador (ID) de um equipamento coletado em consultas ou percursos.
 */
typedef struct NoLista {
    int id;
    struct NoLista *prox;
} NoLista;

/**
 * @brief Descritor da Lista Encadeada Simples (Requisito R03).
 * Mantém ponteiros para o início e para o fim, garantindo inserção no final em O(1)
 * para preservar a sequência exata de coleta/filtragem de equipamentos na árvore.
 */
typedef struct {
    NoLista *inicio;
    NoLista *fim;
    size_t tamanho;
} Lista;

/**
 * @brief Inicializa a lista encadeada como vazia.
 */
void lista_inicializar(Lista *l);

/**
 * @brief Verifica se a lista está vazia.
 * @return 1 se estiver vazia (ou ponteiro nulo); 0 caso contrário.
 */
int lista_vazia(const Lista *l);

/**
 * @brief Retorna a quantidade atual de elementos na lista.
 */
size_t lista_tamanho(const Lista *l);

/**
 * @brief Insere um novo ID no final da lista em complexidade O(1).
 * @return 1 em caso de sucesso; 0 em falha de alocação ou ponteiro inválido.
 */
int lista_inserir_fim(Lista *l, int id);

/**
 * @brief Verifica se um determinado ID está presente na lista.
 * @return 1 se encontrado; 0 caso contrário.
 */
int lista_contem(const Lista *l, int id);

/**
 * @brief Obtém o elemento armazenado em uma posição específica (0 até tamanho - 1).
 * Útil para inspeção e testes automatizados da ordem de descoberta.
 * @return 1 se o índice for válido; 0 caso contrário.
 */
int lista_obter_em(const Lista *l, size_t indice, int *id_saida);

/**
 * @brief Exibe os IDs armazenados na lista na sequência em que foram inseridos.
 */
void lista_exibir(const Lista *l);

/**
 * @brief Libera toda a memória alocada pelos nós da lista e reinicializa seus ponteiros.
 */
void lista_liberar(Lista *l);

#endif /* LISTA_H */
