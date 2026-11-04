// libogg::ogg_stream_pagein(ogg_stream_state* os, ogg_page* og) -- libogg framing: add an incoming page to a logical stream (drop returned data, check serial/version, grow the lacing fifo, unroll a partial packet on a sequence gap, append the body and the segment table, record the granulepos and eos)
// Leaf: @ogg_stream_pagein@8
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8D690, 702 bytes, LTCG __fastcall (os in ecx, og in edx, plain `ret`).
// Library: libogg 1.1 src/framing.c as statically linked into libpk's Xiph decoder (the body is identical in libogg 1.1.1,
//   gitlab.xiph.org xiph/ogg tag v1.1.1). No PDB/RTTI: the name is the libogg API name.
// Identity (two sources): (1) bound `@ogg_stream_pagein@8` already (callers _fetch_headers 0x00C8A0A0,
//   _prefetch_all_headers 0x00C8A1D0, _fetch_and_process_packet 0x00C8A540, ov_raw_seek / ov_pcm_seek_page / ov_pcm_seek);
//   (2) the body: the seven ogg_page_* accessors in source order (version 0x00C8CCF0, continued, bos, eos, granulepos,
//   serialno, pageno 0x00C8CDE0), header[26] segments, the body/lacing memmoves through the MSVCR71 import (0x01440144),
//   `serialno != os->serialno || version > 0 -> -1`, _os_lacing_expand(os,segments+1) 0x00C8CF20, the 0x400 dropped-data
//   lacing mark, _os_body_expand 0x00C8CEF0 + inline memcpy (rep movsd/movsb), the 0x100 bos / 0x200 eos flags, pageno+1 --
//   framing.c ogg_stream_pagein verbatim. It sits between ogg_sync_pageout (~0x00C8D650) and ogg_sync_reset 0x00C8D950,
//   the framing.c order. Catalog label "WeatherRenderer_ProcessStream" is propagated junk.
// Shape: the framing.c slice as one TU. The page accessors are same-TU bodies under `auto_inline(off)` (retail calls them
//   out of line and, knowing they leave ecx alone, reloads og into ecx only after ogg_page_granulepos); the two expand
//   helpers are `static __declspec(noinline)` with retail's private convention (os in esi, count in ecx) chosen by VC7.1.
// Original libogg framing.c; BSD-style terms in reference COPYING.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);         // IAT 0x01440054
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // IAT 0x01440144
#pragma intrinsic(memcpy)
typedef __int64 ogg_int64_t;

typedef struct {
    unsigned char* header;
    long header_len;
    unsigned char* body;
    long body_len;
} ogg_page;

typedef struct {
    unsigned char* body_data;    /* bytes from packet bodies */
    long body_storage;           /* storage elements allocated */
    long body_fill;              /* elements stored; fill mark */
    long body_returned;          /* elements of fill returned */
    int* lacing_vals;            /* The values that will go to the segment table */
    ogg_int64_t* granule_vals;   /* granulepos values for headers */
    long lacing_storage;
    long lacing_fill;
    long lacing_packet;
    long lacing_returned;
    unsigned char header[282];   /* working space for header encode */
    int header_fill;
    int e_o_s;
    int b_o_s;
    long serialno;
    long pageno;
    ogg_int64_t packetno;
    ogg_int64_t granulepos;
} ogg_stream_state;

