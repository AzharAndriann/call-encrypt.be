**UNIVERSITAS GUNADARMA**

**PROGRAM STUDI INFORMATIKA**

Mata Kuliah Algoritma dan Pemrograman 3

**LEMBAR TUGAS KELOMPOK**

*Program Enkripsi dan Dekripsi File Berbasis Pointer dalam Bahasa C*

| **Mata Kuliah**       | Algoritma dan Pemrograman 3                                                        |
|-----------------------|------------------------------------------------------------------------------------|
| **Kelas**             | ..............................................                                     |
| **Dosen Pengampu**    | ..............................................                                     |
| **Sifat Tugas**       | Kelompok, beranggotakan **5 (lima) orang**                                         |
| **Tanggal Diberikan** | ..............................................                                     |
| **Batas Pengumpulan** | **7 (tujuh) hari kalender** setelah tugas diberikan, paling lambat pukul 23.59 WIB |
| **Media Pengumpulan** | Google Drive                                                                       |

# A. Latar Belakang

Enkripsi adalah proses mengubah data asli (*plaintext*) menjadi data yang tidak dapat dibaca (*ciphertext*) dengan menggunakan kunci tertentu, sedangkan dekripsi adalah proses sebaliknya. Pada tingkat komputer, setiap file, baik teks, PDF, maupun dokumen Word, tersimpan sebagai deretan byte. Oleh karena itu, enkripsi file pada dasarnya merupakan manipulasi byte demi byte di dalam memori.

Dalam bahasa C, manipulasi tersebut dilakukan secara efisien menggunakan **pointer**: file diakses melalui pointer FILE \*, data dibaca ke dalam buffer, kemudian setiap byte pada buffer diubah langsung melalui alamat memorinya. Tugas ini dirancang agar mahasiswa memahami peran pointer secara nyata, sekaligus memahami bahwa fungsi library pada dasarnya hanyalah kumpulan perulangan dan manipulasi pointer yang telah dikemas.

# B. Tujuan Pembelajaran

Setelah menyelesaikan tugas ini, mahasiswa diharapkan mampu:

1\. menjelaskan tujuan dan prinsip dasar enkripsi serta dekripsi data;

2\. merumuskan model matematika algoritma enkripsi dan membuktikan bahwa dekripsi mengembalikan data asli;

3\. menyusun flowchart dan pseudocode yang konsisten dengan model matematika;

4\. mengimplementasikan program C yang memproses file biner menggunakan pointer (FILE \*, aritmetika pointer, dan call by reference);

5\. membuat fungsi pengganti library (seperti strlen dan memset) secara mandiri menggunakan pointer, lalu membandingkannya dengan fungsi library standar;

6\. mendokumentasikan penggunaan alat bantu kecerdasan artifisial (AI) secara jujur dan bertanggung jawab.

# C. Deskripsi Tugas

Setiap kelompok membuat **program enkripsi dan dekripsi file** dalam bahasa C dengan algoritma yang ditentukan pada Bagian D. Program dibuat dalam **dua versi** dengan logika algoritma yang sama, yaitu:

- **Versi A**, program **tanpa** fungsi library pendukung; dan

- **Versi B**, program **dengan** fungsi library yang dipersyaratkan.

Kedua versi wajib menggunakan pointer dan wajib mampu mengenkripsi serta mendekripsi file teks (.txt), PDF (.pdf), dan Word (.docx). File hasil dekripsi harus **identik byte demi byte** dengan file asli.

# D. Referensi Algoritma

| No | Algoritma | Enkripsi | Dekripsi | Keterangan |
|----|-----------|----------|----------|------------|
| 1 | Caesar Byte | Cᵢ = (Pᵢ + k) mod 256 | Pᵢ = (Cᵢ − k + 256) mod 256 | k bilangan bulat 1–255 |
| 2 | Vigenère Byte | Cᵢ = (Pᵢ + Kₑ) mod 256, e = i mod m | Pᵢ = (Cᵢ − Kₑ + 256) mod 256 | K berupa string, m = panjang kunci |
| 3 | XOR Berulang | Cᵢ = Pᵢ ⊕ Kₑ, e = i mod m | Pᵢ = Cᵢ ⊕ Kₑ | Rumus enkripsi = rumus dekripsi |
| 4 | Affine Byte | Cᵢ = (a·Pᵢ + b) mod 256 | Pᵢ = a⁻¹·(Cᵢ − b + 256) mod 256 | a wajib ganjil; a⁻¹ dihitung program |
| 5 | Rotasi Bit + XOR | Cᵢ = ROLᵣ(Pᵢ ⊕ Kₑ) | Pᵢ = RORᵣ(Cᵢ) ⊕ Kₑ | r = 1–7 (jumlah geser bit) |
| 6 | Autokey Byte | Cᵢ = (Pᵢ + Zᵢ) mod 256; Zᵢ = Kᵢ (i < m), Pᵢ₋ₘ (i ≥ m) | Pᵢ = (Cᵢ − Zᵢ + 256) mod 256 | Dekripsi memakai plaintext yang telah dipulihkan |

