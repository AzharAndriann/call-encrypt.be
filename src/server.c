#include<stdio.h>
#include"mongoose.h"
#include"server.h"

static const char *s_listen_on = "ws://localhost:8000";

static void fn(
  struct mg_connection *c,
  int ev,
  void *ev_data
) {
  if (ev == MG_EV_HTTP_MSG) {
      struct mg_http_message *hm = ev_data;

      if (
          mg_match(
              hm->uri,
              mg_str("/websocket"),
              NULL
          )
      ) {
          mg_ws_upgrade(c, hm, NULL);
      } else {
          mg_http_reply(
              c,
              404,
              "",
              "Not found\n"
          );
      }
  }

  else if (ev == MG_EV_WS_MSG) {
      struct mg_ws_message *wm = ev_data;

      mg_ws_send(
          c,
          wm->data.buf,
          wm->data.len,
          WEBSOCKET_OP_TEXT
      );
  }
}

int start_server(void)
{
    struct mg_mgr mgr;

    mg_mgr_init(&mgr);

    if (
        mg_http_listen(
            &mgr,
            s_listen_on,
            fn,
            NULL
        ) == NULL
    ) {
        fprintf(stderr, "Gagal menjalankan server\n");
        mg_mgr_free(&mgr);
        return 1;
    }

    printf(
        "Server jalan di "
        "ws://localhost:8000/websocket\n"
    );

    for (;;) {
        mg_mgr_poll(&mgr, 100);
    }

    mg_mgr_free(&mgr);
    return 0;
}