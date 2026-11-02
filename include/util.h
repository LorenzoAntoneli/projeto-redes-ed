#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

/**
 * @brief Limpa o buffer de entrada (stdin) descartando caracteres até '\n' ou EOF.
 */
void util_limpar_buffer_stdin(void);

/**
 * @brief Lê uma linha de texto do usuário de forma segura.
 * Regras do projeto:
 *  - Aceita espaços em branco.
 *  - Deve conter pelo menos um caractere não branco.
 *  - Rejeita se exceder o limite útil (tam_max - 1, ex.: 79 chars para char[80]).
 *  - Se exceder, limpa o buffer restante de stdin para evitar contaminação.
 * 
 * @param rotulo Texto de orientação exibido ao usuário (prompt).
 * @param destino Buffer de destino (ex.: char[80]).
 * @param tam_max Tamanho total do buffer destino (incluindo o terminador '\0').
 * @return 1 se a leitura for válida e aceita; 0 caso contrário.
 */
int util_ler_texto(const char *rotulo, char *destino, size_t tam_max);

/**
 * @brief Lê um número inteiro validado via strtol(), checando overflow, formato e intervalo.
 * 
 * @param rotulo Prompt exibido ao usuário.
 * @param saida Ponteiro para armazenar o valor lido.
 * @param min_val Valor mínimo permitido (inclusive).
 * @param max_val Valor máximo permitido (inclusive).
 * @return 1 se o valor for válido; 0 caso contrário.
 */
int util_ler_inteiro(const char *rotulo, int *saida, int min_val, int max_val);

/**
 * @brief Lê um identificador (ID) estritamente positivo (int > 0).
 * 
 * @param rotulo Prompt exibido ao usuário.
 * @param id_saida Ponteiro para armazenar o ID validado.
 * @return 1 se o ID for positivo e válido; 0 caso contrário.
 */
int util_ler_id_positivo(const char *rotulo, int *id_saida);

/**
 * @brief Pausa a tela aguardando o usuário pressionar ENTER.
 */
void util_pausar(void);

#endif /* UTIL_H */
