// libogg::ogg_page_granulepos(ogg_page* og) -- libogg framing: the page's 64-bit granule position, little-endian bytes 6..13 of the page header
// Leaf: @ogg_page_granulepos@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CD30, length 132 bytes (144 with padding).
// Library: libpk's statically linked Xiph libogg (framing.c; the libvorbis it serves is "Xiph.Org libVorbis I
// 20030909"). No PDB/RTTI: the name is the libogg API name. Convention: LTCG __fastcall (og in ecx). No callees.
// Shape: the libogg body verbatim: each `page[n]&0xff` is an int widened to ogg_int64_t (`cdq`), the running value
// shifted as a 64-bit pair (`shld edi,esi,8 ; shl esi,8`) and or'ed in; the result returns in edx:eax.
typedef __int64 ogg_int64_t;

struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

extern "C" ogg_int64_t __fastcall ogg_page_granulepos(ogg_page* og)
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