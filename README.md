# Projeto Prático Individual de Estrutura de Dados (FATEC Ribeirão Preto)

**Curso:** Tecnologia em Análise e Desenvolvimento de Sistemas (ADS) — 3º Semestre  
**Disciplina:** Estrutura de Dados  
**Docente:** Prof. Me. Heber Castro  
**Autor:** Lorenzo Henrique Antoneli  
**Tema Escolhido:** Tema A — Gerenciamento de Redes (Equipamentos de Infraestrutura)  
**Linguagem:** C (Padrão ISO C99)  
**Data de Entrega:** 14/11/2026 (Prazo oficial prorrogado via Google Classroom)  

---

## 1. Visão Geral do Projeto

Este projeto consiste em uma aplicação interativa de terminal em C com arquitetura modular baseada em Tipos Abstratos de Dados (TADs), integrando:

- **Árvore Binária de Busca (BST):** Estrutura hierárquica central para indexação primária de equipamentos por `id` único positivo, suportando inserção, busca em $O(h)$, remoção nos 3 casos clássicos (nó folha, 1 filho e 2 filhos via sucessor em-ordem), percursos em profundidade (**Em-Ordem**, **Pré-Ordem** e **Pós-Ordem**), cálculo de altura $h$ e consulta de menor/maior ID.
- **Lista Encadeada Simples:** Coleta ordenada de IDs em percursos e filtragens textuais sobre a BST com inserção no final em $O(1)$ através de ponteiro descritor `fim`.
- **Fila Dinâmica Encadeada (FIFO):** Gerenciamento de fila de manutenção técnica de equipamentos cadastrados (`enqueue`, `dequeue`, consulta de `frente` e remoção consistente quando um equipamento é excluído da BST).
- **Pilha Dinâmica Encadeada (LIFO):** Histórico não destrutivo das últimas 5 operações bem-sucedidas que alteram o estado do sistema (`push`, `pop`, `peek` e consulta não destrutiva).
- **Algoritmo de Ordenação Próprio (Merge Sort):** Ordenação estável em $\Theta(n \log n)$ sobre vetor auxiliar de cópias independentes por valor dos registros da BST, ordenados alfabeticamente por `nome` via `strcmp()` com desempate por `id` crescente, preservando a BST intacta.
- **Validação Segura de Entradas e Consistência:** Proteção contra *buffer overflow*, limpeza automática de resíduos em `stdin`, rejeição de strings vazias/apenas espaços ou acima de 79 caracteres úteis, validação numérica via `strtol()` e consistência atômica entre BST e Fila.

> **Nota de Escopo (Comunicado Oficial da Disciplina):** Em conformidade com o ajuste de calendário acadêmico divulgado pela disciplina (prazo prorrogado para **14/11/2026**), os conteúdos de Grafos e Busca em Largura (BFS) deixaram de integrar os requisitos obrigatórios, mantendo-se integralmente válidos os requisitos de Árvores (BST), estruturas lineares (Lista, Fila e Pilha), ordenação própria, TADs, testes reproduzíveis, versionamento Git e relatório técnico.

---

## 2. Modelagem de Dados (Tema A — Redes)

Cada equipamento de rede (`Equipamento`) possui os seguintes campos validados:
- `id`: Inteiro positivo e estritamente único (`id > 0`).
- `nome`: Chave textual com até 79 caracteres úteis (`char[80]`), aceita espaços e exige ao menos um caractere não branco.
- `tipo_equipamento`: Classificação técnica (`char[80]`, ex.: *Roteador, Switch L3, Firewall, Access Point*).
- `localizacao`: Local físico na infraestrutura (`char[80]`, ex.: *Rack A - CPD, Sala Telecom, Bloco B*).

---

## 3. Estrutura do Repositório

```text
projeto-redes-ed/
├── include/
│   ├── registro.h        # TAD Equipamento (Tema A) e comparador textual/ID
│   ├── util.h            # Leitura segura de entradas (fgets, strtol, limpeza de stdin)
│   ├── lista.h           # TAD Lista Encadeada Simples (inserção no fim O(1))
│   ├── fila.h            # TAD Fila Dinâmica FIFO (fila de manutenção de equipamentos)
│   ├── pilha.h           # TAD Pilha Dinâmica LIFO (histórico das últimas 5 alterações)
│   ├── ordenacao.h       # Algoritmo de ordenação próprio (Merge Sort O(n log n))
│   └── bst.h             # TAD Árvore Binária de Busca (BST: inserir, buscar, remover, em/pré/pós-ordem)
├── src/
│   ├── main.c            # Menu interativo de terminal integrando todas as estruturas
│   ├── registro.c        # Implementação do TAD Equipamento
│   ├── util.c            # Implementação das rotinas seguras de entrada
│   ├── lista.c           # Implementação da lista encadeada
│   ├── fila.c            # Implementação da fila dinâmica
│   ├── pilha.c           # Implementação da pilha de histórico
│   ├── ordenacao.c       # Implementação do Merge Sort sobre vetor auxiliar
│   └── bst.c             # Implementação da Árvore Binária de Busca (BST)
├── tests/
│   ├── test_modelagem.c  # Testes de criação, limites de 79 chars e comparadores
│   ├── test_lineares.c   # Testes de lista, fila FIFO e pilha de histórico
│   ├── test_ordenacao.c  # Testes do Merge Sort, desempate por ID e cópia independente
│   ├── test_bst.c        # Testes de inserção, busca, menor/maior, em/pré/pós-ordem e remoções (0/1/2 filhos)
│   └── test_integracao.c # Testes integrados (BST + Lista + Fila + Pilha + Entradas seguras)
├── docs/
│   ├── relatorio_tecnico.pdf # Relatório Técnico completo em PDF (8 páginas de conteúdo + Capa/Sumário/Refs)
│   └── prints/               # Evidências visuais (prints) de compilação, testes e execução do sistema
│       ├── 01_compilacao_e_git_log.png
│       ├── 02_bateria_testes_automatizados.png
│       ├── 03_menu_cadastro_e_percursos_bst.png
│       ├── 04_ordenacao_mergesort_e_filtro_lista.png
│       ├── 05_fila_manutencao_remocao_e_pilha_historico.png
│       └── 06_validacao_entradas_invalidas.png
├── Makefile              # Automação de compilação e execução da bateria de testes
├── .gitignore            # Exclusão de binários e arquivos temporários
└── README.md             # Instruções de compilação, execução, testes e documentação
```

