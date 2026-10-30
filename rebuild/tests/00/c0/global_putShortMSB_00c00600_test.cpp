// Behaviour test for zlib deflate.c putShortMSB (0x00C00600): big-endian 16-bit append to the pending buffer.
#include <cstdio>
typedef unsigned char Byte; typedef unsigned int uInt;
struct deflate_state { Byte* pending_buf; int pending; };
#define put_byte(s, c) {s->pending_buf[s->pending++] = (c);}
static void putShortMSB(deflate_state* s, uInt b) { put_byte(s, (Byte)(b >> 8)); put_byte(s, (Byte)(b & 0xff)); }
int main()
{
    Byte buf[8] = { 0 }; deflate_state s = { buf, 1 };
    putShortMSB(&s, 0x78DAu);
    if (s.pending != 3 || buf[1] != 0x78 || buf[2] != 0xDA) { std::printf("00c00600_TEST FAIL\n"); return 1; }
    std::printf("00c00600_TEST PASS\n"); return 0;
}