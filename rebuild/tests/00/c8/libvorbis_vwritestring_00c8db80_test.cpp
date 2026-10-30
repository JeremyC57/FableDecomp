// Behaviour test for libvorbis info.c _v_writestring (0x00C8DB80): write `bytes` characters as 8-bit fields.
#include <cstdio>
struct pack { unsigned char out[16]; int n; };
static void oggpack_write(pack* b, unsigned long value, int bits) { if (bits == 8) b->out[b->n++] = (unsigned char)value; }
static void _v_writestring(pack* o, const char* s, int bytes) { while (bytes--) oggpack_write(o, *s++, 8); }
int main()
{
    pack p; p.n = 0;
    _v_writestring(&p, "vorbis", 6);
    if (p.n != 6 || p.out[0] != 'v' || p.out[5] != 's') { std::printf("00c8db80_TEST FAIL\n"); return 1; }
    _v_writestring(&p, "x", 0);
    if (p.n != 6) { std::printf("00c8db80_TEST FAIL\n"); return 1; }
    std::printf("00c8db80_TEST PASS\n"); return 0;
}