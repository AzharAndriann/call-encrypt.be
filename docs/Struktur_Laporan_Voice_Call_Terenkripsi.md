# Struktur Laporan Proyek Voice Call Terenkripsi

Dokumen ini merupakan kerangka utama laporan proyek **voice call terenkripsi berbasis WebSocket dalam bahasa C**. Struktur ini diadaptasi dari contoh tugas enkripsi/dekripsi file, tetapi objek yang dilindungi diubah dari file menjadi potongan data suara (*audio chunk*) yang dikirim secara real-time.

Dokumen ini mencakup keseluruhan target proyek, termasuk bagian yang sudah selesai dan bagian yang masih harus diimplementasikan. Isi laporan final harus disesuaikan kembali dengan hasil program dan bukti pengujian terakhir.

> Catatan akademik: contoh tugas awal melarang OpenSSL dan mewajibkan enkripsi file dalam dua versi. Proyek ini menggunakan AES-256-GCM melalui OpenSSL dan berfokus pada voice call. Pastikan perubahan topik dan penggunaan library kriptografi telah disetujui dosen.

---

## A. Identitas Tugas

| Data | Isi |
|---|---|
| Universitas | Universitas Gunadarma |
| Program Studi | Informatika |
| Mata Kuliah | Algoritma dan Pemrograman 3 |
| Judul | Implementasi Voice Call Terenkripsi Menggunakan AES-256-GCM dan WebSocket dalam Bahasa C |
| Kelas | `[Isi kelas]` |
| Dosen | `[Isi nama dosen]` |
| Kelompok | `[Isi nomor/nama kelompok]` |
| Anggota | `[Isi seluruh anggota dan NPM]` |
| Tanggal | `[Isi tanggal]` |

---

## B. Penyesuaian dari Contoh Enkripsi File

Laporan perlu menjelaskan bahwa prinsip pemrosesan byte dan pointer tetap digunakan, tetapi sumber dan tujuan datanya berbeda.

| Contoh enkripsi file | Proyek voice call |
|---|---|
| File dibaca dengan `FILE *` | Suara diambil dari microphone browser |
| File diproses per blok | Suara diproses per audio chunk |
| Blok dienkripsi | Audio chunk dienkripsi |
| Ciphertext ditulis ke file | Ciphertext dikirim melalui WebSocket binary |
| File didekripsi hingga identik | Audio chunk didekripsi lalu diputar |
| Uji file `.txt/.pdf/.docx` | Uji teks, byte binary, dan audio real-time |
| Kesalahan kunci merusak hasil file | Kesalahan key/tag menyebabkan chunk ditolak |

Kalimat yang dapat digunakan:

> Jika enkripsi file melindungi byte yang tersimpan, proyek ini melindungi byte yang sedang dikirim. Data suara dibagi menjadi potongan kecil, dienkripsi, dibungkus dalam packet, diteruskan oleh server, kemudian didekripsi dan diputar oleh penerima.

---

## C. Latar Belakang

Bagian ini minimal menjelaskan:

1. Voice call mengirim data suara melalui jaringan dan dapat disadap jika dikirim sebagai data mentah.
2. Enkripsi mengubah audio plaintext menjadi ciphertext sebelum dikirim.
3. AES-GCM dipilih karena menyediakan kerahasiaan sekaligus autentikasi data.
4. WebSocket digunakan untuk komunikasi dua arah secara real-time.
5. Backend C bertindak sebagai pengelola room dan relay binary.
6. Pointer digunakan untuk mengakses buffer, koneksi, room, ciphertext, serta bagian packet.
7. `malloc()` dan `free()` digunakan ketika ukuran packet baru diketahui saat runtime.

Ancaman yang dibahas:

- penyadapan data suara di perjalanan;
- perubahan ciphertext selama transmisi;
- client asing masuk ke room yang salah;
- packet rusak atau terlalu pendek;
- memory leak ketika alokasi packet tidak dibebaskan.

