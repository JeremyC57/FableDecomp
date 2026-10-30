// Behaviour test for zlib inflate.c syncsearch (0x00C039F0): find the 00 00 FF FF stored-block marker.
#include <cstdio>
static unsigned syncsearch(unsigned* have, unsigned char* buf, unsigned len)
{
    unsigned got = *have, next = 0;
    while (next < len && got < 4) {
        if ((int)(buf[next]) == (got < 2 ? 0 : 0xff)) got++;
        else if (buf[next]) got = 0;
        else got = 4 - got;
        next++;
    }
    *have = got;
    return next;
}
int main()
{
    unsigned char b[] = { 7, 0, 0, 0xff, 0xff, 9 }; unsigned have = 0;
    if (syncsearch(&have, b, 6) != 5 || have != 4) { std::printf("00c039f0_TEST FAIL\n"); return 1; }
    unsigned char c[] = { 0, 0, 0xff }; have = 0;
    if (syncsearch(&have, c, 3) != 3 || have != 3) { std::printf("00c039f0_TEST FAIL\n"); return 1; }
    std::printf("00c039f0_TEST PASS\n"); return 0;
}