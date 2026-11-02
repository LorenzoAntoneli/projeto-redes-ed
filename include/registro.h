#ifndef REGISTRO_H
#define REGISTRO_H

#include <stddef.h>

#define TAM_TEXTO_MAX 80
#define TAM_TEXTO_UTIL (TAM_TEXTO_MAX - 1) /* 79 caracteres úteis + '\0' */

/**
 * @brief Estrutura que representa um Equipamento de Rede (Tema A).
 * Requisitos:
 *  - id: int positivo (> 0) e único
 *  - nome: char[80], textual, espaços permitidos, máx 79 chars úteis
 *  - tipo_equipamento: char[80], textual, espaços permitidos, máx 79 chars úteis
 *  - localizacao: char[80], textual, espaços permitidos, máx 79 chars úteis
 */
typedef struct {
    int id;
    char nome[TAM_TEXTO_MAX];
    char tipo_equipamento[TAM_TEXTO_MAX];
    char localizacao[TAM_TEXTO_MAX];
} Equipamento;

/**
 * @brief Cria e retorna uma instância inicializada de Equipamento.
 */
Equipamento criar_equipamento(int id, const char *nome, const char *tipo, const char *localizacao);

/**
 * @brief Exibe todos os dados do equipamento de forma detalhada em bloco.
 */
void exibir_equipamento_detalhado(const Equipamento *eq);

/**
 * @brief Exibe o equipamento formatado em uma única linha (linha de tabela).
 */
void exibir_equipamento_linha(const Equipamento *eq);

/**
 * @brief Imprime o cabeçalho formatado para listagens tabulares de equipamentos.
 */
void exibir_cabecalho_equipamentos(void);

/**
 * @brief Compara dois equipamentos pelo campo 'nome' via strcmp() sensível a maiúsculas/minúsculas.
 * Critério de desempate: ID crescente quando os nomes forem textualmente idênticos.
 * @return <0 se a < b, 0 se a == b, >0 se a > b
 */
int comparar_equipamentos_por_nome(const Equipamento *a, const Equipamento *b);

#endif /* REGISTRO_H */
