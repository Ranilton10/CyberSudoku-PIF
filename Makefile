
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC = src/main.c src/menu.c src/jogo.c src/historico.c
EXEC = cyber_sudoku

all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LIBS)

run: all
	./$(EXEC)

clean:
	rm -f $(EXEC)
