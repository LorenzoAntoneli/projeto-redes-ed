CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude -O2
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

CORE_SRCS = $(SRC_DIR)/registro.c \
            $(SRC_DIR)/util.c \
            $(SRC_DIR)/lista.c \
            $(SRC_DIR)/fila.c \
            $(SRC_DIR)/pilha.c \
            $(SRC_DIR)/ordenacao.c

TARGET = $(BIN_DIR)/sistema.exe
TEST_MODELAGEM = $(BIN_DIR)/test_modelagem.exe
TEST_LINEARES = $(BIN_DIR)/test_lineares.exe
TEST_ORDENACAO = $(BIN_DIR)/test_ordenacao.exe

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BIN_DIR) $(BUILD_DIR)

$(TARGET): $(SRC_DIR)/main.c $(CORE_SRCS)
	$(CC) $(CFLAGS) $^ -o $@

test: dirs $(TEST_MODELAGEM) $(TEST_LINEARES) $(TEST_ORDENACAO)
	@echo ====================================
	@echo Executando bateria de testes...
	@echo ====================================
	@./$(TEST_MODELAGEM)
	@./$(TEST_LINEARES)
	@./$(TEST_ORDENACAO)

$(TEST_MODELAGEM): $(TEST_DIR)/test_modelagem.c $(SRC_DIR)/registro.c $(SRC_DIR)/util.c
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_LINEARES): $(TEST_DIR)/test_lineares.c $(SRC_DIR)/lista.c $(SRC_DIR)/fila.c $(SRC_DIR)/pilha.c
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_ORDENACAO): $(TEST_DIR)/test_ordenacao.c $(SRC_DIR)/registro.c $(SRC_DIR)/ordenacao.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: all dirs test clean
