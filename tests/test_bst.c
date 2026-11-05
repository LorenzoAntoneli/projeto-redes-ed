#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bst.h"
#include "lista.h"

static void validar_sequencia_em_ordem(const ArvoreBST *arvore, const int *esperado, size_t qtd_esperada) {
    Lista ids;
    lista_inicializar(&ids);
    assert(bst_coletar_ids_em_ordem(arvore, &ids) == 1);
    assert(lista_tamanho(&ids) == qtd_esperada);

    for (size_t i = 0; i < qtd_esperada; i++) {
        int obtido = 0;
        assert(lista_obter_em(&ids, i, &obtido) == 1);
        assert(obtido == esperado[i]);
    }
    lista_liberar(&ids);
}

static void test_arvore_vazia_e_invalidos(void) {
    printf("\n[1] Cenario: Arvore Vazia, IDs Invalidos e Duplicados\n");
    ArvoreBST arv;
    bst_inicializar(&arv);

    Equipamento aux;
    assert(bst_vazia(&arv) == 1);
    assert(bst_tamanho(&arv) == 0);
    assert(bst_altura(&arv) == -1);
    assert(bst_buscar(&arv, 10) == NULL);
    assert(bst_remover(&arv, 10, &aux) == 0);
    assert(bst_obter_menor(&arv, &aux) == 0);
    assert(bst_obter_maior(&arv, &aux) == 0);

    /* Rejeicao de IDs <= 0 e nome vazio */
    assert(bst_inserir(&arv, criar_equipamento(0, "Switch Zero", "Switch", "Rack 1")) == 0);
    assert(bst_inserir(&arv, criar_equipamento(-7, "Switch Negativo", "Switch", "Rack 1")) == 0);
    assert(bst_inserir(&arv, criar_equipamento(15, "", "Switch", "Rack 1")) == 0);
    assert(bst_vazia(&arv) == 1);

    /* Insercao valida seguida de tentativa de duplicata */
    assert(bst_inserir(&arv, criar_equipamento(50, "Roteador Core", "Roteador", "CPD")) == 1);
    assert(bst_altura(&arv) == 0);
    assert(bst_inserir(&arv, criar_equipamento(50, "Clone Indevido", "Outro", "Outro")) == 0);
    assert(bst_tamanho(&arv) == 1);

    const Equipamento *busca = bst_buscar(&arv, 50);
    assert(busca != NULL && strcmp(busca->nome, "Roteador Core") == 0);
    printf("  -> Operacoes em arvore vazia, IDs <= 0 e ID duplicado (50) rejeitados sem corrupcao -> OK\n");

    bst_liberar(&arv);
}

static void test_insercao_busca_percurso_extremos(void) {
    printf("\n[2] Cenario: Insercao, Busca, Em-Ordem e Menor/Maior ID\n");
    ArvoreBST arv;
    bst_inicializar(&arv);

    /*
     * Estrutura da BST construida:
     *              50
     *            /    \
     *          30      70
     *         /  \    /  \
     *        20  40  60  80
     *           /      \
     *          35      65
     */
    int entradas[] = {50, 30, 70, 20, 40, 60, 80, 35, 65};
    size_t n = sizeof(entradas) / sizeof(entradas[0]);
    for (size_t i = 0; i < n; i++) {
        char nome[40];
        sprintf(nome, "Equipamento-%02d", entradas[i]);
        assert(bst_inserir(&arv, criar_equipamento(entradas[i], nome, "Switch L3", "Sala Servidores")) == 1);
    }

    assert(bst_tamanho(&arv) == 9);
    assert(bst_altura(&arv) == 3);

    /* Buscas existentes e ausentes */
    assert(bst_buscar(&arv, 50) != NULL && bst_buscar(&arv, 50)->id == 50);
    assert(bst_buscar(&arv, 35) != NULL && bst_buscar(&arv, 35)->id == 35);
    assert(bst_buscar(&arv, 65) != NULL && bst_buscar(&arv, 65)->id == 65);
    assert(bst_buscar(&arv, 999) == NULL);
    assert(bst_buscar(&arv, 25) == NULL);

    /* Menor e Maior ID */
    Equipamento menor, maior;
    assert(bst_obter_menor(&arv, &menor) == 1 && menor.id == 20);
    assert(bst_obter_maior(&arv, &maior) == 1 && maior.id == 80);
    printf("  -> Menor ID esperado: 20 (obtido: %d) | Maior ID esperado: 80 (obtido: %d) -> OK\n",
           menor.id, maior.id);

    /* Percurso em-ordem */
    int esperado[] = {20, 30, 35, 40, 50, 60, 65, 70, 80};
    validar_sequencia_em_ordem(&arv, esperado, 9);
    bst_exibir_em_ordem(&arv);
    printf("  -> Percurso em-ordem produziu sequencia estritamente crescente de 9 IDs -> OK\n");

    bst_liberar(&arv);
}

