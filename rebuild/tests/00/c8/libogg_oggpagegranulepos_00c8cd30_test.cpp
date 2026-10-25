// Behaviour test for 0x00C8CD30 ogg_page_granulepos (libogg): copy of the logic.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
struct ogg_page { unsigned char* header; long header_len; unsigned char* body; long body_len; };
ogg_int64_t ogg_page_granulepos(ogg_page* og)
{
    unsigned char* page = og->header;
    ogg_int64_t granulepos = page[13] & (0xff);
    granulepos = (granulepos << 8) | (page[12] & 0xff);
    granulepos = (granulepos << 8) | (page[11] & 0xff);
    granulepos = (granulepos << 8) | (page[10] & 0xff);
    granulepos = (granulepos << 8) | (page[9] & 0xff);
    granulepos = (granulepos << 8) | (page[8] & 0xff);
    granulepos = (granulepos << 8) | (page[7] & 0xff);
    granulepos = (granulepos << 8) | (page[6] & 0xff);
    return (granulepos);
}
int main()
{
    unsigned char h[27]; int i;
    for (i = 0; i < 27; i++) h[i] = 0;
    for (i = 0; i < 8; i++) h[6 + i] = (unsigned char)(0x01 + i * 0x11);      // 01 12 23 34 45 56 67 78
    ogg_page og; og.header = h; og.header_len = 27; og.body = 0; og.body_len = 0;
    ogg_int64_t want = ((ogg_int64_t)0x78675645 << 32) | (ogg_int64_t)0x34231201;
    int ok = ogg_page_granulepos(&og) == want;
    for (i = 0; i < 8; i++) h[6 + i] = 0xff;                                  // -1 (no packet finishes)
    ok &= ogg_page_granulepos(&og) == (ogg_int64_t)-1;
    if (!ok) { std::printf("00c8cd30_TEST FAIL\n"); return 1; }
    std::printf("00c8cd30_TEST PASS\n");
    return 0;
}