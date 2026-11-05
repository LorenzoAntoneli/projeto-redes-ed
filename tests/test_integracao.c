#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "registro.h"
#include "util.h"
#include "lista.h"
#include "fila.h"
#include "pilha.h"
#include "ordenacao.h"
#include "bst.h"

static void test_integracao_bst_ordenacao_lista(void) {
    printf("\n[1] Integracao: BST + Ordenacao (Copia Independente) + Lista de Filtragem\n");
    ArvoreBST arv;
    bst_inicializar(&arv);

    assert(bst_inserir(&arv, criar_equipamento(40, "Switch Core", "Switch L3", "Rack A - CPD")) == 1);
    assert(bst_inserir(&arv, criar_equipamento(15, "Firewall Borda", "Firewall", "Rack A - CPD")) == 1);
    assert(bst_inserir(&arv, criar_equipamento(75, "Access Point Recepcao", "AP Wi-Fi 6", "Bloco B")) == 1);
    assert(bst_inserir(&arv, criar_equipamento(25, "Switch Core", "Switch L3", "Rack B - Reserva")) == 1);

    /* Copia independente para vetor auxiliar e ordenacao por nome (desempate ID 25 < 40) */
    Equipamento *copia = NULL;
    size_t qtd = 0;
    assert(bst_copiar_para_vetor(&arv, &copia, &qtd) == 1);
    assert(qtd == 4 && copia != NULL);
    assert(ordenar_equipamentos_por_nome(copia, qtd) == 1);

    assert(copia[0].id == 75); /* Access Point Recepcao */
    assert(copia[1].id == 15); /* Firewall Borda */
    assert(copia[2].id == 25); /* Switch Core (ID 25) */
    assert(copia[3].id == 40); /* Switch Core (ID 40) */
    free(copia);

    /* Comprova que a BST permaneceu 100% intacta na raiz (ID 40) e na ordem de IDs */
    assert(arv.raiz != NULL && arv.raiz->dado.id == 40);
    Lista ids_bst;
    lista_inicializar(&ids_bst);
    assert(bst_coletar_ids_em_ordem(&arv, &ids_bst) == 1);
    int id0 = 0, id1 = 0, id2 = 0, id3 = 0;
    assert(lista_obter_em(&ids_bst, 0, &id0) && id0 == 15);
    assert(lista_obter_em(&ids_bst, 1, &id1) && id1 == 25);
    assert(lista_obter_em(&ids_bst, 2, &id2) && id2 == 40);
    assert(lista_obter_em(&ids_bst, 3, &id3) && id3 == 75);
    lista_liberar(&ids_bst);

    /* Filtragem case-insensitive guardando em Lista Encadeada (insercao no fim O(1)) */
    Lista filtrados;
    lista_inicializar(&filtrados);
    assert(bst_filtrar_por_termo(&arv, "rack a", &filtrados) == 1);
    assert(lista_tamanho(&filtrados) == 2);
    assert(lista_obter_em(&filtrados, 0, &id0) && id0 == 15);
    assert(lista_obter_em(&filtrados, 1, &id1) && id1 == 40);

    assert(bst_filtrar_por_termo(&arv, "inexistente-xyz", &filtrados) == 1);
    assert(lista_vazia(&filtrados) == 1);
    lista_liberar(&filtrados);

    bst_liberar(&arv);
    printf("  -> Ordenacao auxiliar manteve BST intacta e filtragem populou Lista O(1) -> OK\n");
}

static void test_integracao_consistencia_fila_pilha(void) {
    printf("\n[2] Integracao: Consistencia BST + Fila de Manutencao + Pilha de Historico\n");
    ArvoreBST arv;
    Fila fila_manut;
    Pilha historico;

    bst_inicializar(&arv);
    fila_inicializar(&fila_manut);
    pilha_inicializar(&historico);

    /* Cadastros validos registram na pilha */
    assert(bst_inserir(&arv, criar_equipamento(100, "Roteador WAN", "Roteador", "CPD")) == 1);
    pilha_registrar(&historico, ACAO_CADASTRAR_REGISTRO, 100);
    assert(bst_inserir(&arv, criar_equipamento(200, "Switch Distribuicao", "Switch", "Bloco 1")) == 1);
    pilha_registrar(&historico, ACAO_CADASTRAR_REGISTRO, 200);
    assert(bst_inserir(&arv, criar_equipamento(300, "Servidor Radius", "Servidor", "CPD")) == 1);
    pilha_registrar(&historico, ACAO_CADASTRAR_REGISTRO, 300);
    assert(pilha_tamanho(&historico) == 3);

    /* Tentativa de agendar ID inexistente (999) deve ser bloqueada pela consulta na BST */
    assert(bst_buscar(&arv, 999) == NULL);
    assert(pilha_tamanho(&historico) == 3); /* historico inalterado */

    /* Agendamento de 100, 200 e 300 na fila */
    assert(fila_enfileirar(&fila_manut, 100) == 1);
    pilha_registrar(&historico, ACAO_AGENDAR_MANUTENCAO, 100);
    assert(fila_enfileirar(&fila_manut, 200) == 1);
    pilha_registrar(&historico, ACAO_AGENDAR_MANUTENCAO, 200);
    assert(fila_enfileirar(&fila_manut, 300) == 1);
    pilha_registrar(&historico, ACAO_AGENDAR_MANUTENCAO, 300);
    assert(fila_tamanho(&fila_manut) == 3);
    assert(pilha_tamanho(&historico) == 6);

    /* Exclusao do equipamento 200 da BST deve remove-lo tambem da fila de manutencao */
    assert(bst_remover(&arv, 200, NULL) == 1);
    assert(fila_remover_valor(&fila_manut, 200) == 1);
    pilha_registrar(&historico, ACAO_REMOVER_REGISTRO, 200);

    assert(fila_contem(&fila_manut, 200) == 0);
    assert(fila_tamanho(&fila_manut) == 2);

    /* Atendimento na fila deve entregar 100 primeiro e depois 300 (200 ja foi expurgado) */
    int atendido = 0;
    assert(fila_desenfileirar(&fila_manut, &atendido) == 1 && atendido == 100);
    pilha_registrar(&historico, ACAO_ATENDER_MANUTENCAO, atendido);
    assert(fila_desenfileirar(&fila_manut, &atendido) == 1 && atendido == 300);
    pilha_registrar(&historico, ACAO_ATENDER_MANUTENCAO, atendido);
    assert(fila_vazia(&fila_manut) == 1);

    /* Verifica que as 5 acoes mais recentes estao corretas e a pilha guarda todas as 9 */
    assert(pilha_tamanho(&historico) == 9);
    AcaoHistorico top5[PILHA_LIMITE_EXIBICAO];
    assert(pilha_consultar_recentes(&historico, top5, PILHA_LIMITE_EXIBICAO) == 5);
    assert(top5[0].tipo == ACAO_ATENDER_MANUTENCAO && top5[0].id == 300);
    assert(top5[1].tipo == ACAO_ATENDER_MANUTENCAO && top5[1].id == 100);
    assert(top5[2].tipo == ACAO_REMOVER_REGISTRO   && top5[2].id == 200);

    bst_liberar(&arv);
    fila_liberar(&fila_manut);
    pilha_liberar(&historico);
    printf("  -> Consistencia atomica BST/Fila na remocao e rastreamento LIFO validados -> OK\n");
}

