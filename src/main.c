#include <stdio.h>
#include <stdlib.h>
#include "registro.h"
#include "util.h"
#include "lista.h"
#include "fila.h"
#include "pilha.h"
#include "ordenacao.h"
#include "bst.h"

static void exibir_menu(void) {
    printf("\n");
    printf("==========================================================\n");
    printf("      SISTEMA DE GERENCIAMENTO DE REDES (TEMA A)          \n");
    printf("==========================================================\n");
    printf(" 1  - Cadastrar equipamento (BST)\n");
    printf(" 2  - Buscar equipamento por ID (BST)\n");
    printf(" 3  - Remover equipamento por ID (BST)\n");
    printf(" 4  - Listar equipamentos em ordem de ID (BST em-ordem)\n");
    printf(" 5  - Consultar menor e maior ID cadastrados (BST)\n");
    printf(" 6  - Ordenar e exibir equipamentos por nome (Merge Sort)\n");
    printf(" 7  - Filtrar equipamentos por termo (Lista Encadeada)\n");
    printf(" 8  - Agendar equipamento para manutencao (Fila enqueue)\n");
    printf(" 9  - Atender proximo equipamento da fila (Fila dequeue)\n");
    printf(" 10 - Consultar fila de manutencao (Fila FIFO)\n");
    printf(" 11 - Consultar historico de alteracoes (Pilha)\n");
    printf(" 0  - Encerrar\n");
    printf("==========================================================\n");
}

static void operacao_cadastrar(ArvoreBST *arvore, Pilha *historico) {
    int id = 0;
    char nome[TAM_TEXTO_MAX];
    char tipo[TAM_TEXTO_MAX];
    char localizacao[TAM_TEXTO_MAX];

    printf("\n--- [1] Cadastrar Novo Equipamento ---\n");
    if (!util_ler_id_positivo("ID do equipamento (inteiro > 0): ", &id)) {
        return;
    }

    if (bst_buscar(arvore, id) != NULL) {
        printf(">> [ERRO] Ja existe um equipamento cadastrado com o ID %d.\n", id);
        return;
    }

    if (!util_ler_texto("Nome do equipamento (ate 79 chars): ", nome, TAM_TEXTO_MAX)) {
        return;
    }
    if (!util_ler_texto("Tipo de equipamento (ex.: Switch L3, Roteador): ", tipo, TAM_TEXTO_MAX)) {
        return;
    }
    if (!util_ler_texto("Localizacao fisica (ex.: Rack A - CPD): ", localizacao, TAM_TEXTO_MAX)) {
        return;
    }

    Equipamento novo = criar_equipamento(id, nome, tipo, localizacao);
    if (!bst_inserir(arvore, novo)) {
        printf(">> [ERRO] Falha ao inserir equipamento na arvore (memoria insuficiente ou dados invalidos).\n");
        return;
    }

    pilha_registrar(historico, ACAO_CADASTRAR_REGISTRO, id);
    printf("\n>> [SUCESSO] Equipamento cadastrado na BST com sucesso!\n");
    exibir_equipamento_detalhado(&novo);
}

static void operacao_buscar(const ArvoreBST *arvore) {
    printf("\n--- [2] Buscar Equipamento por ID ---\n");
    if (bst_vazia(arvore)) {
        printf(">> Nenhum equipamento cadastrado no sistema.\n");
        return;
    }

    int id = 0;
    if (!util_ler_id_positivo("Informe o ID a buscar: ", &id)) {
        return;
    }

    const Equipamento *encontrado = bst_buscar(arvore, id);
    if (!encontrado) {
        printf(">> [AVISO] Equipamento com ID %d nao encontrado na BST.\n", id);
        return;
    }

    printf("\n>> Equipamento localizado:\n");
    exibir_equipamento_detalhado(encontrado);
}

static void operacao_remover(ArvoreBST *arvore, Fila *fila_manutencao, Pilha *historico) {
    printf("\n--- [3] Remover Equipamento por ID ---\n");
    if (bst_vazia(arvore)) {
        printf(">> Nenhum equipamento cadastrado para remover.\n");
        return;
    }

    int id = 0;
    if (!util_ler_id_positivo("Informe o ID do equipamento a remover: ", &id)) {
        return;
    }

    Equipamento removido;
    if (!bst_remover(arvore, id, &removido)) {
        printf(">> [ERRO] Equipamento com ID %d nao existe na BST. Nenhuma alteracao realizada.\n", id);
        return;
    }

    /* Preserva consistência: se o equipamento estava na fila de manutenção, remove-o também */
    int removido_da_fila = fila_remover_valor(fila_manutencao, id);

    pilha_registrar(historico, ACAO_REMOVER_REGISTRO, id);
    printf("\n>> [SUCESSO] Equipamento removido da BST:\n");
    exibir_equipamento_detalhado(&removido);
    if (removido_da_fila) {
        printf(">> [CONSISTENCIA] O agendamento pendente deste equipamento tambem foi removido da fila de manutencao.\n");
    }
}

