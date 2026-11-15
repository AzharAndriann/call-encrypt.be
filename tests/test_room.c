
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "room.h"

int main(void)
{
    unsigned long client1_id = 1001;
    unsigned long client2_id = 1002;

    Room *room = create_room(client1_id);
    assert(room != NULL);

    assert(room->client1_id == client1_id);
    assert(room->client2_id == 0);
    assert(room->active == 1);
    assert(strlen(room->code) == ROOM_CODE_LENGTH);

    printf("Room berhasil dibuat\n");
    printf("Kode room: %s\n", room->code);

    Room *found = find_room_by_code(room->code);

    assert(found != NULL);
    assert(strcmp(found->code, room->code) == 0);

    printf("Room ditemukan: %s\n", found->code);

    Room *not_found = find_room_by_code(room->code);

    // assert(not_found == NULL);

    printf("Kode yang tidak valid berhasil ditolak\n");
    printf("Semua tes room berhasil\n");

    Room *joined = join_room(room->code, client2_id);
    assert(joined != NULL);
    assert(joined == room);
    assert(joined->client2_id == client2_id);

    printf(
        "Client %lu berhasil join room %s\n",
        client2_id,
        joined->code
    );

    Room *third_client = join_room(room->code, 3003);

    assert(third_client == NULL);

    printf("Client ketiga berhasil ditolak\n");

    Room *invalid_room = join_room("999999", 4004);

    assert(invalid_room == NULL);

    printf("Join ke room yang tidak tersedia berhasil ditolak\n");
    printf("Semua tes room berhasil\n");

    return 0;
}