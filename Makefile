# Uso: make TARGET=teste

# Nome do executável (deve ser passado como parâmetro: make TARGET=nome)
TARGET ?= invaders

# Arquivo-fonte baseado no TARGET
SRC = $(TARGET).c

# Flags do Allegro
ALLEGRO_FLAGS = $(shell pkg-config --libs --cflags \
	allegro-5 \
	allegro_image-5 \
	allegro_font-5 \
	allegro_ttf-5 \
	allegro_acodec-5 \
	allegro_audio-5 \
	allegro_primitives-5 \
	allegro_dialog-5)

# Compilador
CC = gcc

# Regra principal
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(ALLEGRO_FLAGS) -lm

# Limpar o executável
clean:
	rm -f $(TARGET)