---

## D. Tujuan Proyek

Tuliskan tujuan berikut:

1. Membuat server WebSocket menggunakan bahasa C dan Mongoose.
2. Membuat sistem room berisi maksimal dua client.
3. Meneruskan data binary hanya kepada peer pada room yang sama.
4. Mengimplementasikan AES-256-GCM untuk encrypt dan decrypt.
5. Membuat format packet berisi sequence, nonce, ciphertext, dan tag.
6. Menggunakan pointer serta alokasi memori dinamis secara benar.
7. Mendeteksi ciphertext yang berubah melalui tag autentikasi.
8. Mengambil suara microphone, mengenkripsinya, mengirimkannya, lalu memutarnya di sisi penerima.
9. Membuktikan program tidak mengalami memory leak pada pengujian yang dilakukan.

---

## E. Ruang Lingkup dan Batasan

### Ruang lingkup target akhir

- Satu room berisi maksimal dua client.
- Control message menggunakan WebSocket text berformat JSON.
- Data terenkripsi dikirim sebagai WebSocket binary.
- AES menggunakan key 256 bit.
- Nonce setiap audio chunk berukuran 12 byte.
- Tag autentikasi berukuran 16 byte.
- Backend tidak perlu membuka isi audio terenkripsi.
- Frontend menangani microphone, enkripsi, decrypt, dan playback.

### Batasan versi tugas

- Room masih disimpan di memori, bukan database.
- Room hilang ketika server dimatikan.
- Kode room masih dibuat berurutan.
- Key statis hanya boleh digunakan untuk demonstrasi.
- Key exchange ECDH menjadi pengembangan lanjutan jika belum diimplementasikan.
- Belum ditujukan sebagai aplikasi production.
- Reconnect otomatis, autentikasi akun, dan kompresi Opus dapat dijadikan fitur opsional.

---

## F. Dasar Teori

### 1. Audio digital

Microphone menghasilkan aliran sample audio. Sample dibagi menjadi chunk kecil agar dapat dikirim tanpa menunggu rekaman selesai.

```text
Microphone
→ sample audio
→ audio chunk
→ byte array
```

Untuk implementasi awal, laporan dapat membahas PCM mono. Jika menggunakan format atau codec lain, tuliskan format, sample rate, jumlah channel, dan durasi chunk.

### 2. WebSocket

WebSocket menyediakan koneksi dua arah yang tetap terbuka. Proyek menggunakan:

- text frame untuk `create_room`, `join_room`, `peer_joined`, `peer_left`, dan `error`;
- binary frame untuk packet audio terenkripsi.

### 3. AES-256-GCM

AES-256-GCM membutuhkan:

| Komponen | Ukuran | Fungsi |
|---|---:|---|
| Key | 32 byte | Rahasia untuk encrypt dan decrypt |
| Nonce | 12 byte | Nilai unik untuk setiap proses enkripsi |
| Plaintext | Dinamis | Data teks/audio sebelum dienkripsi |
| Ciphertext | Dinamis | Data setelah dienkripsi |
| Tag | 16 byte | Mendeteksi perubahan data atau key yang salah |

Hubungan konseptual:

```text
(ciphertext, tag) = Encrypt(key, nonce, plaintext)
plaintext = Decrypt(key, nonce, ciphertext, tag)
```

Jika tag tidak valid, decrypt harus gagal dan buffer hasilnya tidak boleh digunakan.

### 4. Nonce

Nonce bukan password, tetapi harus unik untuk setiap chunk yang dienkripsi dengan key yang sama. Nonce yang digunakan kembali pada AES-GCM dapat merusak keamanan.

### 5. Packet

Format packet proyek:

```text
┌────────────┬────────────┬────────────────────┬────────────┐
│ sequence   │ nonce      │ ciphertext         │ tag        │
│ 4 byte     │ 12 byte    │ ukuran dinamis     │ 16 byte    │
└────────────┴────────────┴────────────────────┴────────────┘
```

