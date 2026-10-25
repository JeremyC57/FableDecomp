// ogg_page_continued(ogg_page* og) -- Xiph libogg framing.c: header byte 5 bit 0x01 of an Ogg page (continued packet flag)
// Leaf: @ogg_page_continued@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CD00, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); `mov eax,[ecx] ; movzx eax,byte [eax+5] ; and eax,1` reads og->header[5]; the three flag readers sit in framing.c order (continued, bos, eos) right before ogg_page_granulepos 0x00C8CD30 (agent446, MATCH).
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was unattributed CClass_00c8cd00::ConsoleDisplayCSTimecode, a propagated game-side name over an invented model.
struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

extern "C" int __fastcall ogg_page_continued(ogg_page* og)
{
    return ((int)(og->header[5] & 0x01));
}
