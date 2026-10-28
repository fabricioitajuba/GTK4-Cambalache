# Variáveis para o compilador e flags
CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4)
LIBS = $(shell pkg-config --libs gtk4)

# Nome do executável final
TARGET = Ex03

# Arquivos fonte
SRCS = main.c

# Regra principal (padrão)
all: $(TARGET)

# Como construir o executável
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET)
