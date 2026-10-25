// Behaviour test for 00c8cd20: ogg_page_eos (Xiph libogg framing.c)
#include <cstdio>
#include <cstring>
struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

static int ogg_page_eos(ogg_page* og)
{
    return ((int)(og->header[5] & 0x04));
}

int main()
{
    int ok;
    unsigned char h[27] = {0}; h[5] = 7; ogg_page og = {h, 27, 0, 0};
    ok = ogg_page_eos(&og) == 4;
    h[5] = (unsigned char)(7 & ~0x04); ok = ok && ogg_page_eos(&og) == 0;
    std::printf(ok ? "00c8cd20_TEST PASS\n" : "00c8cd20_TEST FAIL\n");
    return ok ? 0 : 1;
}
