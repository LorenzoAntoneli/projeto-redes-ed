#include <stdio.h>
#include <stdlib.h>
#include "registro.h"
#include "util.h"
#include "pilha.h"
#include "ordenacao.h"

static void exibir_menu(void) {
    printf("\n");
    printf("============================================\n");
    printf(" SISTEMA DE GERENCIAMENTO DE REDES (TEMA A) \n");
    printf("============================================\n");
    printf("1 - Cadastrar equipamento (cria vertice)\n");
    printf("2 - Buscar equipamento (BST)\n");
    printf("3 - Remover equipamento e conexoes\n");
    printf("4 - Listar equipamentos (BST em-ordem)\n");
    printf("5 - Ordenar e exibir equipamentos por nome\n");
    printf("6 - Adicionar conexao de rede (aresta)\n");
    printf("7 - Remover conexao de rede (aresta)\n");
    printf("8 - Consultar conexao direta\n");
    printf("9 - Listar alcancaveis na rede (BFS + lista)\n");
    printf("10 - Verificar caminho entre dois equipamentos\n");
    printf("11 - Consultar historico de alteracoes (pilha)\n");
    printf("0 - Encerrar\n");
    printf("============================================\n");
}

int main(void) {
    int opcao = -1;
    Pilha historico;
    pilha_inicializar(&historico);

    printf("========================================================\n");
    printf(" FATEC Ribeirao Preto - ADS 3o Semestre                 \n");
    printf(" Disciplina: Estrutura de Dados | Prof. Me. Heber Castro\n");
    printf(" Projeto Pratico Individual - Tema A: Redes             \n");
    printf("========================================================\n");

    do {
        exibir_menu();
        if (!util_ler_inteiro("Escolha uma opcao: ", &opcao, 0, 11)) {
            continue;
        }

        switch (opcao) {
            case 1:
                printf("\n[Marco 2 ativo] Cadastro integrado (BST + Grafo) sera ativado no Marco 5.\n");
                util_pausar();
                break;
            case 2:
                printf("\n[Marco 2 ativo] Modulo BST sera implementado no Marco 3.\n");
                util_pausar();
                break;
            case 3:
                printf("\n[Marco 2 ativo] Remocao integrada sera concluida no Marco 5.\n");
                util_pausar();
                break;
            case 4:
                printf("\n[Marco 2 ativo] Percurso em-ordem disponivel no Marco 3.\n");
                util_pausar();
                break;
            case 5:
                exibir_equipamentos_ordenados(NULL, 0);
                printf("[Marco 2 ativo] Merge Sort pronto; copia da BST sera integrada a partir do Marco 3.\n");
                util_pausar();
                break;
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                printf("\n[Marco 2 ativo] Modulo de Grafo/BFS disponivel no Marco 4.\n");
                util_pausar();
                break;
            case 11:
                pilha_exibir_ultimas_cinco(&historico);
                util_pausar();
                break;
            case 0:
                printf("\nEncerrando o sistema com sucesso. Liberando recursos...\n");
                pilha_liberar(&historico);
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
