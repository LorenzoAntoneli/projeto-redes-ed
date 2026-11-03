#include <stdio.h>
#include <assert.h>
#include "lista.h"
#include "fila.h"
#include "pilha.h"

static void test_lista_encadeada(void) {
    printf("\n[1] TESTES DO TAD LISTA ENCADEADA (Ordem de Descoberta BFS)\n");
    printf("-----------------------------------------------------------\n");

    Lista l;
    lista_inicializar(&l);
    assert(lista_vazia(&l) == 1);
    assert(lista_tamanho(&l) == 0);

    /* Cenario 1.1: Origem isolada (1 unico elemento) */
    assert(lista_inserir_fim(&l, 500) == 1);
    int obtido = 0;
    assert(lista_obter_em(&l, 0, &obtido) == 1 && obtido == 500);
    assert(lista_tamanho(&l) == 1);
    printf("  - Caso Origem Isolada | Entrada: [500] | Esperado: tam=1, pos[0]=500 | Obtido: tam=%lu, pos[0]=%d -> OK\n",
           (unsigned long)lista_tamanho(&l), obtido);
    lista_liberar(&l);
    assert(lista_vazia(&l) == 1);

    /* Cenario 1.2: Insercao no fim O(1) preservando ordem exata de descoberta */
    int entrada_bfs[] = {40, 10, 25, 99, 5};
    size_t qtd = sizeof(entrada_bfs) / sizeof(entrada_bfs[0]);
    for (size_t i = 0; i < qtd; i++) {
        assert(lista_inserir_fim(&l, entrada_bfs[i]) == 1);
    }
    assert(lista_tamanho(&l) == qtd);

    for (size_t i = 0; i < qtd; i++) {
        int val = 0;
        assert(lista_obter_em(&l, i, &val) == 1);
        assert(val == entrada_bfs[i]);
    }
    assert(lista_contem(&l, 25) == 1);
    assert(lista_contem(&l, 777) == 0);

    /* Cenario 1.3: Rejeicao de IDs invalidos (<= 0) e indices fora do limite */
    assert(lista_inserir_fim(&l, 0) == 0);
    assert(lista_inserir_fim(&l, -15) == 0);
    assert(lista_obter_em(&l, 99, &obtido) == 0);

    printf("  - Insercao no Fim O(1) | Entrada: 40,10,25,99,5 | Esperado: 40 -> 10 -> 25 -> 99 -> 5\n");
    printf("    Obtido: ");
    lista_exibir(&l);

    lista_liberar(&l);
    assert(lista_vazia(&l) == 1);
    assert(lista_tamanho(&l) == 0);
    printf("  - Liberacao de memoria da lista concluida com sucesso -> OK\n");
}

static void test_fila_dinamica(void) {
    printf("\n[2] TESTES DO TAD FILA DINAMICA (Controle FIFO da BFS)\n");
    printf("-----------------------------------------------------------\n");

    Fila f;
    fila_inicializar(&f);
    assert(fila_vazia(&f) == 1);
    assert(fila_tamanho(&f) == 0);

    int saida = -1;
    /* Operacoes em fila vazia devem retornar 0 com seguranca */
    assert(fila_desenfileirar(&f, &saida) == 0);
    assert(fila_frente(&f, &saida) == 0);
    printf("  - Fila Vazia | Operacao: dequeue/frente | Esperado: retorno 0 | Obtido: 0 -> OK\n");

    /* Enfileirar 3 elementos e validar ordem estrita FIFO */
    assert(fila_enfileirar(&f, 101) == 1);
    assert(fila_enfileirar(&f, 202) == 1);
    assert(fila_enfileirar(&f, 303) == 1);
    assert(fila_tamanho(&f) == 3);

    /* Consulta da frente nao remove elemento */
    assert(fila_frente(&f, &saida) == 1 && saida == 101);
    assert(fila_tamanho(&f) == 3);

    int s1 = 0, s2 = 0, s3 = 0;
    assert(fila_desenfileirar(&f, &s1) == 1);
    assert(fila_desenfileirar(&f, &s2) == 1);
    assert(fila_desenfileirar(&f, &s3) == 1);
    assert(s1 == 101 && s2 == 202 && s3 == 303);
    assert(fila_vazia(&f) == 1);
    printf("  - Ordem FIFO | Entrada: 101, 202, 303 | Esperado: 101, 202, 303 | Obtido: %d, %d, %d -> OK\n",
           s1, s2, s3);

    /* Reinsercao apos esvaziamento completo (valida ajuste do ponteiro 'tras') */
    assert(fila_enfileirar(&f, 0) == 1);
    assert(fila_enfileirar(&f, 404) == 1);
    assert(fila_desenfileirar(&f, &saida) == 1 && saida == 0);
    fila_liberar(&f);
    assert(fila_vazia(&f) == 1);
    printf("  - Reinsercao pos-esvaziamento e liberacao de nos restantes -> OK\n");
}

