# WebSocket API

Dokumentasi komunikasi real-time antara frontend dan backend `call-encrypt.be`.

## Status Implementasi

| Fitur | Status |
|---|---|
| Koneksi WebSocket | Tersedia |
| Create room | Tersedia |
| Join room | Belum tersedia |
| Notifikasi peer joined | Belum tersedia |
| Relay audio binary | Belum tersedia |

Frontend sebaiknya hanya menggunakan fitur yang berstatus **Tersedia**.

## Koneksi

### Local development

```text
ws://localhost:8000/websocket
```

### Public development (ngrok)

```text
wss://<NGROK_DOMAIN>/websocket
```

Contoh:

```text
wss://example.ngrok-free.dev/websocket
```

URL ngrok dapat berubah ketika tunnel dijalankan ulang. Simpan URL WebSocket pada environment variable frontend, bukan langsung di source code.

Contoh:

```env
VITE_WS_URL=wss://example.ngrok-free.dev/websocket
```

## Jenis Frame

| Frame WebSocket | Format | Penggunaan |
|---|---|---|
| Text | JSON UTF-8 | Request dan response kontrol |
| Binary | Byte array | Audio terenkripsi (belum tersedia) |

Setiap request kontrol dikirim sebagai satu WebSocket text frame.

## Membuka Koneksi

```js
const socket = new WebSocket(
  import.meta.env.VITE_WS_URL ??
    "ws://localhost:8000/websocket"
);

socket.addEventListener("open", () => {
  console.log("WebSocket connected");
});

socket.addEventListener("close", () => {
  console.log("WebSocket disconnected");
});

socket.addEventListener("error", (error) => {
  console.error("WebSocket error", error);
});
```

Frontend harus menunggu event `open` sebelum mengirim request.

---

## Create Room

Membuat room baru. Koneksi WebSocket yang mengirim request akan menjadi client pertama pada room tersebut.

### Direction

```text
Frontend -> Backend
```

### Frame

```text
WebSocket Text
```

### Request

```json
{
  "type": "create_room"
}
```

### Request fields

| Field | Type | Required | Value/description |
|---|---|---:|---|
| `type` | `string` | Yes | Harus bernilai `"create_room"` |

### Success response

```json
{
  "type": "created",
  "code": "000001"
}
```

### Success response fields

| Field | Type | Description |
|---|---|---|
| `type` | `string` | Bernilai `"created"` |
| `code` | `string` | Kode room sepanjang 6 karakter |

`code` wajib disimpan sebagai `string`, bukan `number`, karena angka nol di depan merupakan bagian dari kode.

Contoh yang benar:

```js
const roomCode = "000001";
```

Contoh yang salah:

```js
const roomCode = 1;
```

### Error response: room gagal dibuat

```json
{
  "type": "error",
  "message": "gagal membuat room"
}
```

### Frontend example

```js
function createRoom(socket) {
  if (socket.readyState !== WebSocket.OPEN) {
    throw new Error("WebSocket belum terhubung");
  }

  socket.send(
    JSON.stringify({
      type: "create_room"
    })
  );
}
```

### Sequence

```text
Frontend                                  Backend
   |                                         |
   |--- WebSocket connect ------------------>| 
   |                                         |
   |--- {"type":"create_room"} ----------->|
   |                                         | create_room(connection_id)
   |                                         |
   |<-- {"type":"created","code":"000001"} |
```

---

## Generic Error Responses

### Field `type` tidak tersedia

Request berikut tidak valid:

```json
{
  "code": "000001"
}
```

Response:

```json
{
  "type": "error",
  "message": "type wajib diisi"
}
```

### Request tidak dikenal

Request:

```json
{
  "type": "request_aneh"
}
```

Response:

```json
{
  "type": "error",
  "message": "request tidak dikenal"
}
```

## Menangani Response di Frontend

```js
socket.addEventListener("message", (event) => {
  if (typeof event.data !== "string") {
    // Binary audio akan ditangani terpisah.
    return;
  }

  let message;

  try {
    message = JSON.parse(event.data);
  } catch {
    console.error("Response server bukan JSON valid");
    return;
  }

  switch (message.type) {
    case "created":
      console.log("Room created:", message.code);
      // Simpan message.code sebagai string pada state frontend.
      break;

    case "error":
      console.error("Server error:", message.message);
      break;

    default:
      console.warn("Unknown server message:", message);
  }
});
```

## TypeScript Types

```ts
export type CreateRoomRequest = {
  type: "create_room";
};

export type RoomCreatedResponse = {
  type: "created";
  code: string;
};

export type ErrorResponse = {
  type: "error";
  message: string;
};

export type ClientTextMessage = CreateRoomRequest;

export type ServerTextMessage =
  | RoomCreatedResponse
  | ErrorResponse;
```

## Catatan Integrasi

- `WebSocket.send()` tidak mengembalikan response. Nilai `undefined` dari pemanggilan `send()` adalah normal.
- Response diterima secara asynchronous melalui event `message`.
- Koneksi harus tetap terbuka setelah room dibuat. Koneksi yang sama nantinya digunakan untuk notifikasi peer dan paket audio.
- Jangan mengirim key enkripsi, private key, atau plaintext audio dalam pesan JSON.
- Jangan mengandalkan teks pada field `message` untuk logika aplikasi. Gunakan field `type`; `message` hanya untuk ditampilkan kepada pengguna/developer.
