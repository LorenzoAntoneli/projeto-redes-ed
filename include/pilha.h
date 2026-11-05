#ifndef PILHA_H
#define PILHA_H

#include <stddef.h>

#define PILHA_LIMITE_EXIBICAO 5

/**
 * @brief Tipos de operações que alteram o estado do sistema (Seção 8 do regulamento).
 * Buscas, listagens, filtragens, ordenação e consultas NÃO entram no histórico.
 */
typedef enum {
    ACAO_CADASTRAR_REGISTRO  = 1,
    ACAO_REMOVER_REGISTRO    = 2,
    ACAO_AGENDAR_MANUTENCAO  = 3,
    ACAO_ATENDER_MANUTENCAO  = 4
} TipoAcao;

/**
 * @brief Dados de uma operação registrada no histórico.
 *  - tipo: categoria da alteração de estado concluída com sucesso.
 *  - id: identificador positivo do equipamento envolvido na operação.
 */
typedef struct {
    TipoAcao tipo;
    int id;
} AcaoHistorico;

/**
 * @brief Nó da pilha dinâmica encadeada.
 */
typedef struct NoPilha {
    AcaoHistorico acao;
    struct NoPilha *abaixo;
} NoPilha;

/**
 * @brief Descritor da Pilha Dinâmica de Histórico (Requisito R04 e Seção 8).
 * Limitada apenas pela memória disponível, seguindo política LIFO (Last-In, First-Out).
 */
typedef struct {
    NoPilha *topo;
    size_t tamanho;
} Pilha;

/**
 * @brief Inicializa a pilha dinâmica como vazia.
 */
void pilha_inicializar(Pilha *p);

/**
 * @brief Verifica se a pilha está vazia.
 * @return 1 se vazia (ou ponteiro nulo); 0 caso contrário.
 */
int pilha_vazia(const Pilha *p);

/**
 * @brief Retorna o total de ações armazenadas na pilha.
 */
size_t pilha_tamanho(const Pilha *p);

/**
 * @brief Monta uma estrutura AcaoHistorico com os parâmetros informados.
 */
AcaoHistorico criar_acao_historico(TipoAcao tipo, int id);

/**
 * @brief Retorna uma descrição textual legível para o tipo de ação.
 */
const char *pilha_descricao_tipo(TipoAcao tipo);

/**
 * @brief Empilha (push) uma ação de alteração de estado concluída com sucesso.
 * Valida se o tipo de ação pertence ao conjunto permitido e se o ID é positivo.
 * @return 1 em caso de sucesso; 0 se a ação for inválida ou falhar alocação.
 */
int pilha_push(Pilha *p, AcaoHistorico acao);

/**
 * @brief Atalho conveniente para criar e empilhar uma ação diretamente.
 * @return 1 em caso de sucesso; 0 caso contrário.
 */
int pilha_registrar(Pilha *p, TipoAcao tipo, int id);

/**
 * @brief Desempilha (pop) a ação do topo da pilha, removendo-a.
 * @param saida Ponteiro opcional onde a ação removida será copiada.
 * @return 1 se desempilhou com sucesso; 0 se a pilha estiver vazia.
 */
int pilha_pop(Pilha *p, AcaoHistorico *saida);

/**
 * @brief Consulta (peek) a ação no topo da pilha sem removê-la.
 * @param saida Ponteiro onde a ação do topo será copiada.
 * @return 1 se houver elemento no topo; 0 se a pilha estiver vazia.
 */
int pilha_peek(const Pilha *p, AcaoHistorico *saida);

/**
 * @brief Copia de forma não destrutiva até 'max_itens' ações a partir do topo
 * (da mais recente para a mais antiga), sem alterar a pilha.
 * @return Quantidade efetiva de ações copiadas (0 até max_itens).
 */
size_t pilha_consultar_recentes(const Pilha *p, AcaoHistorico *destino, size_t max_itens);

/**
 * @brief Exibe no terminal até as 5 últimas ações do histórico (da mais recente
 * para a mais antiga) de maneira estritamente não destrutiva.
 */
void pilha_exibir_ultimas_cinco(const Pilha *p);

/**
 * @brief Libera todos os nós alocados na pilha e restaura o estado inicial.
 */
void pilha_liberar(Pilha *p);

#endif /* PILHA_H */
