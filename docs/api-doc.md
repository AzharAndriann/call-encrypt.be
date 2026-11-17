# Dokumentasi WebSocket API

Dokumentasi sederhana untuk menghubungkan frontend ke backend `call-encrypt.be`.

## 1. Alamat WebSocket

### ngrok ini cuman bisa kalo laptop gw nyala yak

```text
wss://outskirts-equator-capricorn.ngrok-free.dev/websocket
```

## 2. Membuka koneksi

Jalankan kode ini di frontend atau Console browser:

```js
const ws = new WebSocket("ws://outskirts-equator-capricorn.ngrok-free.dev/websocket");

ws.onopen = () => {
  console.log("Terhubung ke server");
};

ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log("Pesan server:", data);
};

ws.onerror = (error) => {
  console.error("WebSocket error:", error);
};

ws.onclose = () => {
  console.log("Koneksi ditutup");
};
```

Tunggu tulisan `Terhubung ke server` sebelum mengirim request.

## 3. Create room

Digunakan oleh User A untuk membuat room.

Request:

```js
ws.send(JSON.stringify({
  type: "create_room"
}));
```

Respons berhasil:

```json
{
  "type": "created",
  "code": "000001"
}
```

Simpan `code` sebagai teks/string karena angka nol di depannya diperlukan.

Respons gagal:

```json
{
  "type": "error",
  "message": "gagal membuat room"
}
```

## 4. Join room

Digunakan oleh User B untuk masuk ke room milik User A.

Gunakan koneksi WebSocket berbeda untuk User B, lalu kirim:

```js
ws.send(JSON.stringify({
  type: "join_room",
  code: "000001"
}));
```

Ganti `000001` dengan kode yang diterima User A.

Respons berhasil:

```json
{
  "type": "joined",
  "code": "000001"
}
```

Jika room tidak ditemukan atau sudah berisi dua user:

```json
{
  "type": "error",
  "message": "room tidak ditemukan atau penuh"
}
```

Jika field `code` tidak dikirim:

```json
{
  "type": "error",
  "message": "code wajib diisi"
}
```

## 5. Error umum

Jika field `type` tidak dikirim:

```json
{
  "type": "error",
  "message": "type wajib diisi"
}
```

Jika nilai `type` tidak dikenali:

```json
{
  "type": "error",
  "message": "request tidak dikenal"
}
```

## 6. Alur penggunaan

```text
1. User A dan User B membuka koneksi WebSocket masing-masing.
2. User A mengirim create_room.
3. Server memberikan code kepada User A.
4. User A membagikan code tersebut kepada User B.
5. User B mengirim join_room dengan code tersebut.
6. Room penuh setelah User A dan User B masuk.
7. User ketiga yang mencoba masuk akan menerima error.
```

## Catatan

- Room masih disimpan di memori, bukan database.
- Jika backend dimatikan atau direstart, semua room akan hilang.
- Satu room saat ini hanya dapat berisi dua user.
- Fitur pengiriman audio belum dibahas dalam dokumentasi ini.