`sequence` menunjukkan urutan chunk. Total overhead packet adalah 32 byte di luar ciphertext.

### 6. Pointer dan memori dinamis

Pointer digunakan untuk:

- menunjuk object `Room`;
- menelusuri linked list koneksi Mongoose;
- menerima plaintext, key, nonce, tag, dan ciphertext;
- mengembalikan panjang packet melalui call by reference;
- menunjuk bagian nonce, ciphertext, dan tag tanpa menyalin seluruh packet;
- menyimpan hasil `malloc()` sampai dilepaskan dengan `free()`.

---

## G. Arsitektur Sistem

### Komponen

```text
Frontend User A
  ├─ microphone
  ├─ audio chunk
  ├─ encrypt + packet
  └─ WebSocket
          │
          ▼
Backend C + Mongoose
  ├─ room management
  ├─ control message
  └─ binary relay
          │
          ▼
Frontend User B
  ├─ parse packet
  ├─ decrypt
  └─ audio playback
```

### Prinsip end-to-end

Backend hanya meneruskan packet binary. Encrypt dilakukan sebelum data dikirim dari frontend, sedangkan decrypt dilakukan setelah packet diterima frontend lawan.

Jika server masih dapat membaca plaintext audio, sistem belum dapat disebut end-to-end encrypted.

---

## H. Struktur Folder Target Akhir

```text
call-encrypt.be/
├── docs/
│   ├── api-doc.md
│   ├── Struktur_Laporan_Voice_Call_Terenkripsi.md
│   ├── Laporan_Final.md
│   └── prompt-ai.md
├── frontend/
│   ├── index.html
│   ├── client.js
│   ├── crypto-client.js
│   └── audio-client.js
├── library/
│   ├── mongoose.c
│   └── mongoose.h
├── screenshots/
├── src/
│   ├── main.c
│   ├── server.c
│   ├── server.h
│   ├── room.c
│   ├── room.h
│   ├── crypto.c
│   ├── crypto.h
│   ├── packet.c
│   └── packet.h
├── tests/
│   ├── test_room.c
│   ├── test_crypto.c
│   ├── test_packet.c
│   ├── test_crypto_packet.c
│   └── browser/
├── makefile.mk
└── README.md
```

Folder `frontend/` dapat diganti dengan nama folder frontend milik anggota kelompok, tetapi laporan harus menyebutkan lokasi kode yang mengambil dan memutar audio.

---

## I. Penjelasan Modul Program

### 1. `main.c`

Titik masuk program dan pemanggil `start_server()`.

### 2. `server.c` dan `server.h`

Tanggung jawab:

- menjalankan server pada port 8000;
- upgrade HTTP menjadi WebSocket;
- membaca request JSON;
- menangani create dan join room;
- mengirim `peer_joined` dan `peer_left`;
- mencari koneksi berdasarkan client ID;
- meneruskan binary kepada peer;
- menangani disconnect.

### 3. `room.c` dan `room.h`

Tanggung jawab:

- menyimpan maksimal `MAX_ROOMS`;
- membuat kode room;
- mencari room berdasarkan kode;
- mencari room berdasarkan client ID;
- memasukkan client kedua;
- menolak client ketiga;
- menghapus room menggunakan `memset()`.

### 4. `crypto.c` dan `crypto.h`

Tanggung jawab:

- AES-256-GCM encrypt;
- menghasilkan ciphertext dan tag;
- AES-256-GCM decrypt;
- menolak tag yang tidak valid;
- membersihkan context OpenSSL pada semua jalur keluar.

Modul C ini membuktikan logika crypto secara lokal. Pada browser, fungsi setara perlu dibuat menggunakan Web Crypto API.

### 5. `packet.c` dan `packet.h`

Tanggung jawab:

- menghitung ukuran packet;
- mengalokasikan packet menggunakan `malloc()`;
- menyusun sequence, nonce, ciphertext, dan tag;
- membongkar packet menggunakan pointer;
- menolak packet yang terlalu pendek;
- mewajibkan pemanggil melakukan `free(packet)`.

### 6. Frontend

Tanggung jawab target akhir:

- membuka WebSocket;
- membuat atau join room;
- memakai shared demo key atau hasil key exchange;
- mengambil microphone;
- membentuk audio chunk;
- menghasilkan nonce unik;
- encrypt audio chunk;
- membentuk packet yang kompatibel dengan `packet.c`;
- mengirim binary;
- parse dan decrypt packet masuk;
- mengurutkan chunk berdasarkan sequence;
- memutar audio;
- menangani `peer_joined`, `peer_left`, dan error.

---

## J. Flowchart yang Harus Dimasukkan

Laporan final minimal memiliki empat flowchart.

### 1. Flow create dan join room

```text
Mulai
→ Connect WebSocket
→ Create room atau join room?
→ Validasi request
→ Simpan client ID
→ Room berisi dua client?
→ Kirim status
→ Selesai
```

### 2. Flow pengirim audio

```text
Microphone
→ Ambil audio chunk
→ Ubah menjadi byte
→ Buat nonce unik
→ Encrypt AES-GCM
→ Bentuk packet
→ Kirim WebSocket binary
→ Ulangi selama panggilan aktif
```

### 3. Flow penerima audio

```text
Terima binary
→ Validasi panjang packet
→ Parse sequence/nonce/ciphertext/tag
→ Verifikasi dan decrypt
→ Tag valid?
   ├─ Tidak: buang chunk
   └─ Ya: masukkan ke playback buffer
→ Putar audio
```

### 4. Flow disconnect

```text
Koneksi ditutup
→ Cari room berdasarkan client ID
→ Cari peer
→ Kirim peer_left
→ Hapus data room
→ Selesai
```

Flowchart final sebaiknya digambar menggunakan draw.io, Canva, atau aplikasi diagram lain dengan simbol terminator, process, decision, input/output, dan panah.

---

## K. Pseudocode yang Harus Dimasukkan

### Create room

```text
FUNCTION create_room(client_id)
    FOR setiap slot room
        IF slot tidak aktif
            kosongkan room
            buat kode room
            simpan client_id sebagai client pertama
            aktifkan room
            RETURN alamat room
        END IF
    END FOR
    RETURN NULL
END FUNCTION
```

### Join room

```text
FUNCTION join_room(code, client_id)
    room = cari room berdasarkan code
    IF room tidak ditemukan
        RETURN NULL
    END IF
    IF client adalah pembuat OR client kedua sudah terisi
        RETURN NULL
    END IF
    simpan client_id sebagai client kedua
    RETURN room
END FUNCTION
```

### Packet create

```text
FUNCTION packet_create(sequence, nonce, ciphertext, tag)
    hitung total ukuran
    alokasikan packet dengan malloc
    IF alokasi gagal
        RETURN NULL
    END IF
    tulis sequence
    salin nonce
    salin ciphertext
    salin tag
    ubah packet_len melalui pointer
    RETURN packet
END FUNCTION
```

### Packet parse

```text
FUNCTION packet_parse(packet)
    IF panjang kurang dari ukuran minimum
        RETURN gagal
    END IF
    arahkan pointer sequence
    arahkan pointer nonce
    arahkan pointer ciphertext
    arahkan pointer tag
    RETURN berhasil
END FUNCTION
```

### Binary relay

```text
FUNCTION relay_binary(connection, data)
    cari room berdasarkan connection ID
    IF tidak ada room
        kirim error
        RETURN
    END IF
    tentukan peer ID
    cari koneksi peer
    IF peer tidak tersedia
        kirim error
        RETURN
    END IF
    kirim data binary ke peer
END FUNCTION
```

### Enkripsi audio frontend

