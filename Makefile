CC=gcc
CFLAGS=-W -Wall -Wextra
BIN_DIR=bin
$(BIN_DIR)/ls: ls.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@
