// libvorbis::host_is_big_endian(void) -- Xiph vorbisfile.c: vorbisfile.c's endianness probe, folded to `return 0` on x86
// Leaf: @host_is_big_endian@0
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8BB10, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); it is `xor eax,eax ; ret` and its only caller 0x00C8BB20 calls it first thing (vorbisfile.c ov_read opens with `int host_endian = host_is_big_endian();`) before the decode calls; it sits between ov_comment 0x00C8BAE0 (agent446) and that ov_read, the vorbisfile.c order. Under LTCG the static function keeps a symbol; it is declared extern here so VC7.1 emits it.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was global AddChildPrimitive, a propagated game-side name over an invented model.

extern "C" int __fastcall host_is_big_endian(void)
{
    long pattern = 0xfeedface;          // ogg_int32_t
    unsigned char* bytewise = (unsigned char*)&pattern;
    if (bytewise[0] == 0xfe) return 1;
    return 0;
}
