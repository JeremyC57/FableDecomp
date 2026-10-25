// libogg::ogg_page_serialno(ogg_page* og) -- Xiph libogg framing.c: the page serial number, header bytes 14..17 little-endian
// Leaf: @ogg_page_serialno@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CDC0, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); it assembles og->header[14..17] (+0xe..+0x11) little-endian; it sits between ogg_page_bos/eos/granulepos 0x00C8CD00..0x00C8CD30 and ogg_page_pageno 0x00C8CDE0 (agent446, MATCH), the framing.c order.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was global CEngineWeatherRenderer::SetDebugBoxColour1, a propagated game-side name over an invented model.
struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

extern "C" int __fastcall ogg_page_serialno(ogg_page* og)
{
    return (og->header[14] |
            (og->header[15] << 8) |
            (og->header[16] << 16) |
            (og->header[17] << 24));
}