Keterangan notasi: Pᵢ = byte ke-i file asli, Cᵢ = byte ke-i file terenkripsi, K = kunci, m = panjang kunci, ⊕ = operasi XOR, ROL/ROR = rotasi bit ke kiri/kanan sebanyak r posisi pada data 8 bit.

# E. Ketentuan Program (Berlaku untuk Kedua Versi)

1\. Program ditulis dalam bahasa C.

2\. File dibuka dalam **mode biner** ("rb" untuk membaca, "wb" untuk menulis) dan diproses per blok menggunakan buffer, sehingga program mampu memproses file yang lebih besar daripada ukuran buffer.

3\. Program menyediakan pilihan mode **enkripsi** atau **dekripsi**, serta meminta masukan nama file masukan, nama file keluaran, dan kunci.

4\. Program melakukan validasi minimal: mode tidak dikenal, kunci kosong atau tidak valid, dan file yang tidak dapat dibuka.

5\. File hasil enkripsi Versi A harus dapat didekripsi oleh Versi B, dan sebaliknya.

6\. Dilarang menggunakan library kriptografi (seperti OpenSSL atau libsodium). Algoritma harus ditulis sendiri.

7\. Setiap baris penting pada program diberi komentar yang menjelaskan maksudnya.

## Penggunaan Pointer yang Diwajibkan

Program wajib memuat seluruh penggunaan pointer berikut:

a\. pointer file (FILE \*) untuk file masukan dan keluaran;

b\. penelusuran buffer pada proses inti enkripsi/dekripsi menggunakan **aritmetika pointer** (misalnya \*p dan p++), **bukan** indeks array buffer[i];

c\. pengiriman buffer dan kunci ke fungsi melalui parameter pointer;

d\. minimal satu variabel yang diubah melalui **call by reference** (misalnya penghitung posisi byte atau jumlah byte yang diproses).

# F. Ketentuan Versi A dan Versi B

| Aspek | Versi A (Tanpa Fungsi Library Pendukung) | Versi B (Dengan Fungsi Library yang Dipersyaratkan) |
|-------|------------------------------------------|------------------------------------------------------|
| **Header yang diizinkan** | Hanya `stdio.h` | `stdio.h`, `stdlib.h`, `string.h`, `ctype.h` |
| **Fungsi library yang boleh dipakai** | Hanya fungsi I/O: `fopen`, `fclose`, `fread`, `fwrite`, `fgetc`, `fputc`, `printf`, `scanf`, `fgets` | Fungsi I/O, ditambah fungsi dari header di atas |
| **Fungsi wajib** | Membuat sendiri, menggunakan pointer, minimal: pengganti `strlen()`; pengganti `memset()` untuk menghapus kunci dari memori; pengganti `toupper()`/`isdigit()` sesuai kebutuhan | Wajib menggunakan minimal: `malloc()` dan `free()` untuk buffer; `strlen()`; `memset()` untuk menghapus kunci dari memori |
| **Buffer** | Array statis berukuran tetap | Buffer dinamis hasil `malloc()` |
| **Fungsi buatan sendiri** | Diperbolehkan | Diperbolehkan |

Header math.h tidak diperbolehkan pada kedua versi. Seluruh perhitungan dilakukan dengan operator dasar C (+, −, %, ^, \<\<, \>\>, &, \|).

# G. Luaran Tugas

Seluruh luaran dituliskan pada **Lembar Jawaban Mahasiswa** yang telah disediakan, dengan urutan sebagai berikut:

1\. **Tujuan enkripsi**: data apa yang dilindungi, dari ancaman apa, dan contoh skenario nyata penggunaannya.

