OPENSSL_PREFIX = $(shell brew --prefix openssl)
LIBS = -lpthread -lcrypto -L$(OPENSSL_PREFIX)/lib

# crypto: src/crypto.c
# 	gcc src/crypto.c -o crypto -Ilibrary -I$(OPENSSL_PREFIX)/include $(LIBS)


main: src/main.c library/mongoose.c src/crypto.c
	gcc src/main.c library/mongoose.c src/crypto.c -o main -Ilibrary -I$(OPENSSL_PREFIX)/include $(LIBS)

crypto_test: src/main.c src/crypto.c src/crypto.h
	cc \
		src/main.c \
		src/crypto.c \
		-o crypto_test \
		-Isrc \
		-I$(OPENSSL_PREFIX)/include \
		-L$(OPENSSL_PREFIX)/lib \
		-lcrypto


# CC = gcc
# CFLAGS = -Wall -Wextra -g -Ilibrary
# LIBS = -lpthread -lcrypto

# # Deteksi otomatis lokasi OpenSSL dari Homebrew
# OPENSSL_PREFIX = $(shell brew --prefix openssl)
# CFLAGS += -I$(OPENSSL_PREFIX)/include
# LIBS += -L$(OPENSSL_PREFIX)/lib

# SRC = src/main.c src/server.c src/room.c src/crypto.c src/packet.c library/mongoose.c

# main: $(SRC)
# 	$(CC) $(CFLAGS) $(SRC) -o main $(LIBS)

# clean:
# 	rm -f main