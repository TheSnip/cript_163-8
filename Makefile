CC := gcc
CFLAGS := -ffast-math -Wall -Wextra
all: main

main: %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm main
run:
	./main
