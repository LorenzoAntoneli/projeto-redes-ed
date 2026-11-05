#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static NoBST *criar_no_bst(Equipamento eq) {
    NoBST *novo = (NoBST *)malloc(sizeof(NoBST));
    if (!novo) {
        return NULL;
    }
    novo->dado = eq;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

static int altura_recursiva(const NoBST *no) {
    if (!no) {
        return -1;
    }
    int alt_esq = altura_recursiva(no->esq);
    int alt_dir = altura_recursiva(no->dir);
    return 1 + (alt_esq > alt_dir ? alt_esq : alt_dir);
}

static NoBST *encontrar_minimo(NoBST *no) {
    if (!no) return NULL;
    while (no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

static const NoBST *encontrar_minimo_const(const NoBST *no) {
    if (!no) return NULL;
    while (no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

static const NoBST *encontrar_maximo_const(const NoBST *no) {
    if (!no) return NULL;
    while (no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

static NoBST *remover_recursivo(NoBST *atual, int id, int *removeu, Equipamento *removido_saida) {
    if (!atual) {
        return NULL;
    }

    if (id < atual->dado.id) {
        atual->esq = remover_recursivo(atual->esq, id, removeu, removido_saida);
    } else if (id > atual->dado.id) {
        atual->dir = remover_recursivo(atual->dir, id, removeu, removido_saida);
    } else {
        /* Nó alvo encontrado */
        *removeu = 1;
        if (removido_saida) {
            *removido_saida = atual->dado;
        }

        /* Caso 1: Nó folha (0 filhos) */
        if (atual->esq == NULL && atual->dir == NULL) {
            free(atual);
            return NULL;
        }

        /* Caso 2a: Apenas filho à direita */
        if (atual->esq == NULL) {
            NoBST *filho_dir = atual->dir;
            free(atual);
            return filho_dir;
        }

        /* Caso 2b: Apenas filho à esquerda */
        if (atual->dir == NULL) {
            NoBST *filho_esq = atual->esq;
            free(atual);
            return filho_esq;
        }

        /* Caso 3: Dois filhos -> substitui pelo sucessor em-ordem (menor da subárvore direita) */
        NoBST *sucessor = encontrar_minimo(atual->dir);
        atual->dado = sucessor->dado;

        int remocao_interna = 0;
        atual->dir = remover_recursivo(atual->dir, sucessor->dado.id, &remocao_interna, NULL);
    }

    return atual;
}

static void exibir_em_ordem_recursivo(const NoBST *no) {
    if (!no) return;
    exibir_em_ordem_recursivo(no->esq);
    exibir_equipamento_linha(&no->dado);
    exibir_em_ordem_recursivo(no->dir);
}

static int coletar_ids_recursivo(const NoBST *no, Lista *destino) {
    if (!no) return 1;

    if (!coletar_ids_recursivo(no->esq, destino)) {
        return 0;
    }
    if (!lista_inserir_fim(destino, no->dado.id)) {
        return 0;
    }
    if (!coletar_ids_recursivo(no->dir, destino)) {
        return 0;
    }
    return 1;
}

static int contem_substring_ci(const char *texto, const char *termo) {
    if (!texto || !termo) return 0;
    if (*termo == '\0') return 1;

    size_t tam_termo = strlen(termo);
    for (const char *p = texto; *p != '\0'; p++) {
        size_t i = 0;
        while (i < tam_termo && p[i] != '\0' &&
               tolower((unsigned char)p[i]) == tolower((unsigned char)termo[i])) {
            i++;
        }
        if (i == tam_termo) {
            return 1;
        }
    }
    return 0;
}

static int filtrar_recursivo(const NoBST *no, const char *termo, Lista *ids_encontrados) {
    if (!no) return 1;

    if (!filtrar_recursivo(no->esq, termo, ids_encontrados)) {
        return 0;
    }

    if (contem_substring_ci(no->dado.nome, termo) ||
        contem_substring_ci(no->dado.tipo_equipamento, termo) ||
        contem_substring_ci(no->dado.localizacao, termo)) {
        if (!lista_inserir_fim(ids_encontrados, no->dado.id)) {
            return 0;
        }
    }

    if (!filtrar_recursivo(no->dir, termo, ids_encontrados)) {
        return 0;
    }

    return 1;
}

static void copiar_em_ordem_recursivo(const NoBST *no, Equipamento *vetor, size_t *indice) {
    if (!no) return;
    copiar_em_ordem_recursivo(no->esq, vetor, indice);
    vetor[*indice] = no->dado;
    (*indice)++;
    copiar_em_ordem_recursivo(no->dir, vetor, indice);
}

static void liberar_pos_ordem(NoBST *no) {
    if (!no) return;
    liberar_pos_ordem(no->esq);
    liberar_pos_ordem(no->dir);
    free(no);
}

void bst_inicializar(ArvoreBST *arvore) {
    if (!arvore) return;
    arvore->raiz = NULL;
    arvore->tamanho = 0;
}

int bst_vazia(const ArvoreBST *arvore) {
    if (!arvore || arvore->raiz == NULL) {
        return 1;
    }
    return 0;
}

size_t bst_tamanho(const ArvoreBST *arvore) {
    if (!arvore) return 0;
    return arvore->tamanho;
}

int bst_altura(const ArvoreBST *arvore) {
    if (!arvore || arvore->raiz == NULL) {
        return -1;
    }
    return altura_recursiva(arvore->raiz);
}

int bst_inserir(ArvoreBST *arvore, Equipamento eq) {
    if (!arvore || eq.id <= 0 || eq.nome[0] == '\0') {
        return 0;
    }

    if (arvore->raiz == NULL) {
        NoBST *novo = criar_no_bst(eq);
        if (!novo) return 0;
        arvore->raiz = novo;
        arvore->tamanho = 1;
        return 1;
    }

    NoBST *atual = arvore->raiz;
    NoBST *pai = NULL;

    while (atual != NULL) {
        pai = atual;
        if (eq.id < atual->dado.id) {
            atual = atual->esq;
        } else if (eq.id > atual->dado.id) {
            atual = atual->dir;
        } else {
            /* ID já existente: rejeita duplicata sem alterar a árvore */
            return 0;
        }
    }

    NoBST *novo = criar_no_bst(eq);
    if (!novo) {
        return 0;
    }

    if (eq.id < pai->dado.id) {
        pai->esq = novo;
    } else {
        pai->dir = novo;
    }

    arvore->tamanho++;
    return 1;
}

const Equipamento *bst_buscar(const ArvoreBST *arvore, int id) {
    if (!arvore || id <= 0) {
        return NULL;
    }

    const NoBST *atual = arvore->raiz;
    while (atual != NULL) {
        if (id < atual->dado.id) {
            atual = atual->esq;
        } else if (id > atual->dado.id) {
            atual = atual->dir;
        } else {
            return &atual->dado;
        }
    }

    return NULL;
}

int bst_remover(ArvoreBST *arvore, int id, Equipamento *removido_saida) {
    if (!arvore || arvore->raiz == NULL || id <= 0) {
        return 0;
    }

    int removeu = 0;
    arvore->raiz = remover_recursivo(arvore->raiz, id, &removeu, removido_saida);
    if (removeu) {
        arvore->tamanho--;
        return 1;
    }
    return 0;
}

int bst_obter_menor(const ArvoreBST *arvore, Equipamento *saida) {
    if (!arvore || arvore->raiz == NULL || !saida) {
        return 0;
    }

    const NoBST *min_no = encontrar_minimo_const(arvore->raiz);
    if (!min_no) return 0;

    *saida = min_no->dado;
    return 1;
}

int bst_obter_maior(const ArvoreBST *arvore, Equipamento *saida) {
    if (!arvore || arvore->raiz == NULL || !saida) {
        return 0;
    }

    const NoBST *max_no = encontrar_maximo_const(arvore->raiz);
    if (!max_no) return 0;

    *saida = max_no->dado;
    return 1;
}

void bst_exibir_em_ordem(const ArvoreBST *arvore) {
    if (!arvore || bst_vazia(arvore)) {
        printf("\n>> Nenhum equipamento cadastrado na arvore (BST vazia).\n");
        return;
    }

    printf("\nListagem de Equipamentos em Ordem Crescente de ID (BST em-ordem | Total: %lu | Altura: %d):\n",
           (unsigned long)arvore->tamanho, bst_altura(arvore));
    exibir_cabecalho_equipamentos();
    exibir_em_ordem_recursivo(arvore->raiz);
    printf("+-------+--------------------------------+----------------------+----------------------+\n");
}

int bst_coletar_ids_em_ordem(const ArvoreBST *arvore, Lista *destino) {
    if (!arvore || !destino) {
        return 0;
    }

    lista_liberar(destino);
    lista_inicializar(destino);

    if (!coletar_ids_recursivo(arvore->raiz, destino)) {
        lista_liberar(destino);
        return 0;
    }
    return 1;
}

int bst_filtrar_por_termo(const ArvoreBST *arvore, const char *termo, Lista *ids_encontrados) {
    if (!arvore || !termo || !ids_encontrados || termo[0] == '\0') {
        return 0;
    }

    lista_liberar(ids_encontrados);
    lista_inicializar(ids_encontrados);

    if (!filtrar_recursivo(arvore->raiz, termo, ids_encontrados)) {
        lista_liberar(ids_encontrados);
        return 0;
    }

    return 1;
}

int bst_copiar_para_vetor(const ArvoreBST *arvore, Equipamento **vetor_saida, size_t *qtd_saida) {
    if (!arvore || !vetor_saida || !qtd_saida) {
        return 0;
    }

    if (arvore->tamanho == 0) {
        *vetor_saida = NULL;
        *qtd_saida = 0;
        return 1;
    }

    Equipamento *vetor = (Equipamento *)malloc(arvore->tamanho * sizeof(Equipamento));
    if (!vetor) {
        return 0;
    }

    size_t indice = 0;
    copiar_em_ordem_recursivo(arvore->raiz, vetor, &indice);

    *vetor_saida = vetor;
    *qtd_saida = indice;
    return 1;
}

void bst_liberar(ArvoreBST *arvore) {
    if (!arvore) return;
    liberar_pos_ordem(arvore->raiz);
    arvore->raiz = NULL;
    arvore->tamanho = 0;
}
