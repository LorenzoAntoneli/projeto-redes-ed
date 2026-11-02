#include <stdio.h>
#include <stdlib.h>
#include "registro.h"
#include "util.h"

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
                printf("\n[Etapa 1 ativa] Modulo de cadastro sera integrado no Marco 5.\n");
                util_pausar();
                break;
            case 2:
                printf("\n[Etapa 1 ativa] Modulo BST sera implementado no Marco 3.\n");
                util_pausar();
                break;
            case 3:
                printf("\n[Etapa 1 ativa] Remocao integrada sera concluida no Marco 5.\n");
                util_pausar();
                break;
            case 4:
                printf("\n[Etapa 1 ativa] Percurso em-ordem disponivel no Marco 3.\n");
                util_pausar();
                break;
            case 5:
                printf("\n[Etapa 1 ativa] Algoritmo de ordenacao disponivel no Marco 2.\n");
                util_pausar();
                break;
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                printf("\n[Etapa 1 ativa] Modulo de Grafo/BFS disponivel no Marco 4.\n");
                util_pausar();
                break;
            case 11:
                printf("\n[Etapa 1 ativa] Pilha de historico disponivel no Marco 2.\n");
                util_pausar();
                break;
            case 0:
                printf("\nEncerrando o sistema com sucesso. Liberando recursos...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
