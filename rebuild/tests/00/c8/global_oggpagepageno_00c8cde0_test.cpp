// Behaviour test for 0x00C8CDE0 ogg_page_pageno (libogg): copy of the logic.
#include <cstdio>
struct ogg_page { unsigned char* header; long header_len; unsigned char* body; long body_len; };
long ogg_page_pageno(ogg_page* og)
{
    return (og->header[18] |
            (og->header[19] << 8) |
            (og->header[20] << 16) |
            (og->header[21] << 24));
}
int main()
{
    unsigned char h[27]; int i;
    for (i = 0; i < 27; i++) h[i] = (unsigned char)(0xA0 + i);
    h[18] = 0x78; h[19] = 0x56; h[20] = 0x34; h[21] = 0x12;
    ogg_page og; og.header = h; og.header_len = 27; og.body = 0; og.body_len = 0;
    int ok = ogg_page_pageno(&og) == 0x12345678L;
    h[18] = 7; h[19] = 0; h[20] = 0; h[21] = 0; ok &= ogg_page_pageno(&og) == 7;
    if (!ok) { std::printf("00c8cde0_TEST FAIL\n"); return 1; }
    std::printf("00c8cde0_TEST PASS\n");
    return 0;
}