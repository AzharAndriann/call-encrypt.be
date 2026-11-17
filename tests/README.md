# Tests

Folder ini berisi tes untuk modul backend yang dapat diuji secara terpisah.

## Tes room

Menguji create, find, join, penolakan client ketiga, dan remove room.

```bash
make -f makefile.mk test_room
./test_room
```

## Tes crypto

Menguji encrypt/decrypt serta memastikan ciphertext yang diubah ditolak.

```bash
make -f makefile.mk crypto_test
./crypto_test
```

## Menjalankan semua tes

```bash
make -f makefile.mk tests
./test_room
./crypto_test
```

Tes packet belum dibuat karena format packet belum diimplementasikan.
