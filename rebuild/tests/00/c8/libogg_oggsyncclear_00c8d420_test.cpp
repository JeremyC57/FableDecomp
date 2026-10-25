// Behaviour test for 0x00C8D420 ogg_sync_clear (libogg): copy of the logic with ogg_sync_init (0x00C8D400).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct ogg_sync_state { unsigned char* data; int storage; int fill; int returned; int unsynced; int headerbytes; int bodybytes; };
int ogg_sync_init(ogg_sync_state* oy)
{
    if (oy) memset(oy, 0, sizeof(*oy));
    return (0);
}
int ogg_sync_clear(ogg_sync_state* oy)
{
    if (oy) {
        if (oy->data) free(oy->data);
        ogg_sync_init(oy);
    }
    return (0);
}
int main()
{
    ogg_sync_state oy; oy.data = (unsigned char*)malloc(32); oy.storage = 32; oy.fill = 9; oy.returned = 4;
    oy.unsynced = 1; oy.headerbytes = 27; oy.bodybytes = 5;
    int ok = ogg_sync_clear(&oy) == 0;
    ok &= oy.data == 0 && oy.storage == 0 && oy.fill == 0 && oy.returned == 0 && oy.bodybytes == 0;
    ok &= ogg_sync_clear(&oy) == 0 && ogg_sync_clear(0) == 0;
    if (!ok) { std::printf("00c8d420_TEST FAIL\n"); return 1; }
    std::printf("00c8d420_TEST PASS\n");
    return 0;
}