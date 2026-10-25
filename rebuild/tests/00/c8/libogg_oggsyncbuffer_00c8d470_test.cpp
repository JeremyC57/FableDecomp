// Behaviour test for 0x00C8D470 ogg_sync_buffer (libogg): copy of the logic.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct ogg_sync_state { unsigned char* data; int storage; int fill; int returned; int unsynced; int headerbytes; int bodybytes; };
char* ogg_sync_buffer(ogg_sync_state* oy, long size)
{
    if (oy->returned) {
        oy->fill -= oy->returned;
        if (oy->fill > 0)
            memmove(oy->data, oy->data + oy->returned, oy->fill);
        oy->returned = 0;
    }
    if (size > oy->storage - oy->fill) {
        long newsize = size + oy->fill + 4096;
        if (oy->data)
            oy->data = (unsigned char*)realloc(oy->data, newsize);
        else
            oy->data = (unsigned char*)malloc(newsize);
        oy->storage = newsize;
    }
    return ((char*)oy->data + oy->fill);
}
int main()
{
    ogg_sync_state oy; memset(&oy, 0, sizeof(oy));
    char* p = ogg_sync_buffer(&oy, 100);
    int ok = p == (char*)oy.data && oy.storage == 4196;
    memcpy(p, "abcdefghij", 10); oy.fill = 10; oy.returned = 4;
    p = ogg_sync_buffer(&oy, 50);                                 // slide out 4 returned bytes, no growth
    ok &= oy.fill == 6 && oy.returned == 0 && oy.storage == 4196 && memcmp(oy.data, "efghij", 6) == 0 && p == (char*)oy.data + 6;
    p = ogg_sync_buffer(&oy, 5000);                               // grow: 5000 + 6 + 4096
    ok &= oy.storage == 9102 && memcmp(oy.data, "efghij", 6) == 0 && p == (char*)oy.data + 6;
    oy.returned = 6; p = ogg_sync_buffer(&oy, 1);                 // everything returned: nothing to move
    ok &= oy.fill == 0 && p == (char*)oy.data;
    free(oy.data);
    if (!ok) { std::printf("00c8d470_TEST FAIL\n"); return 1; }
    std::printf("00c8d470_TEST PASS\n");
    return 0;
}