// Behaviour test for libogg ogg_stream_pagein (0x00C8D690): the libogg 1.1 framing.c body on hand-built pages -- serial
// and version rejection, segment-table decomposition into lacing values (bos 0x100 flag, granulepos on the last complete
// packet), body append, returned-data compaction, the sequence-gap 0x400 mark with continued-packet skipping, and eos 0x200.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#endif
typedef struct { unsigned char* header; long header_len; unsigned char* body; long body_len; } ogg_page;
typedef struct {
    unsigned char* body_data; long body_storage, body_fill, body_returned;
    int* lacing_vals; ogg_int64_t* granule_vals; long lacing_storage, lacing_fill, lacing_packet, lacing_returned;
    unsigned char header[282]; int header_fill, e_o_s, b_o_s; long serialno, pageno; ogg_int64_t packetno, granulepos;
} ogg_stream_state;
static int ogg_page_version(ogg_page* og) { return ((int)(og->header[4])); }
static int ogg_page_continued(ogg_page* og) { return ((int)(og->header[5] & 0x01)); }
static int ogg_page_bos(ogg_page* og) { return ((int)(og->header[5] & 0x02)); }
static int ogg_page_eos(ogg_page* og) { return ((int)(og->header[5] & 0x04)); }
static ogg_int64_t ogg_page_granulepos(ogg_page* og)
{
    unsigned char* page = og->header; ogg_int64_t g = page[13] & 0xff;
    for (int i = 12; i >= 6; i--) g = (g << 8) | (page[i] & 0xff);
    return g;
}
static int ogg_page_serialno(ogg_page* og) { return og->header[14] | (og->header[15] << 8) | (og->header[16] << 16) | (og->header[17] << 24); }
static long ogg_page_pageno(ogg_page* og) { return og->header[18] | (og->header[19] << 8) | (og->header[20] << 16) | (og->header[21] << 24); }
static void _os_body_expand(ogg_stream_state* os, int needed)
{
    if (os->body_storage <= os->body_fill + needed) {
        os->body_storage += (needed + 1024);
        os->body_data = (unsigned char*)realloc(os->body_data, os->body_storage);
    }
}
static void _os_lacing_expand(ogg_stream_state* os, int needed)
{
    if (os->lacing_storage <= os->lacing_fill + needed) {
        os->lacing_storage += (needed + 32);
        os->lacing_vals = (int*)realloc(os->lacing_vals, os->lacing_storage * sizeof(*os->lacing_vals));
        os->granule_vals = (ogg_int64_t*)realloc(os->granule_vals, os->lacing_storage * sizeof(*os->granule_vals));
    }
}
static int ogg_stream_pagein(ogg_stream_state* os, ogg_page* og)
{
    unsigned char* header = og->header; unsigned char* body = og->body; long bodysize = og->body_len; int segptr = 0;
    int version = ogg_page_version(og); int continued = ogg_page_continued(og); int bos = ogg_page_bos(og); int eos = ogg_page_eos(og);
    ogg_int64_t granulepos = ogg_page_granulepos(og); int serialno = ogg_page_serialno(og); long pageno = ogg_page_pageno(og);
    int segments = header[26];
    {
        long lr = os->lacing_returned; long br = os->body_returned;
        if (br) { os->body_fill -= br; if (os->body_fill) memmove(os->body_data, os->body_data + br, os->body_fill); os->body_returned = 0; }
        if (lr) {
            if (os->lacing_fill - lr) {
                memmove(os->lacing_vals, os->lacing_vals + lr, (os->lacing_fill - lr) * sizeof(*os->lacing_vals));
                memmove(os->granule_vals, os->granule_vals + lr, (os->lacing_fill - lr) * sizeof(*os->granule_vals));
            }
            os->lacing_fill -= lr; os->lacing_packet -= lr; os->lacing_returned = 0;
        }
    }
    if (serialno != os->serialno) return (-1);
    if (version > 0) return (-1);
    _os_lacing_expand(os, segments + 1);
    if (pageno != os->pageno) {
        int i;
        for (i = os->lacing_packet; i < os->lacing_fill; i++) os->body_fill -= os->lacing_vals[i] & 0xff;
        os->lacing_fill = os->lacing_packet;
        if (os->pageno != -1) { os->lacing_vals[os->lacing_fill++] = 0x400; os->lacing_packet++; }
        if (continued) {
            bos = 0;
            for (; segptr < segments; segptr++) {
                int val = header[27 + segptr]; body += val; bodysize -= val;
                if (val < 255) { segptr++; break; }
            }
        }
    }
    if (bodysize) { _os_body_expand(os, bodysize); memcpy(os->body_data + os->body_fill, body, bodysize); os->body_fill += bodysize; }
    {
        int saved = -1;
        while (segptr < segments) {
            int val = header[27 + segptr];
            os->lacing_vals[os->lacing_fill] = val; os->granule_vals[os->lacing_fill] = -1;
            if (bos) { os->lacing_vals[os->lacing_fill] |= 0x100; bos = 0; }
            if (val < 255) saved = os->lacing_fill;
            os->lacing_fill++; segptr++;
            if (val < 255) os->lacing_packet = os->lacing_fill;
        }
        if (saved != -1) os->granule_vals[saved] = granulepos;
    }
    if (eos) { os->e_o_s = 1; if (os->lacing_fill > 0) os->lacing_vals[os->lacing_fill - 1] |= 0x200; }
    os->pageno = pageno + 1;
    return (0);
}
static void mkpage(unsigned char* h, int flags, long long gp, int serial, int pageno, int nseg, const int* segs)
{
    std::memset(h, 0, 282); std::memcpy(h, "OggS", 4); h[5] = (unsigned char)flags;
    for (int i = 0; i < 8; i++) h[6 + i] = (unsigned char)(gp >> (8 * i));
    for (int i = 0; i < 4; i++) { h[14 + i] = (unsigned char)(serial >> (8 * i)); h[18 + i] = (unsigned char)(pageno >> (8 * i)); }
    h[26] = (unsigned char)nseg; for (int i = 0; i < nseg; i++) h[27 + i] = (unsigned char)segs[i];
}
int main()
{
    ogg_stream_state os; std::memset(&os, 0, sizeof(os)); os.serialno = 42; os.pageno = -1;
    unsigned char h[282], body[600]; for (int i = 0; i < 600; i++) body[i] = (unsigned char)i;
    int bad = 0;
    int s1[3] = { 255, 45, 10 };                 // packet A = 255+45 bytes, packet B = 10 bytes
    mkpage(h, 0x02, 1234, 42, 0, 3, s1); ogg_page og = { h, 30, body, 310 };
    if (ogg_stream_pagein(&os, &og) != 0) bad = 1;
    if (os.lacing_fill != 3 || os.lacing_packet != 3 || os.body_fill != 310 || os.pageno != 1) bad = 2;
    if (os.lacing_vals[0] != (255 | 0x100) || os.lacing_vals[1] != 45 || os.lacing_vals[2] != 10) bad = 3;
    if (os.granule_vals[0] != -1 || os.granule_vals[1] != -1 || os.granule_vals[2] != 1234) bad = 4;
    if (std::memcmp(os.body_data, body, 310) != 0) bad = 5;
    mkpage(h, 0, 0, 43, 1, 0, s1); if (ogg_stream_pagein(&os, &og) != -1) bad = 6;          // wrong serial
    h[4] = 1; h[14] = 42; if (ogg_stream_pagein(&os, &og) != -1) bad = 7;                    // version > 0
    // first 300 bytes / 2 lacing values returned; next page out of sequence (pageno 5), continued, eos
    os.body_returned = 300; os.lacing_returned = 2;
    int s2[3] = { 20, 30, 40 };
    mkpage(h, 0x05, 999, 42, 5, 3, s2); og.body = body + 100; og.body_len = 90;
    if (ogg_stream_pagein(&os, &og) != 0) bad = 8;
    // compaction leaves B (10 bytes, lacing 10); the gap adds 0x400; the continued segment (20) is skipped
    if (os.body_fill != 10 + 70 || os.lacing_fill != 4 || os.pageno != 6 || !os.e_o_s) bad = 9;
    if (os.lacing_vals[0] != 10 || os.lacing_vals[1] != 0x400 || os.lacing_vals[2] != 30 || os.lacing_vals[3] != (40 | 0x200)) bad = 10;
    if (os.granule_vals[3] != 999 || os.body_data[10] != body[120]) bad = 11;
    if (bad) { std::printf("00c8d690_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c8d690_TEST PASS\n"); return 0;
}