CC := gcc
CFLAGS := -ffast-math
all: crypt

crypt: %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm crypt
test:
	./crypt && echo "\n"
	./crypt -e && echo "\n"
	./crypt -e test && echo "\n"
	./crypt -d BBBABAABBAABABBBBAABBBBBABAA
install:
	cp crypt /usr/local/bin/crypt
	chmod 755 /usr/local/bin/crypt
