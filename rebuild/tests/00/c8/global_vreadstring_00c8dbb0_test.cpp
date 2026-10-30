// Behaviour test for libvorbis info.c _v_readstring (0x00C8DBB0): read `bytes` 8-bit characters into buf.
#include <cstdio>
struct pack { const unsigned char* in; int n; };
static long oggpack_read(pack* b, int bits) { return bits == 8 ? b->in[b->n++] : -1; }
static void _v_readstring(pack* o, char* buf, int bytes) { while (bytes--) *buf++ = (char)oggpack_read(o, 8); }
int main()
{
    static const unsigned char src[] = { 'X', 'i', 'p', 'h' };
    pack p = { src, 0 }; char buf[5] = { 0 };
    _v_readstring(&p, buf, 4);
    if (p.n != 4 || buf[0] != 'X' || buf[3] != 'h' || buf[4] != 0) { std::printf("00c8dbb0_TEST FAIL\n"); return 1; }
    std::printf("00c8dbb0_TEST PASS\n"); return 0;
}