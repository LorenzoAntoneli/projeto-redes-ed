# Projeto Prático Integrado de Estruturas de Dados (FATEC Ribeirão Preto)

**Curso:** Tecnologia em Análise e Desenvolvimento de Sistemas (ADS) — 3º Semestre  
**Disciplina:** Estrutura de Dados  
**Docente:** Prof. Me. Heber Castro  
**Tema Escolhido:** Tema A — Gerenciamento de Redes (Equipamentos e Conexões)  
**Linguagem:** C (Padrão C99)  

---

## 1. Visão Geral do Projeto

Este projeto consiste em uma aplicação de terminal em C com arquitetura modular baseada em Tipos Abstratos de Dados (TADs), integrando:
- **Árvore Binária de Busca (BST):** Indexação primária de equipamentos por ID único positivo.
- **Grafo Não Direcionado e Não Ponderado:** Mapeamento de conexões físicas/lógicas de rede entre equipamentos (capacidade para 100+ vértices com mapeamento ID $\to$ índice compacto).
- **Lista Encadeada Simples:** Armazenamento da ordem de descoberta na Busca em Largura (BFS) com inserção no final $O(1)$.
- **Fila Dinâmica Encadeada:** Controle da fila de processamento na travessia BFS.
- **Pilha Dinâmica Encadeada:** Histórico não destrutivo das últimas 5 operações bem-sucedidas de alteração de estado (cadastro, remoção de registro, adição e remoção de conexões).
- **Algoritmo de Ordenação Próprio:** Ordenação sobre vetor auxiliar de cópias independentes dos registros, ordenados por nome via `strcmp()` com desempate por ID crescente.
- **Tratamento de Entradas Seguras e Consistência (Rollback):** Validação estrita contra overflow, buffer overrun, entradas vazias e garantia atômica entre BST e Grafo.

---

## 2. Modelagem de Dados (Tema A — Redes)

Cada equipamento possui os seguintes campos validados:
- `id`: Inteiro positivo e estritamente único ($id > 0$).
- `nome`: Chave textual com até 79 caracteres úteis (`char[80]`), aceita espaços e requer pelo menos um caractere não branco.
- `tipo_equipamento`: Identificação técnica (`char[80]`, ex.: Roteador, Switch Core, Firewall, Ponto de Acesso).
- `localizacao`: Local físico ou de infraestrutura (`char[80]`, ex.: Rack A - Sala de Servidores, Bloco 2).

---

## 3. Estrutura do Repositório

```text
projeto-redes-ed/
├── include/
│   ├── registro.h      # TAD do Equipamento (Tema A) e comparadores
│   ├── util.h          # Leitura segura de entradas (fgets, strtol, limpeza de buffer)
│   ├── lista.h         # TAD Lista Encadeada (ordem da BFS)
│   ├── fila.h          # TAD Fila Encadeada (fila de BFS)
│   ├── pilha.h         # TAD Pilha de Histórico (últimas ações)
│   ├── ordenacao.h     # Algoritmo de ordenação próprio (Merge Sort O(n log n))
│   ├── bst.h           # TAD Árvore Binária de Busca (BST)
│   └── grafo.h         # TAD Grafo Não Direcionado e BFS
├── src/
│   ├── main.c          # Ponto de entrada do sistema interativo de terminal
│   ├── registro.c      # Implementação do TAD Equipamento
│   ├── util.c          # Implementação das rotinas seguras de I/O
│   ├── lista.c         # Implementação da lista encadeada
│   ├── fila.c          # Implementação da fila
│   ├── pilha.c         # Implementação da pilha
│   ├── ordenacao.c     # Implementação do algoritmo de ordenação
│   ├── bst.c           # Implementação da árvore binária de busca
│   └── grafo.c         # Implementação do grafo e travessia BFS
├── tests/
│   ├── test_modelagem.c  # Testes de modelagem e comparadores
│   ├── test_lineares.c   # Testes de lista, fila e pilha
│   ├── test_ordenacao.c  # Testes de ordenação e desempate
│   ├── test_bst.c        # Testes de inserção, busca e remoção na BST
│   ├── test_grafo.c      # Testes de vértices, arestas e BFS no Grafo
│   └── test_integracao.c # Testes de fluxo integrado e rollback
├── docs/
│   └── relatorio_tecnico.pdf # Relatório técnico de 6 a 10 páginas
├── Makefile            # Automação de compilação e execução de testes
├── .gitignore          # Filtro de arquivos compilados e temporários
└── README.md           # Documentação completa
```

---

## 4. Planejamento de Desenvolvimento Contínuo (Semana de Entrega)

Em conformidade com a Seção 14 do regulamento (Desenvolvimento Contínuo no Git), o projeto é distribuído nos seguintes marcos ao longo da semana:

| Marco | Etapa do Regulamento | Descrição e Entregáveis |
|---|---|---|
| **Marco 1 (02/11)** | Modelagem e Estrutura | Estrutura de diretórios, `.gitignore`, `Makefile`, TAD `registro`, validação segura de entradas (`util`) e testes iniciais. |
| **Marco 2 (03/11)** | Estruturas Lineares & Ordenação | TAD Lista encadeada com inserção no fim, Fila dinâmica, Pilha de histórico (últimas 5 ações) e algoritmo de ordenação próprio. |
| **Marco 3 (04/11)** | Árvore Binária de Busca (BST) | TAD BST completa (inserção, busca, remoção com 0/1/2 filhos, percurso em-ordem, menor/maior ID) e bateria de testes reproduzíveis. |
| **Marco 4 (05/11)** | Grafo e Travessia BFS | TAD Grafo não direcionado (capacidade 100+, mapeamento ID $\to$ índice), arestas, BFS em ordem crescente de ID e consulta de caminho. |
| **Marco 5 (06/11)** | Integração & Consistência | Acoplamento atômico BST + Grafo, estratégia de Rollback, menu terminal interativo completo e testes integrados. |
| **Marco 6 (07/11)** | Documentação & Relatório Técnico | Relatório técnico em PDF (6 a 10 páginas) com modelagem, complexidades assintóticas $O(n)$, testes com evidências e entrega final. |

---

## 5. Como Compilar e Executar

### Pré-requisitos
- Compilador C compatível com C99 (`gcc` ou `clang`)
- GNU Make

### Compilar a Aplicação Principal
```bash
make
```

### Executar a Aplicação
```bash
./bin/sistema.exe
```

### Executar a Bateria de Testes
```bash
make test
```

### Limpar Arquivos de Build
```bash
make clean
```
