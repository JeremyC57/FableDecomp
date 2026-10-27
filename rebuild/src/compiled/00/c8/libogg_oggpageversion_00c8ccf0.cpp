// libogg::ogg_page_version(ogg_page* og) -- Xiph framing.c: the stream structure version, header byte 4 of an Ogg page
// Leaf: @ogg_page_version@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CCF0, 7 bytes, __fastcall (og in ecx), plain ret. No callees.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: framing.c's ogg_page_version verbatim (`(int)(og->header[4])`); it precedes the landed ogg_page_continued
//   0x00C8CD00 / bos / eos (header[5] flag readers) exactly as framing.c orders them; caller 0x00C8D6B3 (ogg_stream_pagein's
//   version check). Label correction: the catalog's "ConsoleDisplayCSTimecode" is propagated.
struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

extern "C" int __fastcall ogg_page_version(ogg_page* og)
{
    return ((int)(og->header[4]));
}