2\. **Deskripsi program secara umum**: cara kerja, masukan, proses, keluaran, serta batasan program.

3\. **Model matematika**: notasi, rumus enkripsi dan dekripsi, pembuktian bahwa dekripsi mengembalikan data asli, serta contoh perhitungan manual minimal 8 byte.

4\. **Flowchart** program menggunakan simbol standar.

5\. **Pseudocode** program.

6\. **Listing program Versi A** (tanpa fungsi library pendukung) beserta tabel penggunaan pointer.

7\. **Listing program Versi B** (dengan fungsi library yang dipersyaratkan) beserta tabel fungsi library yang digunakan.

8\. **Screenshot luaran**, minimal: proses enkripsi, proses dekripsi, dekripsi dengan kunci salah, file asli dan hasil dekripsi yang berhasil dibuka, serta bukti file identik (perintah fc /b di Windows atau cmp di Linux).

9\. **File enkripsi dan dekripsi**: minimal 3 file uji (.txt, .pdf, .docx), masing-masing berupa file asli, file terenkripsi (.enc), dan file hasil dekripsi.

10\. **Prompt AI yang digunakan**: nama alat AI, tanggal, prompt secara utuh, bagian tugas yang dibantu, dan modifikasi yang dilakukan kelompok. Apabila tidak menggunakan AI, kelompok wajib membuat pernyataan tertulis.

# H. Format Pengumpulan

Seluruh berkas dikompresi dalam satu file **.zip** dengan nama **Kelompok_[Kelas]_[Inisial Nama Kelompok].zip** (contoh: Kelompok_2IA99_DELL.zip), dengan struktur folder berikut:

```
Kelompok_2IA99_DELL/
|-- Lembar_Jawaban_Kelompok_03.docx (dan versi .pdf)
|-- program/
|   |-- versiA_tanpa_fungsi.c
|   |-- versiB_dengan_fungsi.c
|-- file_uji/
|   |-- asli/ (dokumen.txt, dokumen.pdf, dokumen.docx)
|   |-- enkripsi/ (dokumen_txt.enc, dokumen_pdf.enc, dokumen_docx.enc)
|   |-- dekripsi/ (hasil dekripsi ketiga file)
|-- screenshot/ (file gambar .png/.jpg)
```

Pengumpulan cukup dilakukan oleh **ketua kelompok**. Kunci yang digunakan untuk file uji wajib dicantumkan pada lembar jawaban agar dosen dapat menguji ulang.

# I. Komponen Penilaian

| **No** | **Komponen**                                                | **Bobot (%)** |
|--------|-------------------------------------------------------------|---------------|
| 1      | Tujuan enkripsi dan deskripsi program                       | 10            |
| 2      | Model matematika (rumus, pembuktian, perhitungan manual)    | 10            |
| 3      | Flowchart                                                   | 10            |
| 4      | Pseudocode                                                  | 10            |
| 5      | Program Versi A (tanpa fungsi library pendukung)            | 20            |
| 6      | Program Versi B (dengan fungsi library yang dipersyaratkan) | 15            |
| 7      | Screenshot luaran serta file enkripsi dan dekripsi          | 10            |
| 8      | Dokumentasi prompt AI dan refleksi penggunaannya            | 5             |
| 9      | Kelengkapan, kerapian dokumen, dan pembagian tugas          | 10            |
|        | **Total**                                                   | **100**       |

# J. Ketentuan Akademik

1\. Keterlambatan pengumpulan dikenakan pengurangan nilai.

2\. Program yang tidak dapat dikompilasi, atau tidak menggunakan pointer sesuai Bagian E, dinilai maksimal 50% pada komponen program terkait.

3\. Penggunaan AI **diperbolehkan** sebagai alat bantu belajar, dengan syarat seluruh prompt didokumentasikan secara jujur. Penggunaan AI yang tidak dilaporkan dianggap sebagai pelanggaran integritas akademik.

4\. Pekerjaan yang terbukti sama atau sangat mirip antarkelompok diberi nilai 0 (nol) untuk seluruh kelompok yang terlibat.

5\. Dosen berhak meminta demonstrasi program dan menanyakan isi program kepada setiap anggota kelompok. Anggota yang tidak mampu menjelaskan bagian yang menjadi tanggung jawabnya dapat memperoleh nilai individu yang berbeda.

*Selamat mengerjakan. Pahami setiap baris program yang Anda tulis.*
