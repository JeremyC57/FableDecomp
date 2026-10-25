// Behaviour test for 00c8d4f0: ogg_sync_wrote (Xiph libogg framing.c)
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

static int ogg_sync_wrote(ogg_sync_state* oy, long bytes)
{
    if (oy->fill + bytes > oy->storage) return (-1);
    oy->fill += bytes;
    return (0);
}

int main()
{
    int ok;
    ogg_sync_state oy = {0, 10, 4, 0, 0, 0, 0};
    ok = ogg_sync_wrote(&oy, 7) == -1 && oy.fill == 4 && ogg_sync_wrote(&oy, 6) == 0 && oy.fill == 10;
    std::printf(ok ? "00c8d4f0_TEST PASS\n" : "00c8d4f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
