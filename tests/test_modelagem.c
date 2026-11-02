#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "registro.h"
#include "util.h"

static void test_criacao_equipamento(void) {
    printf("[TESTE] Criacao e limites de campos de Equipamento...\n");
    Equipamento eq = criar_equipamento(101, "Switch Core 01", "Switch L3", "Data Center Rack A");
    assert(eq.id == 101);
    assert(strcmp(eq.nome, "Switch Core 01") == 0);
    assert(strcmp(eq.tipo_equipamento, "Switch L3") == 0);
    assert(strcmp(eq.localizacao, "Data Center Rack A") == 0);

    /* Teste de truncamento seguro se exceder 79 chars */
    char texto_longo[120];
    memset(texto_longo, 'A', 110);
    texto_longo[110] = '\0';
    Equipamento eq_longo = criar_equipamento(102, texto_longo, "Roteador", "Sala B");
    assert(strlen(eq_longo.nome) == TAM_TEXTO_UTIL);
    assert(eq_longo.nome[TAM_TEXTO_UTIL] == '\0');
    printf("  -> OK: Campos inicializados e limitados a 79 chars com sucesso.\n");
}

static void test_comparacao_e_desempate(void) {
    printf("[TESTE] Comparacao de equipamentos e desempate por ID...\n");
    Equipamento e1 = criar_equipamento(10, "Access Point 01", "AP", "Corredor 1");
    Equipamento e2 = criar_equipamento(20, "Switch Borda", "Switch", "Rack 2");
    Equipamento e3 = criar_equipamento(15, "Access Point 01", "AP", "Corredor 2");

    /* "Access Point 01" vem antes de "Switch Borda" */
    assert(comparar_equipamentos_por_nome(&e1, &e2) < 0);
    assert(comparar_equipamentos_por_nome(&e2, &e1) > 0);

    /* Nomes idênticos ("Access Point 01"): desempate por ID crescente (10 < 15) */
    assert(comparar_equipamentos_por_nome(&e1, &e3) < 0);
    assert(comparar_equipamentos_por_nome(&e3, &e1) > 0);

    /* Comparação do mesmo elemento */
    assert(comparar_equipamentos_por_nome(&e1, &e1) == 0);

    /* Case-sensitive (strcmp padrão do C) */
    Equipamento e_min = criar_equipamento(30, "access point", "AP", "Lab");
    /* 'A' (ASCII 65) < 'a' (ASCII 97) */
    assert(comparar_equipamentos_por_nome(&e1, &e_min) < 0);

    printf("  -> OK: Ordenacao alfabetica e desempate por ID validados.\n");
}

int main(void) {
    printf("==================================================\n");
    printf(" BATERIA DE TESTES - ETAPA 1 (MODELAGEM E REGISTRO)\n");
    printf("==================================================\n");
    test_criacao_equipamento();
    test_comparacao_e_desempate();
    printf("==================================================\n");
    printf(" TODOS OS TESTES DA ETAPA 1 PASSARAM COM SUCESSO!\n");
    printf("==================================================\n");
    return 0;
}
