// Behaviour test for 00c8cdc0: ogg_page_serialno (Xiph libogg framing.c)
#include <cstdio>
#include <cstring>
struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

static int ogg_page_serialno(ogg_page* og)
{
    return (og->header[14] |
            (og->header[15] << 8) |
            (og->header[16] << 16) |
            (og->header[17] << 24));
}

int main()
{
    int ok;
    unsigned char h[27] = {0}; h[14] = 0x78; h[15] = 0x56; h[16] = 0x34; h[17] = 0x12; ogg_page og = {h, 27, 0, 0};
    ok = ogg_page_serialno(&og) == 0x12345678;
    std::printf(ok ? "00c8cdc0_TEST PASS\n" : "00c8cdc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