```text
FOR setiap audio chunk
    ubah sample menjadi byte
    buat nonce unik
    encrypt byte menggunakan AES-GCM
    bentuk packet dengan sequence
    kirim packet melalui WebSocket
    sequence = sequence + 1
END FOR
```

---

## L. Tabel Penggunaan Pointer

Masukkan tabel berikut dan lengkapi nomor baris setelah kode final.

| Pointer | File/fungsi | Kegunaan |
|---|---|---|
| `Room *room` | `room.c`, `server.c` | Menunjuk room tanpa menyalin seluruh struct |
| `struct mg_connection *c` | `server.c` | Menunjuk koneksi client aktif |
| `struct mg_connection *peer` | `server.c` | Menunjuk koneksi lawan bicara |
| `void *ev_data` | `server.c` | Data event generik dari Mongoose |
| `const unsigned char *plaintext` | `crypto.c` | Membaca buffer plaintext |
| `unsigned char *ciphertext` | `crypto.c` | Menulis hasil enkripsi |
| `const unsigned char *key` | `crypto.c` | Membaca key AES |
| `const unsigned char *nonce` | `crypto.c`, `packet.c` | Membaca nonce |
| `unsigned char *tag` | `crypto.c` | Menulis tag autentikasi |
| `unsigned char *packet` | `packet.c` | Menyimpan hasil alokasi dinamis |
| `size_t *packet_len` | `packet.c` | Call by reference untuk mengembalikan ukuran |
| `const unsigned char **ciphertext` | `packet.c` | Mengembalikan alamat bagian ciphertext |
| `const unsigned char **tag` | `packet.c` | Mengembalikan alamat bagian tag |
| `EVP_CIPHER_CTX *ctx` | `crypto.c` | Context proses AES-GCM |

Jelaskan perbedaan:

- `T *` adalah pointer ke object bertipe `T`;
- `const T *` berarti data tidak diubah melalui pointer tersebut;
- `T **` dipakai untuk mengubah nilai pointer milik pemanggil;
- `&variable` mengambil alamat variable;
- `pointer + offset` melakukan aritmetika pointer.

---

## M. Tabel Penggunaan Library

| Library/header | Penggunaan |
|---|---|
| `stdio.h` | Output terminal dan `snprintf()` |
| `stdlib.h` | `malloc()` dan `free()` |
| `string.h` | `memcpy()`, `memset()`, `strcmp()`, `strlen()` |
| `stdint.h` | `uint32_t` untuk sequence |
| `stddef.h` | `size_t` |
| OpenSSL EVP | AES-256-GCM dan context cipher |
| Mongoose | HTTP, WebSocket, event loop, JSON helper |
| Web Crypto API | AES-GCM di browser |
| Web Audio API | Pengambilan dan playback audio |

---

## N. API WebSocket yang Dilaporkan

### Text request

```json
{"type":"create_room"}
```

```json
{"type":"join_room","code":"000001"}
```

### Text response

```json
{"type":"created","code":"000001"}
```

```json
{"type":"joined","code":"000001"}
```

```json
{"type":"peer_joined","code":"000001"}
```

```json
{"type":"peer_left","code":"000001"}
```

```json
{"type":"error","message":"..."}
```

### Binary message

Binary message berisi packet terenkripsi, bukan JSON dan bukan audio plaintext.

---

## O. Pengujian yang Wajib Dilaporkan

### Pengujian unit dan integrasi C

| No | Pengujian | Bukti yang diharapkan |
|---:|---|---|
| 1 | Create room | Room aktif dan client pertama tersimpan |
| 2 | Find room | Kode valid ditemukan, kode salah ditolak |
| 3 | Join room | Client kedua tersimpan |
| 4 | Client ketiga | Ditolak |
| 5 | Remove room | Seluruh field kembali nol |
| 6 | Encrypt/decrypt | `halo` kembali menjadi `halo` |
| 7 | Ciphertext diubah | Decrypt menghasilkan nilai negatif |
| 8 | Packet create/parse | Semua bagian identik setelah parse |
| 9 | Packet terlalu pendek | Ditolak tanpa crash |
| 10 | Crypto + packet | Encrypt, pack, parse, decrypt berhasil |
| 11 | Memory leak | `0 leaks for 0 total leaked bytes` |

