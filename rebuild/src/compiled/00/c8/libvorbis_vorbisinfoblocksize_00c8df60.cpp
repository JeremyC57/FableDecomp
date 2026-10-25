// libvorbis::vorbis_info_blocksize(vorbis_info* vi, int zo) -- Xiph libvorbis info.c: the short (zo 0) or long (zo 1) block size from the codec setup, -1 without one
// Leaf: @vorbis_info_blocksize@8
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DF60, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); ci = vi->codec_setup (+0x1c); `ci ? ci->blocksizes[zo] : -1` (blocksizes[2] lead codec_setup_info), the info.c body; it sits between vorbis_comment_clear 0x00C8DEF0 and vorbis_info_init 0x00C8DF70 (agent446).
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was CEditWorld::DrawGetWorldMap (Get_c8df60), a propagated game-side name over an invented model.
struct vorbis_info {                    // 0x20 bytes
    int   version;                      // +0x00
    int   channels;                     // +0x04
    long  rate;                         // +0x08
    long  bitrate_upper;                // +0x0c
    long  bitrate_nominal;              // +0x10
    long  bitrate_lower;                // +0x14
    long  bitrate_window;               // +0x18
    void* codec_setup;                  // +0x1c (codec_setup_info, 0xE80 bytes)
};
struct codec_setup_info {               // 0xE80 bytes; only its head is read here
    long blocksizes[2];                 // +0x00
};

extern "C" int __fastcall vorbis_info_blocksize(vorbis_info* vi, int zo)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    return ci ? ci->blocksizes[zo] : -1;
}
