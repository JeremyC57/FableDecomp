// Behaviour test for zlib crc32 (0x00C05FD0): NULL buffer gives 0, else the BYFOUR little-endian worker's CRC-32.
#include <cstdio>
typedef unsigned int u4;
static u4 crc_table[4][256];
static void make_tables()
{
    for (u4 n = 0; n < 256; n++) { u4 c = n; for (int k = 0; k < 8; k++) c = c & 1 ? 0xedb88320u ^ (c >> 1) : c >> 1; crc_table[0][n] = c; }
    for (u4 n = 0; n < 256; n++) { u4 c = crc_table[0][n]; for (int k = 1; k < 4; k++) { c = crc_table[0][c & 0xff] ^ (c >> 8); crc_table[k][n] = c; } }
}
#define DOLIT4 c ^= *buf4++; c = crc_table[3][c & 0xff] ^ crc_table[2][(c >> 8) & 0xff] ^ crc_table[1][(c >> 16) & 0xff] ^ crc_table[0][c >> 24]
static u4 crc32_little(u4 crc, const unsigned char* buf, unsigned len)
{
    u4 c = ~crc; const u4* buf4;
    while (len && ((unsigned long)buf & 3)) { c = crc_table[0][(c ^ *buf++) & 0xff] ^ (c >> 8); len--; }
    buf4 = (const u4*)buf;
    while (len >= 32) { DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; len -= 32; }
    while (len >= 4) { DOLIT4; len -= 4; }
    buf = (const unsigned char*)buf4;
    if (len) do { c = crc_table[0][(c ^ *buf++) & 0xff] ^ (c >> 8); } while (--len);
    return ~c;
}
static u4 z_crc32(u4 crc, const unsigned char* buf, unsigned len) { if (buf == 0) return 0; return crc32_little(crc, buf, len); }
int main()
{
    make_tables();
    static unsigned char data[80]; const char* t = "123456789";
    for (int i = 0; i < 9; i++) data[i + 1] = (unsigned char)t[i];
    for (int i = 0; i < 64; i++) data[16 + i] = (unsigned char)(i * 37);
    if (z_crc32(5, 0, 3) != 0 || z_crc32(0, data + 1, 9) != 0xCBF43926u) { std::printf("00c05fd0_TEST FAIL\n"); return 1; }
    u4 a = z_crc32(0, data + 16, 64), b = z_crc32(z_crc32(0, data + 16, 7), data + 23, 57);
    if (a != b) { std::printf("00c05fd0_TEST FAIL\n"); return 1; }
    std::printf("00c05fd0_TEST PASS\n"); return 0;
}