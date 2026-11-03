#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>

static int validar_acao(AcaoHistorico *acao) {
    if (!acao) return 0;

    switch (acao->tipo) {
        case ACAO_CADASTRAR_REGISTRO:
        case ACAO_REMOVER_REGISTRO:
            if (acao->id1 <= 0) return 0;
            acao->id2 = 0;
            return 1;

        case ACAO_ADICIONAR_CONEXAO:
        case ACAO_REMOVER_CONEXAO:
            if (acao->id1 <= 0 || acao->id2 <= 0 || acao->id1 == acao->id2) {
                return 0;
            }
            return 1;

        default:
            return 0;
    }
}

void pilha_inicializar(Pilha *p) {
    if (!p) return;
    p->topo = NULL;
    p->tamanho = 0;
}

int pilha_vazia(const Pilha *p) {
    if (!p || p->topo == NULL) {
        return 1;
    }
    return 0;
}

size_t pilha_tamanho(const Pilha *p) {
    if (!p) return 0;
    return p->tamanho;
}

AcaoHistorico criar_acao_historico(TipoAcao tipo, int id1, int id2) {
    AcaoHistorico acao;
    acao.tipo = tipo;
    acao.id1 = id1;
    acao.id2 = id2;
    return acao;
}

const char *pilha_descricao_tipo(TipoAcao tipo) {
    switch (tipo) {
        case ACAO_CADASTRAR_REGISTRO:
            return "Cadastro de Equipamento";
        case ACAO_REMOVER_REGISTRO:
            return "Remocao de Equipamento";
        case ACAO_ADICIONAR_CONEXAO:
            return "Adicao de Conexao";
        case ACAO_REMOVER_CONEXAO:
            return "Remocao de Conexao";
        default:
            return "Operacao Desconhecida";
    }
}

int pilha_push(Pilha *p, AcaoHistorico acao) {
    if (!p || !validar_acao(&acao)) {
        return 0;
    }

    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (!novo) {
        return 0;
    }

    novo->acao = acao;
    novo->abaixo = p->topo;
    p->topo = novo;
    p->tamanho++;
    return 1;
}

int pilha_registrar(Pilha *p, TipoAcao tipo, int id1, int id2) {
    AcaoHistorico acao = criar_acao_historico(tipo, id1, id2);
    return pilha_push(p, acao);
}

int pilha_pop(Pilha *p, AcaoHistorico *saida) {
    if (!p || p->topo == NULL) {
        return 0;
    }

    NoPilha *removido = p->topo;
    if (saida) {
        *saida = removido->acao;
    }

    p->topo = removido->abaixo;
    free(removido);
    p->tamanho--;
    return 1;
}

int pilha_peek(const Pilha *p, AcaoHistorico *saida) {
    if (!p || p->topo == NULL || !saida) {
        return 0;
    }

    *saida = p->topo->acao;
    return 1;
}

size_t pilha_consultar_recentes(const Pilha *p, AcaoHistorico *destino, size_t max_itens) {
    if (!p || !destino || max_itens == 0) {
        return 0;
    }

    const NoPilha *atual = p->topo;
    size_t copiados = 0;

    while (atual != NULL && copiados < max_itens) {
        destino[copiados] = atual->acao;
        copiados++;
        atual = atual->abaixo;
    }

    return copiados;
}

void pilha_exibir_ultimas_cinco(const Pilha *p) {
    printf("\n+----------------------------------------------------------------------+\n");
    printf("| HISTORICO DE ALTERACOES RECENTES (ATE %d ACOES - MAIS RECENTE 1o)     |\n",
           PILHA_LIMITE_EXIBICAO);
    printf("+----------------------------------------------------------------------+\n");

    if (!p || pilha_vazia(p)) {
        printf("| Nenhuma alteracao de estado foi registrada no historico ate agora.   |\n");
        printf("+----------------------------------------------------------------------+\n");
        return;
    }

    const NoPilha *atual = p->topo;
    size_t exibidos = 0;

    while (atual != NULL && exibidos < PILHA_LIMITE_EXIBICAO) {
        const AcaoHistorico *ac = &atual->acao;
        exibidos++;

        if (ac->tipo == ACAO_CADASTRAR_REGISTRO || ac->tipo == ACAO_REMOVER_REGISTRO) {
            printf("| #%u | %-25s | Equipamento ID: %-18d |\n",
                   (unsigned)exibidos, pilha_descricao_tipo(ac->tipo), ac->id1);
        } else {
            char detalhe_ids[32];
            sprintf(detalhe_ids, "%d <-> %d", ac->id1, ac->id2);
            printf("| #%u | %-25s | Conexao IDs:    %-18s |\n",
                   (unsigned)exibidos, pilha_descricao_tipo(ac->tipo), detalhe_ids);
        }

        atual = atual->abaixo;
    }

    printf("+----------------------------------------------------------------------+\n");
    printf("  Total acumulado na pilha dinamica: %lu alteracao(oes).\n",
           (unsigned long)p->tamanho);
}

void pilha_liberar(Pilha *p) {
    if (!p) return;

    NoPilha *atual = p->topo;
    while (atual != NULL) {
        NoPilha *abaixo = atual->abaixo;
        free(atual);
        atual = abaixo;
    }

    p->topo = NULL;
    p->tamanho = 0;
}
