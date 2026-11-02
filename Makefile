CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude -O2
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

TARGET = $(BIN_DIR)/sistema.exe
TEST_MODELAGEM = $(BIN_DIR)/test_modelagem.exe

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BIN_DIR) $(BUILD_DIR)

$(TARGET): $(SRC_DIR)/main.c $(SRC_DIR)/registro.c $(SRC_DIR)/util.c
	$(CC) $(CFLAGS) $^ -o $@

test: dirs $(TEST_MODELAGEM)
	@echo ====================================
	@echo Executando bateria de testes...
	@echo ====================================
	@./$(TEST_MODELAGEM)

$(TEST_MODELAGEM): $(TEST_DIR)/test_modelagem.c $(SRC_DIR)/registro.c $(SRC_DIR)/util.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: all dirs test clean