extern "C" {

#pragma auto_inline(off)
int __fastcall ogg_page_version(ogg_page* og)                                          // 0x00C8CCF0
{
    return ((int)(og->header[4]));
}

int __fastcall ogg_page_continued(ogg_page* og)                                        // 0x00C8CD00
{
    return ((int)(og->header[5] & 0x01));
}

int __fastcall ogg_page_bos(ogg_page* og)                                              // 0x00C8CD10
{
    return ((int)(og->header[5] & 0x02));
}

int __fastcall ogg_page_eos(ogg_page* og)                                              // 0x00C8CD20
{
    return ((int)(og->header[5] & 0x04));
}

ogg_int64_t __fastcall ogg_page_granulepos(ogg_page* og)                               // 0x00C8CD30
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

int __fastcall ogg_page_serialno(ogg_page* og)                                         // 0x00C8CDC0
{
    return (og->header[14] |
            (og->header[15] << 8) |
            (og->header[16] << 16) |
            (og->header[17] << 24));
}

long __fastcall ogg_page_pageno(ogg_page* og)                                          // 0x00C8CDE0
{
    return (og->header[18] |
            (og->header[19] << 8) |
            (og->header[20] << 16) |
            (og->header[21] << 24));
}

#pragma auto_inline(on)

/* Helpers for ogg_stream_encode; this keeps the structure and
   what's happening fairly clear */

static __declspec(noinline) void _os_body_expand(ogg_stream_state* os, int needed)    // 0x00C8CEF0 (static)
{
    if (os->body_storage <= os->body_fill + needed) {
        os->body_storage += (needed + 1024);
        os->body_data = (unsigned char*)realloc(os->body_data, os->body_storage * sizeof(*os->body_data));
    }
}

static __declspec(noinline) void _os_lacing_expand(ogg_stream_state* os, int needed)  // 0x00C8CF20 (static)
{
    if (os->lacing_storage <= os->lacing_fill + needed) {
        os->lacing_storage += (needed + 32);
        os->lacing_vals = (int*)realloc(os->lacing_vals, os->lacing_storage * sizeof(*os->lacing_vals));
        os->granule_vals = (ogg_int64_t*)realloc(os->granule_vals, os->lacing_storage * sizeof(*os->granule_vals));
    }
}

/* add the incoming page to the stream state; we decompose the page
   into packet segments here as well. */

int __fastcall ogg_stream_pagein(ogg_stream_state* os, ogg_page* og)                    // 0x00C8D690
{
    unsigned char* header = og->header;
    unsigned char* body = og->body;
    long bodysize = og->body_len;
    int segptr = 0;

    int version = ogg_page_version(og);
    int continued = ogg_page_continued(og);
    int bos = ogg_page_bos(og);
    int eos = ogg_page_eos(og);
    ogg_int64_t granulepos = ogg_page_granulepos(og);
    int serialno = ogg_page_serialno(og);
    long pageno = ogg_page_pageno(og);
    int segments = header[26];

    /* clean up 'returned data' */
    {
        long lr = os->lacing_returned;
        long br = os->body_returned;

        /* body data */
        if (br) {
            os->body_fill -= br;
            if (os->body_fill)
                memmove(os->body_data, os->body_data + br, os->body_fill);
            os->body_returned = 0;
        }

        if (lr) {
            /* segment table */
            if (os->lacing_fill - lr) {
                memmove(os->lacing_vals, os->lacing_vals + lr,
                        (os->lacing_fill - lr) * sizeof(*os->lacing_vals));
                memmove(os->granule_vals, os->granule_vals + lr,
                        (os->lacing_fill - lr) * sizeof(*os->granule_vals));
            }
            os->lacing_fill -= lr;
            os->lacing_packet -= lr;
            os->lacing_returned = 0;
        }
    }

    /* check the serial number */
    if (serialno != os->serialno) return (-1);
    if (version > 0) return (-1);

    _os_lacing_expand(os, segments + 1);

    /* are we in sequence? */
    if (pageno != os->pageno) {
        int i;

        /* unroll previous partial packet (if any) */
        for (i = os->lacing_packet; i < os->lacing_fill; i++)
            os->body_fill -= os->lacing_vals[i] & 0xff;
        os->lacing_fill = os->lacing_packet;

        /* make a note of dropped data in segment table */
        if (os->pageno != -1) {
            os->lacing_vals[os->lacing_fill++] = 0x400;
            os->lacing_packet++;
        }

        /* are we a 'continued packet' page?  If so, we'll need to skip
           some segments */
        if (continued) {
            bos = 0;
            for (; segptr < segments; segptr++) {
                int val = header[27 + segptr];
                body += val;
                bodysize -= val;
                if (val < 255) {
                    segptr++;
                    break;
                }
            }
        }
    }

    if (bodysize) {
        _os_body_expand(os, bodysize);
        memcpy(os->body_data + os->body_fill, body, bodysize);
        os->body_fill += bodysize;
    }

    {
        int saved = -1;
        while (segptr < segments) {
            int val = header[27 + segptr];
            os->lacing_vals[os->lacing_fill] = val;
            os->granule_vals[os->lacing_fill] = -1;

            if (bos) {
                os->lacing_vals[os->lacing_fill] |= 0x100;
                bos = 0;
            }

            if (val < 255) saved = os->lacing_fill;

            os->lacing_fill++;
            segptr++;

            if (val < 255) os->lacing_packet = os->lacing_fill;
        }

        /* set the granulepos on the last granuleval of the last full packet */
        if (saved != -1) {
            os->granule_vals[saved] = granulepos;
        }

    }

    if (eos) {
        os->e_o_s = 1;
        if (os->lacing_fill > 0)
            os->lacing_vals[os->lacing_fill - 1] |= 0x200;
    }

    os->pageno = pageno + 1;

    return (0);
}

}   // extern "C"