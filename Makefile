CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude

TARGET = api-client-wikipedia

SRC = src/main.c \
      src/worker.c \
      src/database.c \
      src/http.c \
      src/json.c

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -lsqlite3 -lcurl -lcjson -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