### Pengujian WebSocket

| No | Pengujian | Bukti yang diharapkan |
|---:|---|---|
| 1 | Koneksi | Client menerima event open |
| 2 | Create | Menerima `created` |
| 3 | Join | Menerima `joined` |
| 4 | Peer joined | Creator menerima `peer_joined` |
| 5 | Binary A ke B | Byte yang diterima sama |
| 6 | Binary B ke A | Byte yang diterima sama |
| 7 | Disconnect | Peer menerima `peer_left` |
| 8 | Binary tanpa room | Mendapat error |
| 9 | Dua room bersamaan | Data tidak tertukar |

### Pengujian frontend crypto dan audio

| No | Pengujian | Bukti yang diharapkan |
|---:|---|---|
| 1 | Teks terenkripsi | A mengirim `halo`, B decrypt menjadi `halo` |
| 2 | Packet browser rusak | Decrypt ditolak |
| 3 | Microphone permission | Stream microphone berhasil diperoleh |
| 4 | Audio chunk | Ukuran dan sequence tercatat |
| 5 | Audio A ke B | B dapat mendengar A |
| 6 | Audio B ke A | A dapat mendengar B |
| 7 | Tag salah | Chunk dibuang, aplikasi tidak crash |
| 8 | Call selesai | Stream dihentikan dan room dibersihkan |

Jangan menandai pengujian sebagai berhasil sebelum benar-benar dilakukan.

---

## P. Perintah Build dan Pengujian

```bash
make -f makefile.mk main
make -f makefile.mk test_room
make -f makefile.mk test_packet
make -f makefile.mk crypto_test
make -f makefile.mk crypto_packet_test
```

```bash
./test_room
./test_packet
./crypto_test
./crypto_packet_test
```

Memory leak di macOS:

```bash
leaks --atExit -- ./test_packet
leaks --atExit -- ./crypto_packet_test
```

Memory leak di Windows melalui WSL:

```bash
valgrind --leak-check=full ./test_packet
valgrind --leak-check=full ./crypto_packet_test
```

---

## Q. Screenshot yang Harus Dikumpulkan

Simpan gambar dengan nama yang jelas:

```text
screenshots/
├── 01-build-server.png
├── 02-test-room.png
├── 03-test-crypto.png
├── 04-test-packet.png
├── 05-test-crypto-packet.png
├── 06-memory-leak-zero.png
├── 07-create-room.png
├── 08-join-room.png
├── 09-peer-joined.png
├── 10-binary-relay.png
├── 11-peer-left.png
├── 12-browser-encrypted-text.png
├── 13-microphone-permission.png
├── 14-audio-call-a-to-b.png
├── 15-audio-call-b-to-a.png
└── 16-ciphertext-log.png
```

Setiap screenshot di laporan diberi nomor, judul, dan penjelasan singkat mengenai apa yang dibuktikan.

---

## R. Log Ciphertext Real-Time

Jika log ciphertext dimasukkan sebagai fitur, tampilkan hanya informasi yang aman dan relevan:

```text
room=000001
sequence=15
packet_size=9632
ciphertext_preview=a84f31...
tag_valid=true
```

Jangan mencetak key AES atau private key ke log.

---

## S. Hasil dan Pembahasan

Bagian ini membahas:

1. Apakah create/join room berhasil.
2. Apakah binary relay bekerja dua arah.
3. Apakah packet dapat dibuat dan dibongkar.
4. Apakah ciphertext yang berubah ditolak.
5. Apakah memory leak ditemukan.
6. Apakah browser dapat decrypt packet dari peer.
7. Apakah audio dapat didengar dengan latency yang dapat diterima.
8. Masalah yang ditemukan dan cara memperbaikinya.