static void test_remocoes_0_1_2_filhos(void) {
    printf("\n[3] Cenario: Remocao de ID Inexistente, Folha (0), 1 Filho e 2 Filhos\n");
    ArvoreBST arv;
    bst_inicializar(&arv);

    int entradas[] = {50, 30, 70, 20, 40, 60, 80, 35, 65};
    for (size_t i = 0; i < 9; i++) {
        char nome[40];
        sprintf(nome, "No-%02d", entradas[i]);
        assert(bst_inserir(&arv, nome[0] ? criar_equipamento(entradas[i], nome, "Roteador", "Rack B") : criar_equipamento(1, "X", "X", "X")) == 1);
    }

    Equipamento rem;
    /* 3.1 Remocao de ID inexistente */
    assert(bst_remover(&arv, 999, &rem) == 0);
    assert(bst_tamanho(&arv) == 9);
    printf("  -> Remocao de ID inexistente (999) rejeitada sem alterar tam=9 -> OK\n");

    /* 3.2 Caso 1: Remocao de no folha (ID 20, zero filhos) */
    assert(bst_remover(&arv, 20, &rem) == 1);
    assert(rem.id == 20);
    assert(bst_buscar(&arv, 20) == NULL);
    int esp_pos_folha[] = {30, 35, 40, 50, 60, 65, 70, 80};
    validar_sequencia_em_ordem(&arv, esp_pos_folha, 8);
    printf("  -> Caso Folha (0 filhos): removido ID 20, novo menor ID = 30 -> OK\n");

    /* 3.3 Caso 2a: Remocao de no com apenas 1 filho a direita (ID 30 tem apenas filho direito 40) */
    assert(bst_remover(&arv, 30, &rem) == 1 && rem.id == 30);
    assert(bst_buscar(&arv, 30) == NULL);
    int esp_pos_1dir[] = {35, 40, 50, 60, 65, 70, 80};
    validar_sequencia_em_ordem(&arv, esp_pos_1dir, 7);

    /* 3.4 Caso 2b: Remocao de no com apenas 1 filho a esquerda (ID 40 tem apenas filho esquerdo 35) */
    assert(bst_remover(&arv, 40, &rem) == 1 && rem.id == 40);
    assert(bst_buscar(&arv, 40) == NULL);
    int esp_pos_1esq[] = {35, 50, 60, 65, 70, 80};
    validar_sequencia_em_ordem(&arv, esp_pos_1esq, 6);
    printf("  -> Caso 1 Filho (direito ID 30 e esquerdo ID 40): subarvores reconectadas -> OK\n");

    /* 3.5 Caso 3: Remocao de no interno com 2 filhos (ID 70 possui filhos 60 e 80) */
    assert(bst_remover(&arv, 70, &rem) == 1 && rem.id == 70);
    assert(bst_buscar(&arv, 70) == NULL);
    int esp_pos_2int[] = {35, 50, 60, 65, 80};
    validar_sequencia_em_ordem(&arv, esp_pos_2int, 5);

    /* 3.6 Caso 3 na Raiz: Remocao da raiz (ID 50 possui filhos 35 e 80/60, sucessor em-ordem = 60) */
    assert(bst_remover(&arv, 50, &rem) == 1 && rem.id == 50 && strcmp(rem.nome, "No-50") == 0);
    assert(bst_buscar(&arv, 50) == NULL);
    assert(arv.raiz != NULL && arv.raiz->dado.id == 60);
    int esp_pos_raiz[] = {35, 60, 65, 80};
    validar_sequencia_em_ordem(&arv, esp_pos_raiz, 4);
    printf("  -> Caso 2 Filhos (interno ID 70 e raiz ID 50 -> nova raiz sucessor 60) -> OK\n");

    bst_liberar(&arv);
    assert(bst_vazia(&arv) == 1 && bst_tamanho(&arv) == 0);
}

static void test_arvore_degenerada(void) {
    printf("\n[4] Cenario: Degeneracao da BST (Insercao Estritamente Crescente)\n");
    ArvoreBST deg;
    bst_inicializar(&deg);

    for (int id = 10; id <= 50; id += 10) {
        assert(bst_inserir(&deg, criar_equipamento(id, "Switch Linha", "Switch", "Bloco A")) == 1);
    }

    /* Com 5 elementos em ordem crescente, a BST degenera em lista (altura = n - 1 = 4) */
    assert(bst_tamanho(&deg) == 5);
    assert(bst_altura(&deg) == 4);

    Equipamento menor, maior;
    assert(bst_obter_menor(&deg, &menor) == 1 && menor.id == 10);
    assert(bst_obter_maior(&deg, &maior) == 1 && maior.id == 50);

    /* Remocao da raiz com apenas 1 filho direito (ID 10) */
    assert(bst_remover(&deg, 10, NULL) == 1);
    assert(bst_altura(&deg) == 3);
    assert(bst_obter_menor(&deg, &menor) == 1 && menor.id == 20);
    printf("  -> Altura degenerada validada (h=4 para n=5) e remocao de raiz com 1 filho -> OK\n");

    bst_liberar(&deg);
}

int main(void) {
    printf("============================================================\n");
    printf(" BATERIA DE TESTES - ARVORE BINARIA DE BUSCA (BST)\n");
    printf("============================================================\n");
    test_arvore_vazia_e_invalidos();
    test_insercao_busca_percurso_extremos();
    test_remocoes_0_1_2_filhos();
    test_arvore_degenerada();
    printf("\n============================================================\n");
    printf(" TODOS OS TESTES DA ARVORE BINARIA DE BUSCA PASSARAM!\n");
    printf("============================================================\n");
    return 0;
}
