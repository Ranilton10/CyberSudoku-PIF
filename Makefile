
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET = cyber_sudoku
SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
