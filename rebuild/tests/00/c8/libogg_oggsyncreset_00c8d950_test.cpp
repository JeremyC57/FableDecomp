// Behaviour test for 00c8d950: ogg_sync_reset (Xiph libogg framing.c)
#include <cstdio>
#include <cstring>
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;                // +0x00
    int            storage;             // +0x04
    int            fill;                // +0x08
    int            returned;            // +0x0c
    int            unsynced;            // +0x10
    int            headerbytes;         // +0x14
    int            bodybytes;           // +0x18
};

static int ogg_sync_reset(ogg_sync_state* oy)
{
    oy->fill = 0;
    oy->returned = 0;
    oy->unsynced = 0;
    oy->headerbytes = 0;
    oy->bodybytes = 0;
    return (0);
}

int main()
{
    int ok;
    ogg_sync_state oy = {0, 9, 1, 2, 3, 4, 5};
    ok = ogg_sync_reset(&oy) == 0 && oy.storage == 9 && oy.fill == 0 && oy.returned == 0 && oy.unsynced == 0 && oy.headerbytes == 0 && oy.bodybytes == 0;
    std::printf(ok ? "00c8d950_TEST PASS\n" : "00c8d950_TEST FAIL\n");
    return ok ? 0 : 1;
}