Jika memungkinkan, catat:

- ukuran rata-rata audio chunk;
- packet per detik;
- latency perkiraan;
- packet yang gagal decrypt;
- penggunaan memory selama tes.

---

## T. Keamanan dan Keterbatasan

Laporan wajib jujur mengenai hal berikut:

1. Static demo key tidak aman untuk production.
2. Production sebaiknya menggunakan ECDH key exchange.
3. Nonce tidak boleh digunakan ulang dengan key yang sama.
4. Backend hanya dapat disebut relay end-to-end jika tidak menerima plaintext atau key.
5. HTTPS/WSS wajib digunakan pada deployment publik.
6. Kode room berurutan mudah ditebak dan perlu dibuat acak pada versi production.
7. Tidak ada autentikasi identitas user.
8. Room disimpan di RAM dan hilang saat restart.
9. Raw PCM menggunakan bandwidth lebih besar daripada Opus.
10. Packet yang gagal autentikasi harus dibuang.

---

## U. Dokumentasi Penggunaan AI

Buat tabel untuk seluruh bantuan AI yang digunakan.

| Tanggal | Alat AI | Prompt/pertanyaan | Bagian yang dibantu | Modifikasi kelompok |
|---|---|---|---|---|
| `[tanggal]` | ChatGPT/Codex | `[prompt lengkap]` | Crypto, room, packet, laporan, dll. | `[jelaskan perubahan]` |

Tambahkan refleksi:

- AI membantu menjelaskan konsep dan menemukan kesalahan sintaks.
- Seluruh kode diuji dan dipahami kembali oleh kelompok.
- Kelompok tetap bertanggung jawab terhadap hasil akhir.
- Kode tidak dianggap benar sebelum berhasil dikompilasi dan diuji.

Riwayat percakapan yang relevan dapat dirangkum atau dilampirkan sesuai instruksi dosen.

---

## V. Pembagian Tugas Kelompok

| Anggota | Tanggung jawab | Bukti kontribusi |
|---|---|---|
| Anggota 1 | Server dan WebSocket | Commit, screenshot, penjelasan |
| Anggota 2 | Room management | Unit test dan listing |
| Anggota 3 | Crypto AES-GCM | Test crypto dan tag |
| Anggota 4 | Packet dan memory | Test packet dan leak report |
| Anggota 5 | Frontend audio dan dokumentasi | Demo audio, UI, laporan |

Sesuaikan pembagian dengan kontribusi sebenarnya.

---

## W. Kesimpulan

Kesimpulan final menjawab:

1. Apakah sistem berhasil menghubungkan dua client dalam satu room.
2. Apakah binary dapat direlay tanpa tercampur dengan room lain.
3. Apakah AES-GCM berhasil menjaga kerahasiaan dan integritas.
4. Apakah packet berhasil membawa metadata yang dibutuhkan penerima.
5. Apakah penggunaan pointer dan memori dinamis sudah benar.
6. Apakah voice call terenkripsi berhasil dua arah.
7. Apa yang masih harus dikembangkan untuk production.

Jangan menulis bahwa voice call berhasil jika pengujian audio belum dilakukan.

---

## X. Daftar Pustaka

Minimal mencantumkan sumber resmi:

- Dokumentasi OpenSSL EVP AES-GCM.
- Dokumentasi Mongoose WebSocket.
- Dokumentasi MDN WebSocket API.
- Dokumentasi MDN Web Crypto API.
- Dokumentasi MDN Web Audio API atau AudioWorklet.
- Buku atau materi perkuliahan bahasa C dan pointer.

Gunakan satu gaya sitasi secara konsisten.

---

## Y. Lampiran

Lampiran laporan final:

