#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "registro.h"
#include "ordenacao.h"

static void verificar_vetor_ordenado(const Equipamento *v, size_t n) {
    for (size_t i = 1; i < n; i++) {
        assert(comparar_equipamentos_por_nome(&v[i - 1], &v[i]) <= 0);
    }
}

static void test_vetor_desordenado(void) {
    printf("\n[1] Cenario: Vetor Desordenado\n");
    Equipamento v[5];
    v[0] = criar_equipamento(40, "Switch Core", "Switch L3", "Rack A");
    v[1] = criar_equipamento(10, "Access Point Lab", "AP Wi-Fi 6", "Bloco B");
    v[2] = criar_equipamento(30, "Roteador Borda", "Roteador", "Sala Telecom");
    v[3] = criar_equipamento(20, "Firewall Perimetro", "Firewall", "Rack A");
    v[4] = criar_equipamento(50, "Patch Panel 01", "Passivo", "Rack C");

    assert(ordenar_equipamentos_por_nome(v, 5) == 1);
    verificar_vetor_ordenado(v, 5);

    assert(strcmp(v[0].nome, "Access Point Lab") == 0);
    assert(strcmp(v[1].nome, "Firewall Perimetro") == 0);
    assert(strcmp(v[2].nome, "Patch Panel 01") == 0);
    assert(strcmp(v[3].nome, "Roteador Borda") == 0);
    assert(strcmp(v[4].nome, "Switch Core") == 0);
    printf("  -> Entrada: [Switch, Access Point, Roteador, Firewall, Patch Panel]\n");
    printf("  -> Obtido:  [%s, %s, %s, %s, %s] -> OK\n",
           v[0].nome, v[1].nome, v[2].nome, v[3].nome, v[4].nome);
}

static void test_vetor_ordenado_e_invertido(void) {
    printf("\n[2] Cenarios: Vetor Ja Ordenado e Vetor Invertido\n");

    /* Ja ordenado */
    Equipamento ord[4];
    ord[0] = criar_equipamento(1, "Alpha", "Roteador", "R1");
    ord[1] = criar_equipamento(2, "Bravo", "Switch", "R2");
    ord[2] = criar_equipamento(3, "Charlie", "Firewall", "R3");
    ord[3] = criar_equipamento(4, "Delta", "Servidor", "R4");

    assert(ordenar_equipamentos_por_nome(ord, 4) == 1);
    verificar_vetor_ordenado(ord, 4);
    assert(ord[0].id == 1 && ord[1].id == 2 && ord[2].id == 3 && ord[3].id == 4);
    printf("  -> Ja Ordenado: [Alpha, Bravo, Charlie, Delta] mantido em O(n log n) -> OK\n");

    /* Estritamente invertido */
    Equipamento inv[4];
    inv[0] = criar_equipamento(4, "Zeta", "Servidor", "R4");
    inv[1] = criar_equipamento(3, "Omega", "Firewall", "R3");
    inv[2] = criar_equipamento(2, "Gamma", "Switch", "R2");
    inv[3] = criar_equipamento(1, "Beta", "Roteador", "R1");

    assert(ordenar_equipamentos_por_nome(inv, 4) == 1);
    verificar_vetor_ordenado(inv, 4);
    assert(strcmp(inv[0].nome, "Beta") == 0);
    assert(strcmp(inv[1].nome, "Gamma") == 0);
    assert(strcmp(inv[2].nome, "Omega") == 0);
    assert(strcmp(inv[3].nome, "Zeta") == 0);
    printf("  -> Invertido: [Zeta, Omega, Gamma, Beta] -> [%s, %s, %s, %s] -> OK\n",
           inv[0].nome, inv[1].nome, inv[2].nome, inv[3].nome);
}

