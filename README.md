# Sistema de Gerenciamento de Equipamentos de Rede em C

Projeto prático desenvolvido para a disciplina de **Estrutura de Dados** do curso de Análise e Desenvolvimento de Sistemas (FATEC Ribeirão Preto).

- **Aluno:** Lorenzo Henrique Antoneli
- **Professor:** Me. Heber Castro
- **Linguagem:** C (padrão C99)

---

## Sobre o Projeto

O sistema simula o controle de equipamentos de infraestrutura de rede (switches, roteadores, firewalls e access points) pelo terminal. O objetivo é integrar diferentes estruturas de dados vistas em aula dentro de uma mesma aplicação:

- **Árvore Binária de Busca (BST):** estrutura principal onde os equipamentos ficam cadastrados e organizados pelo `id` único. Permite inserir, buscar, consultar menor/maior ID, percorrer a árvore (em-ordem, pré-ordem e pós-ordem) e remover tratando os 3 casos clássicos (nó folha, 1 filho e 2 filhos).
- **Merge Sort (em vetor auxiliar):** copia os equipamentos da árvore para um vetor separado e ordena em ordem alfabética de `nome` (usando o `id` como desempate), sem alterar os ponteiros da BST.
- **Lista Encadeada Simples:** guarda os IDs encontrados na busca por termo textual (nome, tipo ou localização), com inserção no fim em `O(1)`.
- **Fila Dinâmica (FIFO):** controla a fila de aparelhos agendados para manutenção técnica por ordem de chegada. Se um equipamento é excluído da BST, ele também é removido da fila.
- **Pilha Dinâmica (LIFO):** registra o histórico das alterações feitas no sistema (cadastros, remoções e manutenções) e permite consultar as 5 últimas ações sem apagar a pilha.

---

## Estrutura de Pastas

```text
projeto-redes-ed/
├── include/          # Cabeçalhos (.h) de cada TAD
├── src/              # Implementações (.c) e menu principal (main.c)
├── tests/            # Testes automatizados de cada estrutura
├── docs/
│   ├── relatorio_tecnico.pdf
│   └── prints/       # Prints de execução do sistema e dos testes
└── Makefile          # Script de compilação
```

---

## Opções do Menu

1. Cadastrar equipamento na BST
2. Buscar equipamento por ID
3. Remover equipamento por ID
4. Listar equipamentos em ordem crescente de ID (em-ordem)
5. Consultar menor/maior ID e percursos da BST
6. Ordenar e listar equipamentos por nome (Merge Sort)
7. Filtrar equipamentos por texto (Lista Encadeada)
8. Agendar equipamento para manutenção (Fila)
9. Atender próximo equipamento da fila de manutenção
10. Consultar fila de manutenção
11. Consultar histórico das 5 últimas alterações (Pilha)
0. Sair e liberar memória

---

## Como Compilar e Rodar

Para compilar o programa principal:
```bash
make
```

Para executar o sistema:
```bash
./bin/sistema.exe
```

Para rodar os testes automatizados:
```bash
make test
```

Para limpar os executáveis gerados:
```bash
make clean
```

---

## Documentação

O relatório técnico em PDF está na pasta [`docs/relatorio_tecnico.pdf`](docs/relatorio_tecnico.pdf) e as capturas de tela da execução estão em [`docs/prints/`](docs/prints/).
