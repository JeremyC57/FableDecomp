#include <cstdio>
// ogg_page_version: header byte 4.
struct ogg_page { unsigned char* header; int header_len; unsigned char* body; int body_len; };
static int ogg_page_version(ogg_page* og) { return (int)(og->header[4]); }
int main()
{
    unsigned char h[27] = { 'O', 'g', 'g', 'S', 0, 2 };
    ogg_page p; p.header = h; p.header_len = 27; p.body = 0; p.body_len = 0;
    if (ogg_page_version(&p) != 0) { std::printf("00c8ccf0_TEST FAIL\n"); return 1; }
    h[4] = 0xff;
    if (ogg_page_version(&p) != 255) { std::printf("00c8ccf0_TEST FAIL\n"); return 1; }
    std::printf("00c8ccf0_TEST PASS\n");
    return 0;
}