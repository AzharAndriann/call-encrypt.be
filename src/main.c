// #include <stdio.h>
// #include "mongoose.h"

// static const char *s_listen_on = "ws://localhost:8000";

// static void fn(struct mg_connection *c, int ev, void *ev_data){
//   if (ev == MG_EV_HTTP_MSG) {
//     struct mg_http_message *hm = (struct mg_http_message *) ev_data;
//     if (mg_match(hm->uri, mg_str("/websocket"), NULL)) {
//       // Upgrade to websocket. From now on, connection is full-duplex
//       // Websocket connection, which will receive MG_EV_WS_MSG events.
//       mg_ws_upgrade(c, hm, NULL);
//     } else if (mg_match(hm->uri, mg_str("/rest"), NULL)) {
//       // Serve REST response
//       mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);
//     } else {
//       // Serve static files
//       mg_http_reply(c, 400, "", "{\"result\": %d}\n", 123);
//     }
//   } else if (ev == MG_EV_WS_MSG) {
//     // Got websocket frame. Received data is wm->data. Echo it back!
//     struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
//     mg_ws_send(c, wm->data.buf, wm->data.len, WEBSOCKET_OP_TEXT);
//   }
// }

// int main(void) {
//   struct mg_mgr mgr;
//   mg_mgr_init(&mgr);

//   printf("Server jalan di %s/websocket\n", s_listen_on);
//   mg_http_listen(&mgr, s_listen_on, fn, NULL);

//   for (;;) mg_mgr_poll(&mgr, 1000);  // event loop, jalan terus

//   mg_mgr_free(&mgr);
//   return 0;
// }

#include <stdio.h>
#include <openssl/rand.h>
#include <string.h>
#include "crypto.h"

#define BUFFER_SIZE 1024


int main(void) {
  char input[BUFFER_SIZE];
  unsigned char key[KEY_SIZE];
  unsigned char nonce[NONCE_SIZE];
  unsigned char tag[TAG_SIZE];

  unsigned char ciphertext[BUFFER_SIZE];
  unsigned char decrypted[BUFFER_SIZE + 1];

  printf("Masukkan Pesan\n");

  if (fgets(input, sizeof(input), stdin) == NULL) {
    fprintf(stderr, "Gagal membaca input\n");
    return 1;
}

/* Menghapus newline dari fgets() */
input[strcspn(input, "\n")] = '\0';

if (RAND_bytes(key, sizeof(key)) != 1 ||
    RAND_bytes(nonce, sizeof(nonce)) != 1) {
    fprintf(stderr, "Gagal membuat key atau nonce\n");
    return 1;
}

int ciphertext_len = crypto_encrypt(
    (const unsigned char *)input,
    (int)strlen(input),
    key,
    nonce,
    ciphertext,
    tag
);

if (ciphertext_len < 0) {
    fprintf(stderr, "Enkripsi gagal\n");
    return 1;
}

printf("Ciphertext: ");

for (int i = 0; i < ciphertext_len; i++) {
    printf("%02x", ciphertext[i]);
}

printf("\n");

int decrypted_len = crypto_decrypt(
    ciphertext,
    ciphertext_len,
    key,
    nonce,
    tag,
    decrypted
);

if (decrypted_len < 0) {
    fprintf(stderr, "Dekripsi gagal: tag tidak valid\n");
    return 1;
}

/* Tambahkan terminator agar bisa dicetak sebagai string */
decrypted[decrypted_len] = '\0';

printf("Hasil dekripsi: %s\n", decrypted);

return 0;
}
  

