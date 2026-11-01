// Behaviour test for 0x00C05CB0 crc32_big (zlib 1.2.1 crc32.c): copy of the body over a runtime-built crc_table. On a
// little-endian host the word loop reads each word byte-reversed, so feeding a word-swapped aligned buffer reproduces
// what a big-endian machine computes, which must equal the standard (bytewise) CRC-32.
#include <cstdio>
typedef unsigned int u4;
static u4 crc_table[8][256];
#define REV(w) (((w)>>24)+(((w)>>8)&0xff00)+(((w)&0xff00)<<8)+(((w)&0xff)<<24))
static void make_crc_table()
{
    for (u4 n = 0; n < 256; n++) {
        u4 c = n;
        for (int k = 0; k < 8; k++) c = c & 1 ? 0xedb88320UL ^ (c >> 1) : c >> 1;
        crc_table[0][n] = c; crc_table[4][n] = REV(c);
    }
    for (u4 n = 0; n < 256; n++) {
        u4 c = crc_table[0][n];
        for (int k = 1; k < 4; k++) { c = crc_table[0][c & 0xff] ^ (c >> 8); crc_table[k][n] = c; crc_table[k + 4][n] = REV(c); }
    }
}
#define DOBIG4 c ^= *++buf4; \
        c = crc_table[4][c & 0xff] ^ crc_table[5][(c >> 8) & 0xff] ^ \
            crc_table[6][(c >> 16) & 0xff] ^ crc_table[7][c >> 24]
#define DOBIG32 DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4
static u4 crc32_big(u4 crc, const unsigned char* buf, unsigned len)
{
    u4 c = REV(crc);
    c = ~c;
    while (len && ((unsigned long)buf & 3)) { c = crc_table[4][(c >> 24) ^ *buf++] ^ (c << 8); len--; }
    const u4* buf4 = (const u4*)buf;
    buf4--;
    while (len >= 32) { DOBIG32; len -= 32; }
    while (len >= 4) { DOBIG4; len -= 4; }
    buf4++;
    buf = (const unsigned char*)buf4;
    if (len) do { c = crc_table[4][(c >> 24) ^ *buf++] ^ (c << 8); } while (--len);
    c = ~c;
    return REV(c);
}
static u4 crc_bytewise(u4 crc, const unsigned char* b, unsigned len)
{
    u4 c = ~crc;
    while (len--) c = crc_table[0][(c ^ *b++) & 0xff] ^ (c >> 8);
    return ~c;
}
int main()
{
    make_crc_table();
    u4 a[32], b[32];
    unsigned char* data = (unsigned char*)a;
    unsigned char* sw = (unsigned char*)b;
    for (int i = 0; i < 128; i++) data[i] = (unsigned char)(i * 37 + 11);
    for (int i = 0; i < 128; i++) sw[i] = data[(i & ~3) + 3 - (i & 3)];
    for (unsigned len = 0; len <= 128; len += 4)
        if (crc32_big(0x5678u, sw, len) != crc_bytewise(0x5678u, data, len)) { std::printf("00c05cb0_TEST FAIL\n"); return 1; }
    std::printf("00c05cb0_TEST PASS\n");
    return 0;
}