static void operacao_menor_maior(const ArvoreBST *arvore) {
    printf("\n--- [5] Consultar Menor e Maior ID na BST ---\n");
    Equipamento menor, maior;

    if (!bst_obter_menor(arvore, &menor) || !bst_obter_maior(arvore, &maior)) {
        printf(">> Nenhum equipamento cadastrado na arvore (BST vazia).\n");
        return;
    }

    printf("\n[Menor ID cadastrado na extrema esquerda da BST]\n");
    exibir_equipamento_detalhado(&menor);

    printf("\n[Maior ID cadastrado na extrema direita da BST]\n");
    exibir_equipamento_detalhado(&maior);
}

static void operacao_ordenar_por_nome(const ArvoreBST *arvore) {
    printf("\n--- [6] Ordenar Equipamentos por Nome (Vetor Auxiliar) ---\n");
    if (bst_vazia(arvore)) {
        exibir_equipamentos_ordenados(NULL, 0);
        return;
    }

    Equipamento *vetor_aux = NULL;
    size_t qtd = 0;

    if (!bst_copiar_para_vetor(arvore, &vetor_aux, &qtd)) {
        printf(">> [ERRO] Falha ao alocar vetor auxiliar para ordenacao.\n");
        return;
    }

    if (!ordenar_equipamentos_por_nome(vetor_aux, qtd)) {
        printf(">> [ERRO] Falha de memoria durante a execucao do Merge Sort.\n");
        free(vetor_aux);
        return;
    }

    exibir_equipamentos_ordenados(vetor_aux, qtd);
    printf("  (Nota: Ordenacao realizada em copia independente; a estrutura da BST permaneceu intacta.)\n");

    free(vetor_aux);
}

static void operacao_filtrar_lista(const ArvoreBST *arvore) {
    printf("\n--- [7] Filtrar Equipamentos (Lista Encadeada Simples) ---\n");
    if (bst_vazia(arvore)) {
        printf(">> Nenhum equipamento cadastrado para filtrar.\n");
        return;
    }

    char termo[TAM_TEXTO_MAX];
    if (!util_ler_texto("Digite parte do nome, tipo ou localizacao: ", termo, TAM_TEXTO_MAX)) {
        return;
    }

    Lista ids_encontrados;
    lista_inicializar(&ids_encontrados);

    if (!bst_filtrar_por_termo(arvore, termo, &ids_encontrados)) {
        printf(">> [ERRO] Nao foi possivel concluir a filtragem.\n");
        lista_liberar(&ids_encontrados);
        return;
    }

    if (lista_vazia(&ids_encontrados)) {
        printf(">> Nenhum equipamento corresponde ao termo '%s'.\n", termo);
        lista_liberar(&ids_encontrados);
        return;
    }

    printf("\nSequencia na Lista Encadeada (insercao no fim O(1) - %lu item(ns)):\n  IDs: ",
           (unsigned long)lista_tamanho(&ids_encontrados));
    lista_exibir(&ids_encontrados);

    printf("\nDetalhes dos equipamentos filtrados:\n");
    exibir_cabecalho_equipamentos();
    size_t total = lista_tamanho(&ids_encontrados);
    for (size_t i = 0; i < total; i++) {
        int id_item = 0;
        if (lista_obter_em(&ids_encontrados, i, &id_item)) {
            const Equipamento *eq = bst_buscar(arvore, id_item);
            if (eq) {
                exibir_equipamento_linha(eq);
            }
        }
    }
    printf("+-------+--------------------------------+----------------------+----------------------+\n");

    lista_liberar(&ids_encontrados);
}

static void operacao_agendar_manutencao(const ArvoreBST *arvore, Fila *fila_manutencao, Pilha *historico) {
    printf("\n--- [8] Agendar Equipamento na Fila de Manutencao ---\n");
    if (bst_vazia(arvore)) {
        printf(">> Cadastre ao menos um equipamento na BST antes de agendar manutencao.\n");
        return;
    }

    int id = 0;
    if (!util_ler_id_positivo("Informe o ID do equipamento para manutencao: ", &id)) {
        return;
    }

    const Equipamento *eq = bst_buscar(arvore, id);
    if (!eq) {
        printf(">> [ERRO] Equipamento com ID %d nao existe na BST.\n", id);
        return;
    }

    if (fila_contem(fila_manutencao, id)) {
        printf(">> [ERRO] O equipamento ID %d ('%s') ja aguarda atendimento na fila de manutencao.\n",
               id, eq->nome);
        return;
    }

    if (!fila_enfileirar(fila_manutencao, id)) {
        printf(">> [ERRO] Falha de alocacao ao enfileirar equipamento.\n");
        return;
    }

    pilha_registrar(historico, ACAO_AGENDAR_MANUTENCAO, id);
    printf(">> [SUCESSO] Equipamento ID %d ('%s') inserido no fim da fila de manutencao (posicao #%lu).\n",
           id, eq->nome, (unsigned long)fila_tamanho(fila_manutencao));
}

