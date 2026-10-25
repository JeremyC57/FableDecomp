// Behaviour test for 0x00C8CF60 ogg_page_checksum_set (libogg): copy of the logic over a generated crc_lookup
// table (poly 0x04C11DB7, MSB first), checked against a bitwise CRC of the same bytes.
#include <cstdio>
typedef unsigned int ogg_uint32_t;
struct ogg_page { unsigned char* header; long header_len; unsigned char* body; long body_len; };
static ogg_uint32_t crc_lookup[256];
void ogg_page_checksum_set(ogg_page* og)
{
    if (og) {
        ogg_uint32_t crc_reg = 0;
        int i;
        og->header[22] = 0;
        og->header[23] = 0;
        og->header[24] = 0;
        og->header[25] = 0;
        for (i = 0; i < og->header_len; i++)
            crc_reg = (crc_reg << 8) ^ crc_lookup[((crc_reg >> 24) & 0xff) ^ og->header[i]];
        for (i = 0; i < og->body_len; i++)
            crc_reg = (crc_reg << 8) ^ crc_lookup[((crc_reg >> 24) & 0xff) ^ og->body[i]];
        og->header[22] = (unsigned char)(crc_reg & 0xff);
        og->header[23] = (unsigned char)((crc_reg >> 8) & 0xff);
        og->header[24] = (unsigned char)((crc_reg >> 16) & 0xff);
        og->header[25] = (unsigned char)((crc_reg >> 24) & 0xff);
    }
}
static ogg_uint32_t bitwise(const unsigned char* p, int n, ogg_uint32_t crc)
{
    int i, b;
    for (i = 0; i < n; i++) {
        crc ^= (ogg_uint32_t)p[i] << 24;
        for (b = 0; b < 8; b++) crc = (crc & 0x80000000u) ? (crc << 1) ^ 0x04c11db7u : (crc << 1);
    }
    return crc;
}
int main()
{
    int i;
    for (i = 0; i < 256; i++) { unsigned char c = (unsigned char)i; crc_lookup[i] = bitwise(&c, 1, 0); }
    unsigned char h[28], body[50];
    for (i = 0; i < 28; i++) h[i] = (unsigned char)(i * 7 + 3);
    for (i = 0; i < 50; i++) body[i] = (unsigned char)(i * 13 + 1);
    ogg_page og; og.header = h; og.header_len = 28; og.body = body; og.body_len = 50;
    ogg_page_checksum_set(&og);
    unsigned char hz[28]; for (i = 0; i < 28; i++) hz[i] = h[i];
    hz[22] = hz[23] = hz[24] = hz[25] = 0;
    ogg_uint32_t want = bitwise(body, 50, bitwise(hz, 28, 0));
    ogg_uint32_t got = h[22] | (h[23] << 8) | (h[24] << 16) | ((ogg_uint32_t)h[25] << 24);
    int ok = got == want && crc_lookup[1] == 0x04c11db7u;
    ogg_page_checksum_set(0);
    if (!ok) { std::printf("00c8cf60_TEST FAIL\n"); return 1; }
    std::printf("00c8cf60_TEST PASS\n");
    return 0;
}