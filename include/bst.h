#ifndef BST_H
#define BST_H

#include <stddef.h>
#include "registro.h"
#include "lista.h"

/**
 * @brief Nó da Árvore Binária de Busca (BST).
 * Indexado primariamente pelo campo 'dado.id' (inteiro positivo único).
 */
typedef struct NoBST {
    Equipamento dado;
    struct NoBST *esq;
    struct NoBST *dir;
} NoBST;

/**
 * @brief Descritor da Árvore Binária de Busca (Requisito R07).
 * Mantém o ponteiro para a raiz e a contagem total de equipamentos cadastrados.
 */
typedef struct {
    NoBST *raiz;
    size_t tamanho;
} ArvoreBST;

/**
 * @brief Inicializa a árvore binária de busca como vazia.
 */
void bst_inicializar(ArvoreBST *arvore);

/**
 * @brief Verifica se a árvore está vazia.
 * @return 1 se vazia (ou ponteiro nulo); 0 caso contrário.
 */
int bst_vazia(const ArvoreBST *arvore);

/**
 * @brief Retorna a quantidade total de equipamentos armazenados na árvore.
 */
size_t bst_tamanho(const ArvoreBST *arvore);

/**
 * @brief Calcula a altura da árvore (em número de arestas no maior caminho raiz-folha).
 * @return -1 se a árvore estiver vazia, 0 se contiver apenas a raiz, ou >= 1.
 */
int bst_altura(const ArvoreBST *arvore);

/**
 * @brief Insere um novo equipamento na BST respeitando a ordenação por ID único.
 * Rejeita IDs não positivos (<= 0), nomes vazios ou IDs duplicados.
 * @return 1 em caso de inserção bem-sucedida; 0 se inválido, duplicado ou falha de memória.
 */
int bst_inserir(ArvoreBST *arvore, Equipamento eq);

/**
 * @brief Busca um equipamento na BST pelo seu ID em complexidade O(h).
 * @return Ponteiro constante para o Equipamento encontrado, ou NULL se ausente.
 */
const Equipamento *bst_buscar(const ArvoreBST *arvore, int id);

/**
 * @brief Remove um equipamento da BST pelo ID, tratando os 3 casos clássicos:
 *  - Nó folha (0 filhos);
 *  - Nó com 1 filho (esquerdo ou direito);
 *  - Nó com 2 filhos (substituição pelo sucessor em-ordem da subárvore direita).
 * @param removido_saida Ponteiro opcional para copiar os dados do registro removido.
 * @return 1 se o equipamento foi encontrado e removido; 0 se ID inexistente ou árvore vazia.
 */
int bst_remover(ArvoreBST *arvore, int id, Equipamento *removido_saida);

/**
 * @brief Obtém o equipamento com o menor ID cadastrado na árvore (extrema esquerda).
 * @return 1 se a árvore não estiver vazia; 0 caso contrário.
 */
int bst_obter_menor(const ArvoreBST *arvore, Equipamento *saida);

/**
 * @brief Obtém o equipamento com o maior ID cadastrado na árvore (extrema direita).
 * @return 1 se a árvore não estiver vazia; 0 caso contrário.
 */
int bst_obter_maior(const ArvoreBST *arvore, Equipamento *saida);

/**
 * @brief Exibe em formato tabular todos os equipamentos usando percurso em-ordem
 * (ordem estritamente crescente de ID).
 */
void bst_exibir_em_ordem(const ArvoreBST *arvore);

/**
 * @brief Percorre a BST em-ordem e armazena os IDs na Lista Encadeada com inserção no fim O(1).
 * @return 1 em caso de sucesso; 0 em falha de alocação ou parâmetros inválidos.
 */
int bst_coletar_ids_em_ordem(const ArvoreBST *arvore, Lista *destino);

/**
 * @brief Filtra equipamentos cujo nome, tipo ou localização contenham 'termo'
 * (busca textual insensível a maiúsculas/minúsculas), guardando os IDs encontrados
 * em ordem crescente em uma Lista Encadeada própria (inserção no fim O(1)).
 * @return 1 em caso de sucesso; 0 em parâmetros inválidos ou falha de memória.
 */
int bst_filtrar_por_termo(const ArvoreBST *arvore, const char *termo, Lista *ids_encontrados);

/**
 * @brief Copia todos os registros da BST para um vetor auxiliar recém-alocado de
 * cópias independentes por valor (sem expor ponteiros internos de nós da árvore).
 * O chamador é responsável por liberar (*vetor_saida) com free() após o uso.
 * @return 1 em caso de sucesso (mesmo se vazia, com *vetor_saida = NULL e *qtd_saida = 0);
 *         0 em caso de ponteiros inválidos ou falha de alocação.
 */
int bst_copiar_para_vetor(const ArvoreBST *arvore, Equipamento **vetor_saida, size_t *qtd_saida);

/**
 * @brief Libera todos os nós alocados na árvore (percurso pós-ordem) e zera seu descritor.
 */
void bst_liberar(ArvoreBST *arvore);

#endif /* BST_H */