static void test_empate_textual_e_case_sensitive(void) {
    printf("\n[3] Cenarios: Desempate por ID Crescente e Sensibilidade ASCII (strcmp)\n");

    Equipamento v[6];
    v[0] = criar_equipamento(90, "Switch Acesso", "Switch L2", "Bloco C");
    v[1] = criar_equipamento(15, "Switch Acesso", "Switch L2", "Bloco A");
    v[2] = criar_equipamento(42, "Switch Acesso", "Switch L2", "Bloco B");
    v[3] = criar_equipamento(11, "switch acesso", "Switch L2", "Lab 01");
    v[4] = criar_equipamento(7,  "Firewall Core", "Appliance", "CPD");
    v[5] = criar_equipamento(3,  "access point",  "AP", "Recepcao");

    assert(ordenar_equipamentos_por_nome(v, 6) == 1);
    verificar_vetor_ordenado(v, 6);

    /* Em ASCII: 'F' (70) < 'S' (83) < 'a' (97) < 's' (115) */
    assert(strcmp(v[0].nome, "Firewall Core") == 0 && v[0].id == 7);
    assert(strcmp(v[1].nome, "Switch Acesso") == 0 && v[1].id == 15);
    assert(strcmp(v[2].nome, "Switch Acesso") == 0 && v[2].id == 42);
    assert(strcmp(v[3].nome, "Switch Acesso") == 0 && v[3].id == 90);
    assert(strcmp(v[4].nome, "access point") == 0  && v[4].id == 3);
    assert(strcmp(v[5].nome, "switch acesso") == 0 && v[5].id == 11);

    printf("  -> Desempate 'Switch Acesso' (IDs 90, 15, 42) -> Obtido IDs: %d, %d, %d -> OK\n",
           v[1].id, v[2].id, v[3].id);
    printf("  -> Case-sensitive ASCII: '%s' < '%s' < '%s' < '%s' -> OK\n",
           v[0].nome, v[1].nome, v[4].nome, v[5].nome);
}

static void test_copia_independente_e_exibicao(void) {
    printf("\n[4] Cenario: Copia Independente (Estrutura Original Intacta) e Exibicao\n");

    /* Simula registros na ordem de IDs da BST (intacta) */
    Equipamento base_original[3];
    base_original[0] = criar_equipamento(100, "Servidor DNS", "Servidor", "Rack 01");
    base_original[1] = criar_equipamento(200, "Balanceador Carga", "Appliance", "Rack 02");
    base_original[2] = criar_equipamento(300, "Gateway VPN", "Roteador", "Rack 01");

    Equipamento *vetor_aux = (Equipamento *)malloc(3 * sizeof(Equipamento));
    assert(vetor_aux != NULL);
    memcpy(vetor_aux, base_original, 3 * sizeof(Equipamento));

    assert(ordenar_equipamentos_por_nome(vetor_aux, 3) == 1);
    exibir_equipamentos_ordenados(vetor_aux, 3);

    /* Verifica que o vetor auxiliar foi ordenado por nome */
    assert(vetor_aux[0].id == 200); /* Balanceador Carga */
    assert(vetor_aux[1].id == 300); /* Gateway VPN */
    assert(vetor_aux[2].id == 100); /* Servidor DNS */

    /* Verifica que a estrutura original permaneceu 100% inalterada */
    assert(base_original[0].id == 100 && strcmp(base_original[0].nome, "Servidor DNS") == 0);
    assert(base_original[1].id == 200 && strcmp(base_original[1].nome, "Balanceador Carga") == 0);
    assert(base_original[2].id == 300 && strcmp(base_original[2].nome, "Gateway VPN") == 0);

    free(vetor_aux);

    /* Casos de contorno */
    assert(ordenar_equipamentos_por_nome(NULL, 0) == 1);
    assert(ordenar_equipamentos_por_nome(NULL, 5) == 0);
    assert(ordenar_equipamentos_por_nome(base_original, 1) == 1);
    printf("  -> Estrutura original preservada intacta e memoria auxiliar liberada -> OK\n");
}

int main(void) {
    printf("============================================================\n");
    printf(" BATERIA DE TESTES - ETAPA 2B (ORDENACAO PROPRIA MERGE SORT)\n");
    printf("============================================================\n");
    test_vetor_desordenado();
    test_vetor_ordenado_e_invertido();
    test_empate_textual_e_case_sensitive();
    test_copia_independente_e_exibicao();
    printf("\n============================================================\n");
    printf(" TODOS OS TESTES DE ORDENACAO PASSARAM COM SUCESSO!\n");
    printf("============================================================\n");
    return 0;
}
