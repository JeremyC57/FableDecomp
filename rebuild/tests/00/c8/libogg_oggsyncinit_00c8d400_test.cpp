// Behaviour test for 0x00C8D400 libogg ogg_sync_init: zero the sync state (null-safe), return 0
#include <stdio.h>
#include <string.h>
#if !defined(_MSC_VER)
#define __fastcall
#endif
typedef struct {
    unsigned char* data;
    int storage;
    int fill;
    int returned;
    int unsynced;
    int headerbytes;
    int bodybytes;
} ogg_sync_state;
extern "C" int __fastcall ogg_sync_init(ogg_sync_state* oy)
{
    if (oy) {
        memset(oy, 0, sizeof(*oy));
    }
    return (0);
}
static unsigned char gBuf[4];
int main()
{
    ogg_sync_state s;
    s.data = gBuf; s.storage = 1; s.fill = 2; s.returned = 3; s.unsynced = 4; s.headerbytes = 5; s.bodybytes = 6;
    bool ok = true;
    ok = ok && ogg_sync_init(&s) == 0;
    ok = ok && s.data == 0 && s.storage == 0 && s.fill == 0 && s.returned == 0;
    ok = ok && s.unsynced == 0 && s.headerbytes == 0 && s.bodybytes == 0;
    ok = ok && ogg_sync_init(0) == 0;            // null state: nothing touched, still 0
    if (ok) { printf("00c8d400_TEST PASS\n"); return 0; }
    printf("00c8d400_TEST FAIL\n");
    return 1;
}