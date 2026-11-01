// Behaviour test for 0x00C059D0 crc32_little (zlib 1.2.1 crc32.c): copy of the body over a runtime-built crc_table.
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
#define DOLIT4 c ^= *buf4++; \
        c = crc_table[3][c & 0xff] ^ crc_table[2][(c >> 8) & 0xff] ^ \
            crc_table[1][(c >> 16) & 0xff] ^ crc_table[0][c >> 24]
#define DOLIT32 DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4
static u4 crc32_little(u4 crc, const unsigned char* buf, unsigned len)
{
    u4 c = ~crc;
    while (len && ((unsigned long)buf & 3)) { c = crc_table[0][(c ^ *buf++) & 0xff] ^ (c >> 8); len--; }
    const u4* buf4 = (const u4*)buf;
    while (len >= 32) { DOLIT32; len -= 32; }
    while (len >= 4) { DOLIT4; len -= 4; }
    buf = (const unsigned char*)buf4;
    if (len) do { c = crc_table[0][(c ^ *buf++) & 0xff] ^ (c >> 8); } while (--len);
    return ~c;
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
    u4 store[32];
    unsigned char* data = (unsigned char*)store;
    for (int i = 0; i < 128; i++) data[i] = (unsigned char)(i * 37 + 11);
    const char* check = "123456789";
    if (crc32_little(0, (const unsigned char*)check, 9) != 0xCBF43926UL) { std::printf("00c059d0_TEST FAIL\n"); return 1; }
    for (int off = 0; off < 4; off++)
        for (unsigned len = 0; len < 120; len += 7)
            if (crc32_little(0x1234u, data + off, len) != crc_bytewise(0x1234u, data + off, len)) { std::printf("00c059d0_TEST FAIL\n"); return 1; }
    std::printf("00c059d0_TEST PASS\n");
    return 0;
}