static void test_pilha_historico(void) {
    printf("\n[3] TESTES DO TAD PILHA DINAMICA (Historico de Alteracoes)\n");
    printf("-----------------------------------------------------------\n");

    Pilha p;
    pilha_inicializar(&p);
    AcaoHistorico topo;
    AcaoHistorico recentes[PILHA_LIMITE_EXIBICAO];

    /* Cenario 3.1: Pilha vazia */
    assert(pilha_vazia(&p) == 1);
    assert(pilha_pop(&p, &topo) == 0);
    assert(pilha_peek(&p, &topo) == 0);
    assert(pilha_consultar_recentes(&p, recentes, PILHA_LIMITE_EXIBICAO) == 0);
    printf("  - Pilha Vazia | Operacao: pop, peek e consulta | Esperado: 0 itens | Obtido: 0 itens -> OK\n");
    pilha_exibir_ultimas_cinco(&p);

    /* Cenario 3.2: Rejeicao de acoes invalidas (ex.: IDs <= 0, laco A==A ou tipo fora de alteracao) */
    assert(pilha_registrar(&p, ACAO_CADASTRAR_REGISTRO, 0, 0) == 0);
    assert(pilha_registrar(&p, ACAO_CADASTRAR_REGISTRO, -5, 0) == 0);
    assert(pilha_registrar(&p, ACAO_ADICIONAR_CONEXAO, 10, 10) == 0);
    assert(pilha_registrar(&p, (TipoAcao)99, 10, 0) == 0);
    assert(pilha_vazia(&p) == 1);
    printf("  - Filtro de Estado | Entrada: IDs invalidos/laco/tipo=99 | Esperado: rejeitado | Obtido: tam=0 -> OK\n");

    /* Cenario 3.3: Menos de cinco acoes (3 acoes) + peek + consulta nao destrutiva */
    assert(pilha_registrar(&p, ACAO_CADASTRAR_REGISTRO, 10, 0) == 1);
    assert(pilha_registrar(&p, ACAO_CADASTRAR_REGISTRO, 20, 0) == 1);
    assert(pilha_registrar(&p, ACAO_ADICIONAR_CONEXAO, 10, 20) == 1);
    assert(pilha_tamanho(&p) == 3);

    assert(pilha_peek(&p, &topo) == 1);
    assert(topo.tipo == ACAO_ADICIONAR_CONEXAO && topo.id1 == 10 && topo.id2 == 20);
    assert(pilha_tamanho(&p) == 3); /* peek nao remove */

    size_t copiados = pilha_consultar_recentes(&p, recentes, PILHA_LIMITE_EXIBICAO);
    assert(copiados == 3);
    assert(recentes[0].tipo == ACAO_ADICIONAR_CONEXAO && recentes[0].id1 == 10 && recentes[0].id2 == 20);
    assert(recentes[1].tipo == ACAO_CADASTRAR_REGISTRO && recentes[1].id1 == 20);
    assert(recentes[2].tipo == ACAO_CADASTRAR_REGISTRO && recentes[2].id1 == 10);
    assert(pilha_tamanho(&p) == 3); /* consulta nao destrutiva */
    printf("  - Menos de 5 Acoes (3) | Esperado: 3 exibidas (LIFO), tam=3 intacto | Obtido: copiados=%lu, tam=%lu -> OK\n",
           (unsigned long)copiados, (unsigned long)pilha_tamanho(&p));

    /* Cenario 3.4: Mais de cinco acoes (7 acoes no total) */
    assert(pilha_registrar(&p, ACAO_CADASTRAR_REGISTRO, 30, 0) == 1);
    assert(pilha_registrar(&p, ACAO_ADICIONAR_CONEXAO, 20, 30) == 1);
    assert(pilha_registrar(&p, ACAO_REMOVER_CONEXAO, 10, 20) == 1);
    assert(pilha_registrar(&p, ACAO_REMOVER_REGISTRO, 10, 0) == 1);
    assert(pilha_tamanho(&p) == 7);

    copiados = pilha_consultar_recentes(&p, recentes, PILHA_LIMITE_EXIBICAO);
    assert(copiados == 5);
    assert(recentes[0].tipo == ACAO_REMOVER_REGISTRO && recentes[0].id1 == 10);
    assert(recentes[1].tipo == ACAO_REMOVER_CONEXAO && recentes[1].id1 == 10 && recentes[1].id2 == 20);
    assert(recentes[2].tipo == ACAO_ADICIONAR_CONEXAO && recentes[2].id1 == 20 && recentes[2].id2 == 30);
    assert(recentes[3].tipo == ACAO_CADASTRAR_REGISTRO && recentes[3].id1 == 30);
    assert(recentes[4].tipo == ACAO_ADICIONAR_CONEXAO && recentes[4].id1 == 10 && recentes[4].id2 == 20);
    assert(pilha_tamanho(&p) == 7); /* todas as 7 continuam na pilha dinamica */

    printf("  - Mais de 5 Acoes (7) | Esperado: consulta retorna 5 mais recentes, tam=7 | Obtido: copiados=%lu, tam=%lu -> OK\n",
           (unsigned long)copiados, (unsigned long)pilha_tamanho(&p));
    pilha_exibir_ultimas_cinco(&p);

    /* Cenario 3.5: Pop remove o topo e revela acao anterior na proxima consulta */
    AcaoHistorico desempilhada;
    assert(pilha_pop(&p, &desempilhada) == 1);
    assert(desempilhada.tipo == ACAO_REMOVER_REGISTRO && desempilhada.id1 == 10);
    assert(pilha_tamanho(&p) == 6);

    copiados = pilha_consultar_recentes(&p, recentes, PILHA_LIMITE_EXIBICAO);
    assert(copiados == 5);
    assert(recentes[0].tipo == ACAO_REMOVER_CONEXAO);
    assert(recentes[4].tipo == ACAO_CADASTRAR_REGISTRO && recentes[4].id1 == 20);
    printf("  - Operacao Pop | Topo removido (Remocao ID 10), novo tam=%lu -> OK\n",
           (unsigned long)pilha_tamanho(&p));

    pilha_liberar(&p);
    assert(pilha_vazia(&p) == 1);
    assert(pilha_tamanho(&p) == 0);
    printf("  - Liberacao integral da pilha dinamica -> OK\n");
}

int main(void) {
    printf("============================================================\n");
    printf(" BATERIA DE TESTES - ETAPA 2A (LISTA, FILA E PILHA)\n");
    printf("============================================================\n");
    test_lista_encadeada();
    test_fila_dinamica();
    test_pilha_historico();
    printf("\n============================================================\n");
    printf(" TODOS OS TESTES DAS ESTRUTURAS LINEARES PASSARAM!\n");
    printf("============================================================\n");
    return 0;
}
