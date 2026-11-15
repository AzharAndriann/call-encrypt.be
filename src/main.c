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
#include <openssl/sha.h>
#include <string.h>
#include "crypto.h"

// void sha256(const char *string) {
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     SHA256_Update(&sha256, string, strlen(string));
//     SHA256_Final(hash, &sha256);

//     for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
//         printf("%02x", hash[i]);
//     }
//     printf("\n");
// }

int main(void) {
  printf("Menjalankan crypto...\n");

  if (!crypto_demo()){
  printf("Gagal\n");
  }
  printf("Berhasil\n");
  
}
