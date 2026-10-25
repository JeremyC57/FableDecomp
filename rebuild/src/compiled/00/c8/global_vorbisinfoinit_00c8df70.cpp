// vorbis_info_init(vorbis_info* vi) -- Xiph libvorbis info.c: zero the vorbis_info and hang a calloc'd 0xE80-byte codec_setup_info off it
// Leaf: @vorbis_info_init@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DF70, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); memset of the eight-word vorbis_info, then codec_setup (+0x1c) = calloc(1, 0xE80) through the MSVCR71 IAT 0x01440058 (sizeof(codec_setup_info) per agent446), the info.c body; the landed source already named it as vorbis_info_init.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was SCodecInfo_00c8df70::Init, a propagated game-side name over an invented model.
#include <string.h>
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
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // MSVCR71 IAT 0x01440058

extern "C" void __fastcall vorbis_info_init(vorbis_info* vi)
{
    memset(vi, 0, sizeof(*vi));
    vi->codec_setup = calloc(1, 0xE80);   // sizeof(codec_setup_info)
}
