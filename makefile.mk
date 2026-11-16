CC := cc
OPENSSL_PREFIX := $(shell brew --prefix openssl)

CFLAGS := -Wall -Wextra -g
COMMON_INCLUDES := -Isrc -Ilibrary
OPENSSL_INCLUDES := -I$(OPENSSL_PREFIX)/include
OPENSSL_LIBS := -L$(OPENSSL_PREFIX)/lib -lcrypto -lpthread

.PHONY: all main main_input tests test_room crypto_test clean

# ============================================================
# Default
# ============================================================

all: main

# ============================================================
# Application / server
# ============================================================

main: src/main.c library/mongoose.c src/crypto.c src/crypto.h src/room.c src/room.h
	$(CC) $(CFLAGS) \
		src/main.c \
		library/mongoose.c \
		src/crypto.c \
		src/room.c \
		-o main \
		$(COMMON_INCLUDES) \
		$(OPENSSL_INCLUDES) \
		$(OPENSSL_LIBS)

# ============================================================
# Manual input test from src/main.c (room module only)
# Run: make -f makefile.mk main_input && ./build/main_input
# ============================================================

main_input: src/main.c src/room.c src/room.h
	mkdir -p build
	$(CC) $(CFLAGS) \
		src/main.c \
		src/room.c \
		-o build/main_input \
		$(COMMON_INCLUDES)

# ============================================================
# Unit tests
# ============================================================

tests: test_room crypto_test

test_room: tests/test_room.c src/room.c src/room.h
	$(CC) $(CFLAGS) \
		tests/test_room.c \
		src/room.c \
		-o test_room \
		$(COMMON_INCLUDES)

crypto_test: src/main.c src/crypto.c src/crypto.h
	$(CC) $(CFLAGS) \
		src/main.c \
		src/crypto.c \
		-o crypto_test \
		$(COMMON_INCLUDES) \
		$(OPENSSL_INCLUDES) \
		$(OPENSSL_LIBS)

# ============================================================
# Cleanup (only runs when explicitly requested)
# ============================================================

clean:
	rm -f main test_room crypto_test
	rm -f build/main_input

# ============================================================
# Previous standalone crypto command (kept as reference)
# ============================================================

# crypto: src/crypto.c
# 	$(CC) src/crypto.c -o crypto \
# 		$(COMMON_INCLUDES) $(OPENSSL_INCLUDES) $(OPENSSL_LIBS)
