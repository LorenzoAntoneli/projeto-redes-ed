#include "registro.h"
#include <stdio.h>
#include <string.h>

Equipamento criar_equipamento(int id, const char *nome, const char *tipo, const char *localizacao) {
    Equipamento eq;
    eq.id = id;

    if (nome) {
        strncpy(eq.nome, nome, TAM_TEXTO_UTIL);
        eq.nome[TAM_TEXTO_UTIL] = '\0';
    } else {
        eq.nome[0] = '\0';
    }

    if (tipo) {
        strncpy(eq.tipo_equipamento, tipo, TAM_TEXTO_UTIL);
        eq.tipo_equipamento[TAM_TEXTO_UTIL] = '\0';
    } else {
        eq.tipo_equipamento[0] = '\0';
    }

    if (localizacao) {
        strncpy(eq.localizacao, localizacao, TAM_TEXTO_UTIL);
        eq.localizacao[TAM_TEXTO_UTIL] = '\0';
    } else {
        eq.localizacao[0] = '\0';
    }

    return eq;
}

void exibir_equipamento_detalhado(const Equipamento *eq) {
    if (!eq) return;
    printf("+----------------------------------------------------------------------+\n");
    printf("| ID:                %-49d |\n", eq->id);
    printf("| Nome:              %-49s |\n", eq->nome);
    printf("| Tipo Equipamento:  %-49s |\n", eq->tipo_equipamento);
    printf("| Localizacao:       %-49s |\n", eq->localizacao);
    printf("+----------------------------------------------------------------------+\n");
}

void exibir_cabecalho_equipamentos(void) {
    printf("+-------+--------------------------------+----------------------+----------------------+\n");
    printf("| %-5s | %-30s | %-20s | %-20s |\n", "ID", "NOME", "TIPO EQUIPAMENTO", "LOCALIZACAO");
    printf("+-------+--------------------------------+----------------------+----------------------+\n");
}

void exibir_equipamento_linha(const Equipamento *eq) {
    if (!eq) return;
    printf("| %-5d | %-30.30s | %-20.20s | %-20.20s |\n",
           eq->id, eq->nome, eq->tipo_equipamento, eq->localizacao);
}

int comparar_equipamentos_por_nome(const Equipamento *a, const Equipamento *b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;

    /* Comparação estrita com strcmp() sensível a maiúsculas/minúsculas */
    int cmp = strcmp(a->nome, b->nome);
    if (cmp != 0) {
        return cmp;
    }

    /* Critério de desempate: ID crescente quando os nomes forem textualmente idênticos */
    if (a->id < b->id) return -1;
    if (a->id > b->id) return 1;
    return 0;
}