1. Listing lengkap source code.
2. Makefile.
3. Dokumentasi API.
4. Hasil seluruh unit test.
5. Laporan memory leak.
6. Screenshot pengujian.
7. Prompt AI.
8. Link repository dan petunjuk menjalankan program.

---

## Z. Status Implementasi Proyek

Gunakan tabel ini sebagai checklist. Perbarui status sebelum laporan dikumpulkan.

| Bagian | Status saat dokumen dibuat | Wajib sebelum final? |
|---|---|---:|
| Server WebSocket | Selesai | Ya |
| Create room | Selesai | Ya |
| Join room | Selesai | Ya |
| Penolakan client ketiga | Selesai | Ya |
| `peer_joined` | Selesai | Ya |
| Binary relay dua arah | Selesai | Ya |
| Pencarian room berdasarkan client | Selesai | Ya |
| Disconnect dan `peer_left` | Perlu pemeriksaan struktur event | Ya |
| AES-256-GCM C | Selesai | Ya |
| Penolakan ciphertext rusak | Selesai | Ya |
| Packet create/parse | Selesai | Ya |
| `malloc/free` packet | Selesai | Ya |
| Memory leak test | Selesai, 0 leak | Ya |
| Crypto + packet integration | Selesai | Ya |
| Encrypted text antar-browser | Belum | Ya |
| Shared demo key frontend | Belum | Ya |
| Nonce unik per message | Belum | Ya |
| Microphone capture | Belum | Ya |
| Audio chunk | Belum | Ya |
| Encrypt audio frontend | Belum | Ya |
| Decrypt audio frontend | Belum | Ya |
| Playback audio | Belum | Ya |
| Voice call dua arah | Belum | Ya |
| Log ciphertext | Belum/opsional | Tidak |
| ECDH key exchange | Belum/opsional untuk tugas | Tidak |
| Multi-room test | Belum | Disarankan |
| Dokumentasi API final | Perlu diperbarui | Ya |
| Screenshot final | Belum lengkap | Ya |
| Prompt AI | Belum dirangkum | Ya |

---

## Koreksi Teknis Sebelum Laporan Final

1. Pastikan `MG_EV_CLOSE` berada sejajar dengan `MG_EV_WS_MSG`, bukan di dalam pengecekan opcode.
2. Tambahkan `crypto_packet_test` ke `.PHONY`, target `tests`, dan target `clean` pada Makefile.
3. Tambahkan `packet.c` ke target aplikasi hanya jika backend memang memakainya.
4. Gunakan `wss://` untuk URL ngrok, bukan `ws://`.
5. Jangan gunakan nonce tetap di aplikasi nyata; nonce tetap hanya untuk unit test.
6. Jangan mencetak atau mengirim key melalui log biasa.
7. Hapus atau rapikan file duplikat seperti `src/crypto copy.c` sebelum pengumpulan.
8. Jangan memasukkan binary hasil build dan folder `.dSYM` ke ZIP source kecuali diminta.
9. Pastikan seluruh target dapat dibangun dari clone baru.
10. Jalankan seluruh tes sekali lagi sebelum mengambil screenshot final.

---

## Struktur Berkas Pengumpulan yang Disarankan

```text
Kelompok_[Kelas]_[NamaKelompok]/
├── Laporan_Voice_Call_Terenkripsi.docx
├── Laporan_Voice_Call_Terenkripsi.pdf
├── source/
│   ├── src/
│   ├── frontend/
│   ├── library/
│   ├── tests/
│   └── makefile.mk
├── screenshot/
├── dokumentasi/
│   ├── api-doc.md
│   ├── prompt-ai.md
│   └── README.md
└── video-demo/ atau link-demo.txt
```

Laporan final sebaiknya fokus pada voice call terenkripsi dan tidak mengklaim sebagai program enkripsi file. Prinsip pointer, buffer, packet, enkripsi, autentikasi, serta pengujian memori tetap dijelaskan dengan kedalaman yang setara dengan contoh tugas.