static void test_validacao_entradas_seguras(void) {
    printf("\n[3] Entradas Seguras: Limite de 79 chars, Limpeza de Buffer, Vazios e Overflow\n");

    const char *arquivo_tmp = "build/test_stdin_entradas.txt";
    FILE *fp = fopen(arquivo_tmp, "w");
    assert(fp != NULL);

    /* 1) Linha vazia */
    fprintf(fp, "\n");
    /* 2) Linha apenas com espacos */
    fprintf(fp, "      \n");
    /* 3) Linha com 300 caracteres 'X' (estoura buffer_temp de fgets e exige limpeza de stdin!) */
    for (int i = 0; i < 300; i++) fputc('X', fp);
    fputc('\n', fp);
    /* 4) Linha com 85 caracteres 'Y' (> 79 uteis) */
    for (int i = 0; i < 85; i++) fputc('Y', fp);
    fputc('\n', fp);
    /* 5) Linha valida com espacos logo apos os estouros */
    fprintf(fp, "Switch Core Sala 01\n");

    /* 6) Entradas numericas invalidas: letras, sufixo invalido, zero, negativo, overflow */
    fprintf(fp, "abc\n");
    fprintf(fp, "45xyz\n");
    fprintf(fp, "0\n");
    fprintf(fp, "-12\n");
    fprintf(fp, "9999999999999999999999999\n");
    /* 7) ID positivo valido */
    fprintf(fp, "  77  \n");
    fclose(fp);

    assert(freopen(arquivo_tmp, "r", stdin) != NULL);

    char texto[TAM_TEXTO_MAX];
    assert(util_ler_texto(NULL, texto, TAM_TEXTO_MAX) == 0); /* vazia */
    assert(util_ler_texto(NULL, texto, TAM_TEXTO_MAX) == 0); /* so espacos */
    assert(util_ler_texto(NULL, texto, TAM_TEXTO_MAX) == 0); /* 300 chars + limpeza stdin */
    assert(util_ler_texto(NULL, texto, TAM_TEXTO_MAX) == 0); /* 85 chars (> 79) */
    assert(util_ler_texto(NULL, texto, TAM_TEXTO_MAX) == 1); /* valida! */
    assert(strcmp(texto, "Switch Core Sala 01") == 0);

    int id_lido = 0;
    assert(util_ler_id_positivo(NULL, &id_lido) == 0); /* "abc" */
    assert(util_ler_id_positivo(NULL, &id_lido) == 0); /* "45xyz" */
    assert(util_ler_id_positivo(NULL, &id_lido) == 0); /* "0" */
    assert(util_ler_id_positivo(NULL, &id_lido) == 0); /* "-12" */
    assert(util_ler_id_positivo(NULL, &id_lido) == 0); /* overflow */
    assert(util_ler_id_positivo(NULL, &id_lido) == 1); /* "  77  " */
    assert(id_lido == 77);

    printf("\n  -> Todas as entradas invalidas foram rejeitadas com limpeza de buffer intacta -> OK\n");
}

int main(void) {
    printf("============================================================\n");
    printf(" BATERIA DE TESTES - INTEGRACAO, CONSISTENCIA E ENTRADAS\n");
    printf("============================================================\n");
    test_integracao_bst_ordenacao_lista();
    test_integracao_consistencia_fila_pilha();
    test_validacao_entradas_seguras();
    printf("\n============================================================\n");
    printf(" TODOS OS TESTES INTEGRADOS PASSARAM COM SUCESSO!\n");
    printf("============================================================\n");
    return 0;
}
