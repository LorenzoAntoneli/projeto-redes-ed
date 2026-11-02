#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

void util_limpar_buffer_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres residuais */
    }
}

int util_ler_texto(const char *rotulo, char *destino, size_t tam_max) {
    if (!destino || tam_max < 2) return 0;

    if (rotulo) {
        printf("%s", rotulo);
        fflush(stdout);
    }

    /* Buffer temporário com folga para detectar se a linha excedeu o limite */
    char buffer_temp[256];
    if (!fgets(buffer_temp, sizeof(buffer_temp), stdin)) {
        return 0;
    }

    char *quebra = strchr(buffer_temp, '\n');
    if (!quebra) {
        /* A entrada foi maior do que o buffer de leitura. Limpa o restante da linha */
        util_limpar_buffer_stdin();
        printf(">> [ERRO] Texto muito longo! O limite maximo e de %u caracteres uteis.\n", (unsigned)(tam_max - 1));
        return 0;
    }

    /* Remove quebras de linha '\n' e '\r' */
    *quebra = '\0';
    size_t len = strlen(buffer_temp);
    if (len > 0 && buffer_temp[len - 1] == '\r') {
        buffer_temp[len - 1] = '\0';
        len--;
    }

    /* Verifica se ultrapassou o limite especificado */
    if (len > (tam_max - 1)) {
        printf(">> [ERRO] Texto excede o limite permitido de %u caracteres.\n", (unsigned)(tam_max - 1));
        return 0;
    }

    /* Valida se contém ao menos um caractere não branco */
    int contem_nao_branco = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isspace((unsigned char)buffer_temp[i])) {
            contem_nao_branco = 1;
            break;
        }
    }

    if (!contem_nao_branco) {
        printf(">> [ERRO] O campo nao pode ser vazio ou conter apenas espacos em branco.\n");
        return 0;
    }

    /* Copia de forma segura para o destino */
    strncpy(destino, buffer_temp, tam_max - 1);
    destino[tam_max - 1] = '\0';
    return 1;
}

int util_ler_inteiro(const char *rotulo, int *saida, int min_val, int max_val) {
    if (!saida) return 0;

    if (rotulo) {
        printf("%s", rotulo);
        fflush(stdout);
    }

    char linha[128];
    if (!fgets(linha, sizeof(linha), stdin)) {
        return 0;
    }

    char *quebra = strchr(linha, '\n');
    if (!quebra) {
        util_limpar_buffer_stdin();
        printf(">> [ERRO] Entrada excessivamente longa para valor numerico.\n");
        return 0;
    }
    *quebra = '\0';

    /* Ignora espaços iniciais */
    char *p = linha;
    while (isspace((unsigned char)*p)) p++;

    if (*p == '\0') {
        printf(">> [ERRO] Nenhum valor informado.\n");
        return 0;
    }

    char *endptr = NULL;
    errno = 0;
    long val = strtol(p, &endptr, 10);

    if (errno == ERANGE || val < INT_MIN || val > INT_MAX) {
        printf(">> [ERRO] Numero fora dos limites de representacao inteira (overflow/underflow).\n");
        return 0;
    }

    if (endptr == p) {
        printf(">> [ERRO] Entrada nao e um numero valido.\n");
        return 0;
    }

    /* Verifica se existem caracteres residuais que não sejam espaços */
    while (*endptr != '\0') {
        if (!isspace((unsigned char)*endptr)) {
            printf(">> [ERRO] Caracteres invalidos detectados apos o numero: '%s'.\n", endptr);
            return 0;
        }
        endptr++;
    }

    if (val < min_val || val > max_val) {
        printf(">> [ERRO] Valor %ld fora do intervalo permitido [%d a %d].\n", val, min_val, max_val);
        return 0;
    }

    *saida = (int)val;
    return 1;
}

int util_ler_id_positivo(const char *rotulo, int *id_saida) {
    int id;
    if (!util_ler_inteiro(rotulo, &id, 1, INT_MAX)) {
        return 0;
    }
    *id_saida = id;
    return 1;
}

void util_pausar(void) {
    printf("\nPressione [ENTER] para continuar...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* aguarda o enter */
    }
}
