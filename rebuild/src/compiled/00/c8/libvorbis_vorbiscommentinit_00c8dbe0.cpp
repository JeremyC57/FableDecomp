// libvorbis::vorbis_comment_init(vorbis_comment* vc) -- Xiph libvorbis info.c: zero the 16-byte vorbis_comment
// Leaf: @vorbis_comment_init@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DBE0, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); the 16-byte memset (four stores of the zero register) of vorbis_comment {user_comments, comment_lengths, comments, vendor}; it precedes vorbis_comment_add 0x00C8DBF0 (agent446, info.c order).
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was C3DGroup2::C3DGroup2 (placeholder C00c8dbe0::M), a propagated game-side name over an invented model.
#include <string.h>
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};

extern "C" void __fastcall vorbis_comment_init(vorbis_comment* vc)
{
    memset(vc, 0, sizeof(*vc));
}
