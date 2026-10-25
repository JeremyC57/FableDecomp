// vorbis_comment_clear(vorbis_comment* vc) -- libvorbis info.c: free every comment string, the comment and length arrays and the vendor string, then zero the record
// Leaf: vorbis_comment_clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DEF0, length 102 bytes (112 with padding).
// Library: libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I 20030909"). No PDB/RTTI: libvorbis API name.
// Convention: LTCG __fastcall (vc in ecx). Callee: MSVCR71 free (IAT 0x01440154, cached in ebx).
// Shape: the libvorbis 1.0.x body verbatim -- the memset sits OUTSIDE the `if (vc)` (the four zero stores are on
// the null path too).
#include <string.h>
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // IAT 0x01440154
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00 (comments + 1 entries, NULL-terminated)
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};

extern "C" void __fastcall vorbis_comment_clear(vorbis_comment* vc)
{
    if (vc) {
        long i;
        for (i = 0; i < vc->comments; i++)
            if (vc->user_comments[i]) free(vc->user_comments[i]);
        if (vc->user_comments) free(vc->user_comments);
        if (vc->comment_lengths) free(vc->comment_lengths);
        if (vc->vendor) free(vc->vendor);
    }
    memset(vc, 0, sizeof(*vc));
}