---

## 4. Funcionalidades do Menu Interativo

| Opção | Operação | Estrutura Principal | Altera Histórico (Pilha)? |
|:---:|---|---|:---:|
| `1` | Cadastrar equipamento | BST (`bst_inserir`) | Sim |
| `2` | Buscar equipamento por ID | BST (`bst_buscar`) | Não |
| `3` | Remover equipamento por ID (0, 1 ou 2 filhos) | BST + Fila (consistência) | Sim |
| `4` | Listar equipamentos em ordem crescente de ID | BST (`bst_exibir_em_ordem`) | Não |
| `5` | Consultar menor/maior ID e percursos da BST | BST (`menor`/`maior` + Em/Pré/Pós-Ordem) | Não |
| `6` | Ordenar e exibir equipamentos por nome | Vetor Auxiliar + Merge Sort | Não |
| `7` | Filtrar equipamentos por termo textual | BST + Lista Encadeada $O(1)$ | Não |
| `8` | Agendar equipamento para manutenção | Fila Dinâmica (`enqueue`) | Sim |
| `9` | Atender próximo equipamento da manutenção | Fila Dinâmica (`dequeue`) | Sim |
| `10` | Consultar fila de manutenção | Fila Dinâmica (`frente` + FIFO) | Não |
| `11` | Consultar histórico recente (até 5 ações) | Pilha Dinâmica (não destrutiva) | Não |
| `0` | Encerrar sistema e liberar memória | Todas (`free` integral) | — |

---

## 5. Evolução Contínua no Git

| Etapa | Escopo | Status |
|---|---|---|
| **Etapa 1** | Estrutura do repositório, `Makefile`, modelagem `Equipamento` e leitura segura `util` | Concluída |
| **Etapa 2** | Estruturas lineares (`lista`, `fila`, `pilha`) e ordenação própria (`Merge Sort`) | Concluída |
| **Etapa 3** | Adequação ao escopo oficial (sem Grafo/BFS), implementação da Árvore Binária de Busca (`BST` com percursos Em-Ordem, Pré-Ordem e Pós-Ordem), integração completa no menu e testes automatizados | Concluída |
| **Etapa 4** | Captura das evidências visuais (`docs/prints/`), elaboração do Relatório Técnico em PDF (`docs/relatorio_tecnico.pdf`) e revisão final para entrega (14/11/2026) | Concluída |

---

## 6. Como Compilar e Executar

### Pré-requisitos
- Compilador C compatível com C99 (`gcc` ou `clang`)
- GNU Make (`make`)

### Compilar a Aplicação Principal
```bash
make
```

### Executar o Sistema Interativo
```bash
./bin/sistema.exe
```

### Executar toda a Bateria de Testes Reproduzíveis
```bash
make test
```

### Limpar Binários e Arquivos Temporários
```bash
make clean
```

---

## 7. Documentação Técnica e Evidências de Execução

- **Relatório Técnico em PDF:** Disponível em [`docs/relatorio_tecnico.pdf`](docs/relatorio_tecnico.pdf), contendo Capa, Sumário, **8 páginas de conteúdo técnico** (atendendo rigorosamente ao intervalo de 6 a 10 páginas de conteúdo da Seção 13), diagramas dos 3 casos de remoção da BST, análise de complexidade assintótica (Melhor, Médio e Pior Caso + Espaço), fundamentação teórica de Grafos/BFS, matriz de testes reproduzíveis, prints de execução e Referências/Declaração de Uso de Ferramentas de Apoio.
- **Prints de Execução:** Disponíveis em [`docs/prints/`](docs/prints/), comprovando compilação limpa (`-Wall -Wextra -pedantic -std=c99`), aprovação das 5 suítes de testes (`make test`), operações na BST, Merge Sort, Lista Encadeada $O(1)$, Fila FIFO com consistência atômica, Pilha LIFO e rejeição segura de entradas inválidas.