static void operacao_atender_manutencao(const ArvoreBST *arvore, Fila *fila_manutencao, Pilha *historico) {
    printf("\n--- [9] Atender Proximo Equipamento da Manutencao (Dequeue) ---\n");
    int id_atendido = 0;
    if (!fila_desenfileirar(fila_manutencao, &id_atendido)) {
        printf(">> A fila de manutencao esta vazia no momento.\n");
        return;
    }

    pilha_registrar(historico, ACAO_ATENDER_MANUTENCAO, id_atendido);
    printf(">> [SUCESSO] Equipamento retirado da frente da fila para atendimento tecnico:\n");

    const Equipamento *eq = bst_buscar(arvore, id_atendido);
    if (eq) {
        exibir_equipamento_detalhado(eq);
    } else {
        printf("  Equipamento ID: %d\n", id_atendido);
    }
    printf("  Equipamentos restantes aguardando na fila: %lu\n",
           (unsigned long)fila_tamanho(fila_manutencao));
}

static void operacao_consultar_fila(const ArvoreBST *arvore, const Fila *fila_manutencao) {
    printf("\n--- [10] Consultar Fila de Manutencao (FIFO) ---\n");
    if (fila_vazia(fila_manutencao)) {
        printf(">> Nenhum equipamento aguardando na fila de manutencao.\n");
        return;
    }

    int id_frente = 0;
    if (fila_frente(fila_manutencao, &id_frente)) {
        const Equipamento *prox = bst_buscar(arvore, id_frente);
        printf(">> Proximo a ser atendido (frente da fila): ID %d (%s)\n\n",
               id_frente, prox ? prox->nome : "N/D");
    }

    printf("Ordem de atendimento na Fila Dinamica (%lu equipamento(s)):\n",
           (unsigned long)fila_tamanho(fila_manutencao));
    exibir_cabecalho_equipamentos();

    size_t total = fila_tamanho(fila_manutencao);
    for (size_t i = 0; i < total; i++) {
        int id_item = 0;
        if (fila_obter_em(fila_manutencao, i, &id_item)) {
            const Equipamento *eq = bst_buscar(arvore, id_item);
            if (eq) {
                exibir_equipamento_linha(eq);
            }
        }
    }
    printf("+-------+--------------------------------+----------------------+----------------------+\n");
}

int main(void) {
    int opcao = -1;
    ArvoreBST arvore;
    Fila fila_manutencao;
    Pilha historico;

    bst_inicializar(&arvore);
    fila_inicializar(&fila_manutencao);
    pilha_inicializar(&historico);

    printf("==========================================================\n");
    printf(" FATEC Ribeirao Preto - ADS 3o Semestre                   \n");
    printf(" Disciplina: Estrutura de Dados | Prof. Me. Heber Castro  \n");
    printf(" Projeto Pratico Individual - Tema A: Redes               \n");
    printf("==========================================================\n");

    do {
        exibir_menu();
        if (!util_ler_inteiro("Escolha uma opcao: ", &opcao, 0, 11)) {
            if (feof(stdin)) {
                printf("\nFim de entrada detectado (EOF). Encerrando com seguranca...\n");
                break;
            }
            continue;
        }

        switch (opcao) {
            case 1:
                operacao_cadastrar(&arvore, &historico);
                util_pausar();
                break;
            case 2:
                operacao_buscar(&arvore);
                util_pausar();
                break;
            case 3:
                operacao_remover(&arvore, &fila_manutencao, &historico);
                util_pausar();
                break;
            case 4:
                bst_exibir_em_ordem(&arvore);
                util_pausar();
                break;
            case 5:
                operacao_menor_maior(&arvore);
                util_pausar();
                break;
            case 6:
                operacao_ordenar_por_nome(&arvore);
                util_pausar();
                break;
            case 7:
                operacao_filtrar_lista(&arvore);
                util_pausar();
                break;
            case 8:
                operacao_agendar_manutencao(&arvore, &fila_manutencao, &historico);
                util_pausar();
                break;
            case 9:
                operacao_atender_manutencao(&arvore, &fila_manutencao, &historico);
                util_pausar();
                break;
            case 10:
                operacao_consultar_fila(&arvore, &fila_manutencao);
                util_pausar();
                break;
            case 11:
                pilha_exibir_ultimas_cinco(&historico);
                util_pausar();
                break;
            case 0:
                printf("\nEncerrando o sistema com sucesso. Liberando toda a memoria alocada...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0 && !feof(stdin));

    bst_liberar(&arvore);
    fila_liberar(&fila_manutencao);
    pilha_liberar(&historico);

    return 0;
}
