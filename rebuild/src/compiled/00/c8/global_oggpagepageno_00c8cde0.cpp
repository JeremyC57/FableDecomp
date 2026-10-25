// ogg_page_pageno(ogg_page* og) -- libogg framing: the page sequence number, little-endian bytes 18..21 of the page header
// Leaf: ogg_page_pageno
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CDE0, length 29 bytes.
// Library: libpk's statically linked Xiph libogg (framing.c, libVorbis I 20030909 era). No PDB/RTTI: libogg API name.
// Convention: LTCG __fastcall (og in ecx). No callees. Twin of ogg_page_serialno 0x00C8CDC0 (bytes 14..17, landed).
// Shape: the libogg expression `h[18] | h[19]<<8 | h[20]<<16 | h[21]<<24`; VC7.1 reassociates it into
// `((h21:h20 in cx) << 8 | h19) << 8 | h18` (`xor ecx,ecx ; mov ch ; mov cl`).
struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

extern "C" long __fastcall ogg_page_pageno(ogg_page* og)
{
    return (og->header[18] |
            (og->header[19] << 8) |
            (og->header[20] << 16) |
            (og->header[21] << 24));
}